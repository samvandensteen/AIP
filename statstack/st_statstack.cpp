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

#include "st_statstack.h"

ST_statstack::ST_statstack(Knobs* k){
	sampler_i = new sampler("instr", k, 0, true, false);
	sampler_d = new sampler("data", k, 0, true, false);
	sampler_trace = new sampler("trace", k, 0, false, true);

	compress = k->enable_compression;
	cold_writer = new Proto_Writer("cold_misses", compress);

	init_cold_miss_structures(k->min_rob, k->step_rob, k->max_rob);

	data_access_counter = 0;
	instr_access_counter = 0;
}

ST_statstack::~ST_statstack(){
	delete sampler_i;
	delete sampler_d;
	delete sampler_trace;

	delete cold_writer;
}

int ST_statstack::trace_ins(uint64_t ip, uint64_t addr, uint16_t tid, uint64_t inscount){
	sampler_i->sample_memory_access(addr, instr_access_counter, Access_Type::LOAD, tid, ip, inscount);
	++instr_access_counter;
	return 0;
}

int ST_statstack::trace_mem_detailed(uint64_t ip, uint64_t addr, uint16_t tid, uint32_t ref_type, uint64_t inscount, instrument_mode_t mode){
	if(mode == DETAILED_TRACE){
		sampler_trace->in_trace();
	}

	if(ref_type == 0){
		profile_cold_loads_detailed(addr, inscount);
		sampler_d->sample_memory_access(addr, data_access_counter, Access_Type::LOAD, tid, ip, inscount);
		sampler_trace->sample_memory_access(addr, data_access_counter, Access_Type::LOAD, tid, ip, inscount);
	}
	else if(ref_type == 1){
		profile_cold_stores_detailed(addr);
		sampler_d->sample_memory_access(addr, data_access_counter, Access_Type::STORE, tid, ip, inscount);
		sampler_trace->sample_memory_access(addr, data_access_counter, Access_Type::STORE, tid, ip, inscount);
	}

	++data_access_counter;

	return 0;
}

int ST_statstack::trace_mem_warmup(uint64_t ip, uint64_t addr, uint16_t tid, uint32_t ref_type, uint64_t inscount){
	if(ref_type == 0){
		profile_cold_loads_warmup(addr);
		sampler_d->sample_memory_access(addr, data_access_counter, Access_Type::LOAD, tid, ip, inscount);
	}
	else if(ref_type == 1){
		profile_cold_stores_warmup(addr);
		sampler_d->sample_memory_access(addr, data_access_counter, Access_Type::STORE, tid, ip, inscount);
	}

	++data_access_counter;

	return 0;
}

void ST_statstack::init_cold_miss_structures(int min_rob, int step_rob, int max_rob){
	no_robs = (max_rob - min_rob) / step_rob + 1;

	for(uint32_t i = 0; i < no_robs; i++){
		robs.push_back(min_rob + i * step_rob);
	}

	for(uint32_t i = 0; i < no_robs; i++){
		cold_misses.push_back(0);
		next_cold_miss_rob.push_back(robs[i]);
		cold_miss_distribution.push_back(vector<uint64_t>(robs[i], 0));
	}
}

void ST_statstack::profile_cold_loads_warmup(uint64_t mem_load){
	unordered_map<uint64_t, char>::iterator uls = unique_loads_stores.find(mem_load >> (int) log2(cache_block_size));
	if(uls == unique_loads_stores.end()){
		unique_loads_stores[mem_load >> (int) log2(cache_block_size)] = '0';
	}
}

void ST_statstack::profile_cold_loads_detailed(uint64_t mem_load, uint64_t inscount){
	unordered_map<uint64_t, char>::iterator uls = unique_loads_stores.find(mem_load >> (int) log2(cache_block_size));
	if(uls == unique_loads_stores.end()){
		unique_loads_stores[mem_load >> (int) log2(cache_block_size)] = '1';
		for(uint32_t i = 0; i < no_robs; i++){
			assert(cold_misses[i] <= robs[i]);
			// approximate uopcount
			if(inscount * UOPS_PER_INSTR > next_cold_miss_rob[i]){
				cold_miss_distribution[i][cold_misses[i]]++;

				cold_misses[i] = 0;
				next_cold_miss_rob[i] = (inscount * UOPS_PER_INSTR + robs[i] - 1) / robs[i] * robs[i];
			}
			cold_misses[i]++;
		}
	}
}

void ST_statstack::profile_cold_stores_warmup(uint64_t mem_store){
	unordered_map<uint64_t, char>::iterator uls = unique_loads_stores.find(mem_store >> (int) log2(cache_block_size));
	if(uls == unique_loads_stores.end()){
		unique_loads_stores[mem_store >> (int) log2(cache_block_size)] =  '2';
	}
}

void ST_statstack::profile_cold_stores_detailed(uint64_t mem_store){
	unordered_map<uint64_t, char>::iterator uls = unique_loads_stores.find(mem_store >> (int) log2(cache_block_size));
	if(uls == unique_loads_stores.end()){
		unique_loads_stores[mem_store >> (int) log2(cache_block_size)] = '3';
	}
}

void ST_statstack::save_cold_miss_distribution(uint64_t window_count, uint64_t inscount){
	Cold_Window *cw = make_cold_window(window_count);
  add_ROB_cold_distribution(cw, cache_block_size, robs, cold_miss_distribution);
	cold_writer->write_streaming(cw, compress);
	delete cw;

	reset_cold_miss_distribution_variables(inscount);
}

void ST_statstack::reset_cold_miss_distribution_variables(uint64_t inscount){
	cold_miss_distribution.clear();

	for(uint32_t i = 0; i < no_robs; i++){
		cold_misses[i] = 0;
		cold_miss_distribution.push_back(vector<uint64_t>(robs[i], 0));
		next_cold_miss_rob[i] = (inscount * UOPS_PER_INSTR + robs[i] - 1) / robs[i] * robs[i];
	}
}
