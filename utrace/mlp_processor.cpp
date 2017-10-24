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

#include "mlp_processor.h"

MLP_Processor::MLP_Processor(bool enable_compression){
	trace_size = 0;
	loads_in_trace = 0;

	pw = new Proto_Writer("mlp", enable_compression);
}

MLP_Processor::~MLP_Processor(){
	delete pw;
}

void MLP_Processor::process_load_trace(vector<int> trace_loads, int trace_load_count, int trace_count){
	trace_size = trace_loads.size();
	loads_in_trace = trace_load_count;
	this->trace_loads = trace_loads;

	calculate_ROB_sizes();
	calculate_load_distr();
	// no dependent loads: this is a corner case where the map is empty, the the analyzer would fail to distinguish between no loads seen and no dependent loads seen
	if(trace_load_count > 0){
		load_chain_distr[1] = deque<uint32_t>(1, trace_load_count);
	}

	// tp->save_mlp_stats(trace_count, loads_in_trace, load_chain_distr, pc_ref_RD_hist, pc_ref_utrace_first, pc_mem_stride_hist);
	MLP_Trace* mlp = make_mlp_trace(trace_count);
  add_load_chains(mlp, load_chain_distr);
  add_pc_reuses(mlp, pc_ref_RD_hist, pc_ref_utrace_first);
  add_pc_strides(mlp, pc_mem_stride_hist, pc_mem_utrace_first);
	pw->write_streaming(mlp, true);
	delete mlp;

	clear_data_structures();
}

void MLP_Processor::calculate_ROB_sizes(){
	rob_sizes = vector<vector<int> >(trace_size, vector<int>(1, 1));
	for(int i = 0; i < trace_size; i++){
		if(trace_loads[i] >= 0){
			int uop_spacing = i - trace_loads[i], dependent_on = trace_loads[i];
			for(vector<int>::iterator it = rob_sizes[dependent_on].begin(); it != rob_sizes[dependent_on].end(); ++it){
				rob_sizes[i].push_back(uop_spacing + *it);
			}
		}
	}
}

void MLP_Processor::calculate_load_distr(){
	vector<int> all_values;
	map<int, vector<int> > load_dep_chains;
	uint64_t longest_dependecy_value = 0;
	for(int i = 0; i < trace_size; i++){
		uint64_t load_dependency_chain = rob_sizes[i].size();
		if(load_dependency_chain > longest_dependecy_value){
			longest_dependecy_value = load_dependency_chain;
		}
		// ignore independent loads (loads with rob_sizes value 1)
		for(uint64_t j = 1; j < load_dependency_chain; j++){
			map<int, vector<int>>::iterator it = load_dep_chains.find(j);
			if(it != load_dep_chains.end()){
				load_dep_chains[j].push_back(rob_sizes[i][j]);
				all_values.push_back(rob_sizes[i][j]);
			}
			else{
				load_dep_chains[j] = vector<int>(1, rob_sizes[i][j]);
				all_values.push_back(rob_sizes[i][j]);
			}
		}
	}

	// sort different map vectors
	for(map<int, vector<int> >::iterator it = load_dep_chains.begin(); it != load_dep_chains.end(); ++it){
		sort(it->second.begin(), it->second.end());
	}

	// all values to unique values
	sort(all_values.begin(), all_values.end());
	all_values.erase(unique(all_values.begin(), all_values.end()), all_values.end());

	for(uint64_t i = 0; i < all_values.size(); i++){
		load_chain_distr[all_values[i]] = deque<uint32_t>(0, 0);
	}

	for(vector<int>::reverse_iterator rit_1 = all_values.rbegin(); rit_1 != all_values.rend(); ++rit_1){
		int loads_smaller = 0, previous_loads_smaller = 0;
		for(uint64_t i = longest_dependecy_value - 1; i > 0; i--){
			// increment iterator in loop, only when element is smaller, else erase the value (iterator will then automatically point to next element)
			vector<int>::reverse_iterator rit_2 = load_dep_chains[i].rbegin();
			while(rit_2 != load_dep_chains[i].rend()){
				if(*rit_2 > *rit_1){
					// for some bizarre reason reverse iterators do no properly support erase, hence apply some tricks
					// sources
					// http://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
					// http://stackoverflow.com/questions/1830158/how-to-call-erase-with-a-reverse-iterator
					// increase iterator by 1 and take base (kind of transforming it to a normal iterator) before erasing
					// normal iterator now points to previous element, make a reverse iterator that points to the previous element which can be erased
					vector<int>::iterator temp_it = load_dep_chains[i].erase(next(rit_2).base());
					rit_2 = vector<int>::reverse_iterator(temp_it);
					// all elements were deleted, decrement longest_dependecy_value to not loop over it anymore
					if(load_dep_chains[i].size() == 0){
						longest_dependecy_value--;
						break;
					}
				}
				else{
					break;
				}
			}
			loads_smaller = load_dep_chains[i].size();
			// load_chain_distr[*rit_1][i] = (double)(loads_smaller - previous_loads_smaller) / loads_in_trace;
			uint32_t insert_loads = loads_smaller - previous_loads_smaller;
			if(insert_loads > 0){
				load_chain_distr[*rit_1].push_front(insert_loads);
			}
			previous_loads_smaller = loads_smaller;
		}
		// load_chain_distr[*rit_1][0] = (double)(loads_in_trace - previous_loads_smaller) / loads_in_trace;
		load_chain_distr[*rit_1].push_front(loads_in_trace - previous_loads_smaller);
	}
}

void MLP_Processor::log_stride_and_reuse_PC(uint32_t trace_uopcount, uint64_t ip, uint64_t addr){
	unordered_map<uint64_t, uint32_t>::iterator find_1 = pc_ref_utrace.find(ip);
	unordered_map<uint64_t, unordered_map<uint32_t, uint32_t> >::iterator find_2 = pc_ref_RD_hist.find(ip);
	unordered_map<uint64_t, uint64_t>::iterator find_3 = pc_mem_utrace.find(ip);
	unordered_map<uint64_t, unordered_map<uint64_t, uint32_t> >::iterator find_4 = pc_mem_stride_hist.find(ip);

	if(find_1 != pc_ref_utrace.end()){
		// rd of PC's
		uint32_t rd = trace_uopcount - find_1->second;
		unordered_map<uint32_t, uint32_t>::iterator find_5 = find_2->second.find(rd);
		if(find_5 != find_2->second.end()){
			find_5->second++;
		}
		else{
			find_2->second[rd] = 1;
		}
		find_1->second = trace_uopcount;

		// stride
		uint64_t stride = UNSIGNED_SUB(addr, find_3->second);
		unordered_map<uint64_t, uint32_t>::iterator find_6 = find_4->second.find(stride);
		if(find_6 != find_4->second.end()){
			find_6->second++;
		}
		else{
			find_4->second[stride] = 1;
		}
		find_3->second = addr;
	}
	else{
		pc_ref_utrace[ip] = trace_uopcount;
		pc_ref_utrace_first[ip] = trace_uopcount;
		pc_ref_RD_hist[ip].reserve(10);
		pc_mem_utrace[ip] = addr;
		pc_mem_utrace_first[ip] = addr;
		pc_mem_stride_hist[ip].reserve(10);
	}
}

void MLP_Processor::clear_data_structures(){
	trace_loads.clear();
	rob_sizes.clear();

	load_chain_distr.clear();

	// clear pc ref datastructures
	pc_ref_utrace.clear();
	pc_ref_utrace_first.clear();
	pc_ref_RD_hist.clear();

	// clear pc stride datastructures
	pc_mem_utrace.clear();
	pc_mem_stride_hist.clear();
}
