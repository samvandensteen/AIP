/*
*  This file is part of the program: Architecture Independent Profiler (AIP).
*
*  AIP is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  AIP is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with AIP.  If not, see <http://www.gnu.org/licenses/>.
*
*  Authors: Sam Van den Steen, Ghent University
*  Copyright: 2016, Ghent University
*/

#include "utrace_processor.h"

std::unordered_map<uint64_t, const vector<const MicroOp*>* > utrace_processor::instructions;

utrace_processor::utrace_processor(Knobs *k){
	ins_trace_length = k->trace_ins_length;
	mem_trace_length = k->trace_mem_length;
	trace_counter = 0, trace_uopcount = 0, trace_load_count = 0, trace_store_count = 0;

	trace_ended = true;

	pw = new Proto_Writer("utrace", k->enable_compression);
  // add used uop categories
	vector<string> uop_string_id;
	for(int i = 0; i < UOP_TYPE_INVALID; i++){
		uop_string_id.push_back(uop_to_string(static_cast<extended_uop_type>(i)));
	}
	ID_string* ids = make_id_string(uop_string_id);
  pw->write_streaming(ids, true);
	delete ids;

	dependence_processor = new Dependence_Processor(k->approx_dep, k->min_rob, k->step_rob, k->max_rob);
	mlp_proc = new MLP_Processor(k->enable_compression);
}

utrace_processor::~utrace_processor(){
	delete pw;
	delete mlp_proc;
}

void utrace_processor::init_new_trace(){
	// check if the last trace ended
	if(!trace_ended){
		LOG_WARNING("Trace didn't end before inializing a new trace!");
		end_trace();
	}

	trace_buffer = vector<Extended_Micro_Op>(ins_trace_length, Extended_Micro_Op());
	dependence_position = vector<vector<int>>(ins_trace_length, vector<int>());
	trace_uop_type = vector<extended_uop_type>(ins_trace_length, UOP_TYPE_INVALID);
	trace_dependent_loads = vector<int>(ins_trace_length, -1);

	trace_inscount = 0, trace_uopcount = 0, trace_load_count = 0, trace_store_count = 0;

	load_addr_1 = 0;
	load_addr_2 = 0;
	store_addr = 0;

	trace_ended = false;
}

void utrace_processor::end_trace(){
	if(!trace_ended) {
		trace_buffer.resize(trace_uopcount);
		dependence_position.resize(trace_uopcount);
		trace_uop_type.resize(trace_uopcount);
		trace_dependent_loads.resize(trace_uopcount);

		uTrace* ut = make_utrace(trace_counter);
		add_utrace_stats(ut, trace_inscount, trace_uopcount, trace_load_count, trace_store_count);

		vector<uint32_t> uop_ids, uop_freqs;
		map_to_vectors(trace_hist_uops, uop_ids, uop_freqs);
		add_utrace_uop_hist(ut, uop_ids, uop_freqs);

		dependence_processor->process_trace(ut, dependence_position, trace_uop_type, trace_counter);

		pw->write_streaming(ut, true);
		delete ut;

		mlp_proc->process_load_trace(trace_dependent_loads, trace_load_count, trace_counter);

		trace_buffer.clear();
		dependence_position.clear();
		trace_uop_type.clear();
		trace_dependent_loads.clear();
		trace_hist_uops.clear();

		for(int i = 0; i < XED_REG_LAST; i++){
			trace_producers[i] = 0;
		}
		trace_store_buffer.clear();
	}

	trace_counter++;
	trace_ended = true;
}

void utrace_processor::add_instr(uint64_t insAddr, const vector<const MicroOp*>* ins_uops){
	instructions[insAddr] = ins_uops;
}

void utrace_processor::data_load(uint64_t load_addr, int op){
	assert(op < 2 && "ERROR: operand-counter of load instruction is bigger than 1");

	if(op == 0){
		load_addr_1 = load_addr;
	}
	else{
		load_addr_2 = load_addr;
	}
}

void utrace_processor::data_store(uint64_t store_addr){
	this->store_addr = store_addr;
}

bool utrace_processor::process_instr(uint64_t insAddr){
	// due to basic block instrumentation switching, we can still try and process instructions here despite the fact that we should've switched
	if(trace_ended){
		return true;
	}

	vector<const MicroOp*> uop = *instructions[insAddr];

	bool first_load_in_instr = true;
	for(uint32_t i = 0; i < uop.size(); i++){
		if(trace_uopcount >= trace_buffer.size()){
			// make 10% larger at once to prevent multiple consecutive linear resize operations
			int resize_to = trace_buffer.size() * 1.1;

			stringstream ss;
			ss << trace_counter << ": trace buffer too small, resizing to " << resize_to << "!";
			LOG_WARNING(ss.str());

			trace_buffer.resize(resize_to, Extended_Micro_Op());
			dependence_position.resize(resize_to, vector<int>());
			trace_uop_type.resize(resize_to, UOP_TYPE_INVALID);
			trace_dependent_loads.resize(resize_to, -1);
		}

		Extended_Micro_Op current = Extended_Micro_Op(*(uop[i]), trace_uopcount);

		// set addresses of instructions according to profiled info in trace_mem
		bool is_load = current.is_load(), is_store = current.is_store();
		if(is_load){
			trace_hist_uops[UOP_TYPE_LOAD]++;
			if(first_load_in_instr){
				current.set_address(load_addr_1);
				mlp_proc->log_stride_and_reuse_PC(trace_uopcount, insAddr, load_addr_1);
				first_load_in_instr = false;
			}
			else{
				current.set_address(load_addr_2);
				mlp_proc->log_stride_and_reuse_PC(trace_uopcount, insAddr, load_addr_2);
			}
			trace_load_count++;
		}
		else if(is_store){
			trace_hist_uops[UOP_TYPE_STORE]++;
			current.set_address(store_addr);
			trace_store_count++;
		}
		else{
			trace_hist_uops[current.get_extended_uop_type()]++;
		}
		trace_uop_type[trace_uopcount] = current.get_extended_uop_type();

		trace_buffer[trace_uopcount] = current;
		add_dependencies(is_load, is_store);

		trace_uopcount++;

		// stop condition
		if(trace_uopcount >= ins_trace_length && trace_store_count + trace_load_count > mem_trace_length){
		// if((trace_uopcount >= ins_trace_length && trace_load_count > mem_trace_length) || trace_uopcount >= 10 * ins_trace_length){
			end_trace();
			return true;
		}
	}

	trace_inscount++;

	return false;
}

void utrace_processor::add_dependencies(bool is_load, bool is_store){
	Extended_Micro_Op* inserted = &trace_buffer[trace_uopcount];
	MicroOp uop = inserted->get_uop();
	// we will investigate the trace buffer for possible dependences, so here the micro-op still only dependens on itself (and if it's a load, it's only dependent on itself)
	int max_path = 1, dependent_load_index = -1;

	// intra instruction dependencies
	if(uop.intraInstructionDependencies == 1){
		int index = trace_uopcount - 1;
		inserted->add_producer(&trace_buffer[index]);
		dependence_position[trace_uopcount].push_back(index);

		max_path = trace_buffer[index].get_path_length() + 1;

		if(is_load){
			// check if directly dependent on a load
			if(trace_buffer[index].is_load()){
				dependent_load_index = index;
			}
			// check if there is a load in the dependency chain
			else{
				int load_index = trace_buffer[index].get_producing_load_position();
				if(load_index >= 0){
					dependent_load_index = load_index;
				}
			}
		}
	}
	else if(uop.intraInstructionDependencies == 2){
		int index_1 = trace_uopcount - 1;
		int index_2 = trace_uopcount - 2;
		inserted->add_producer(&trace_buffer[index_1]);
		dependence_position[trace_uopcount].push_back(index_1);
		inserted->add_producer(&trace_buffer[index_2]);
		dependence_position[trace_uopcount].push_back(index_2);

		// choose the biggest one (or if they're equal in length of the dependency chain, choose the closest one for most accuarate results)
		if(trace_buffer[index_1].get_path_length() >= trace_buffer[index_2].get_path_length()){
			max_path = trace_buffer[index_1].get_path_length() + 1;
		}
		else{
			max_path = trace_buffer[index_2].get_path_length() + 1;
		}

		if(is_load){
			// check if any of the two previous uops is a load (will be marked)
			if(trace_buffer[index_1].is_load() || trace_buffer[index_2].is_load()){
				if(trace_buffer[index_1].get_no_producing_loads() >= trace_buffer[index_2].get_no_producing_loads()){
					dependent_load_index = index_1;
				}
				else{
					dependent_load_index = index_2;
				}
			}
			// check if, through non-load instructions, we have a load-load dependency
			else{
				int load_index_1 = trace_buffer[index_1].get_producing_load_position(), load_index_2 = trace_buffer[index_2].get_producing_load_position();
				if(load_index_1 >= 0 || load_index_2 >= 0){
					if(trace_buffer[load_index_1].get_no_producing_loads() >= trace_buffer[load_index_2].get_no_producing_loads()){
						dependent_load_index = load_index_1;
					}
					else{
						dependent_load_index = load_index_2;
					}
				}
			}
		}
	}

	// memory dependencies
	if(is_load){
		uint64_t key = inserted->get_address();
		if(trace_store_buffer.find(key) != trace_store_buffer.end()){
			// check if update would be bigger, else it's not necessary
			int index = trace_store_buffer[key];
			inserted->add_producer(&trace_buffer[index]);
			dependence_position[trace_uopcount].push_back(index);

			int possible_new_max_path = trace_buffer[index].get_path_length() + 1;
			if(possible_new_max_path > max_path){
				max_path = possible_new_max_path;
			}

			// we don't have to check if this uop depends directly on another load (unlike in the other dependence cases) since we know it depends on a store in this if-statement
			// hence, immediately check if there's another load in the complete dependence chain
			int load_index = trace_buffer[index].get_producing_load_position();
			if(load_index >= 0){
				if(dependent_load_index >= 0){
					if(trace_buffer[load_index].get_no_producing_loads() >= trace_buffer[dependent_load_index].get_no_producing_loads()){
						dependent_load_index = load_index;
					}
				}
				else{
					dependent_load_index = load_index;
				}
			}
		}
	}

	// register dependencies
	for(unsigned int i = 0; i < uop.getSourceRegistersLength(); i++){
		xed_reg_enum_t sourceRegister = uop.getSourceRegister(i);
		// there is a producer for this uop
		if(trace_producers[sourceRegister] != 0){
			// add all registers as producer (might be necessary for updates)
			int index = trace_producers[sourceRegister];
			inserted->add_producer(&trace_buffer[index]);
			dependence_position[trace_uopcount].push_back(index);

			// check for the largest crit path given all the producers for this uop
			int possible_new_max_path = trace_buffer[index].get_path_length() + 1;
			if(possible_new_max_path > max_path){
				max_path = possible_new_max_path;
			}

			if(is_load){
				// check if directly dependent on a load
				if(trace_buffer[index].is_load()){
					if(dependent_load_index >= 0){
						if(trace_buffer[index].get_no_producing_loads() >= trace_buffer[dependent_load_index].get_no_producing_loads()){
							dependent_load_index = index;
						}
					}
					else{
						dependent_load_index = index;
					}
				}
				else{
					int load_index = trace_buffer[index].get_producing_load_position();
					if(load_index >= 0){
						if(dependent_load_index >= 0){
							if(trace_buffer[load_index].get_no_producing_loads() >= trace_buffer[dependent_load_index].get_no_producing_loads()){
								dependent_load_index = load_index;
							}
						}
						else{
							dependent_load_index = load_index;
						}
					}
				}
			}
		}
	}

	// set critical path of current uop
	if(max_path != 1){
		inserted->set_path_length(max_path);
	}

	if(is_load && dependent_load_index){
		trace_dependent_loads[trace_uopcount] = dependent_load_index;
	}
	else if(is_store){
		trace_store_buffer[inserted->get_address()] = trace_uopcount;
	}

	// update producers of registers
	for(unsigned int i = 0; i < uop.getDestinationRegistersLength(); i++){
		xed_reg_enum_t destinationRegister = uop.getDestinationRegister(i);
		trace_producers[destinationRegister] = trace_uopcount;
	}
}

void utrace_processor::map_to_vectors(map<extended_uop_type, uint32_t> uop_hist, vector<uint32_t> &uop_ids, vector<uint32_t> &uop_freqs){
	for(map<extended_uop_type, uint32_t>::iterator it = uop_hist.begin(); it != uop_hist.end(); ++it){
		uop_ids.push_back((uint32_t) it->first);
		uop_freqs.push_back(it->second);
	}
}
