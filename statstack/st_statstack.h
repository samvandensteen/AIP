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

#ifndef ST_SAMPLER_H
#define ST_SAMPLER_H

#include <cstdlib>
#include <iostream>
#include <unordered_map>

#include "globals.h"
#include "sampler.h"
#include "proto_writer.h"
#include "cold_file.pb.h"
#include "cold_file_util.h"

using std::unordered_map;

using memory_file::Access_Type;
using cold_file::Cold_Window;

class ST_statstack{
	private:
		sampler *sampler_i;
		sampler *sampler_d;
		sampler *sampler_trace;

		bool compress;
		Proto_Writer *cold_writer;

		uint64_t instr_access_counter, data_access_counter;

		// cold miss variables
		string rob_string;
		uint32_t cache_block_size = 64;
		unordered_map<uint64_t, char> unique_loads_stores;
		uint32_t no_robs;
		vector<uint32_t> robs;
		vector<uint32_t> cold_misses;
		vector<uint64_t> next_cold_miss_rob;
		vector<vector<uint64_t>> cold_miss_distribution;

		// cold miss functions
		void init_cold_miss_structures(int min_rob, int step_rob, int max_rob);
		void profile_cold_loads_warmup(uint64_t mem_load);
		void profile_cold_loads_detailed(uint64_t mem_load, uint64_t inscount);
		void profile_cold_stores_warmup(uint64_t mem_store);
		void profile_cold_stores_detailed(uint64_t mem_store);

		void reset_cold_miss_distribution_variables(uint64_t inscount);

	public:
		ST_statstack(Knobs* k);
		~ST_statstack();

		int trace_ins(uint64_t ip, uint64_t addr, uint16_t tid, uint64_t inscount);
		int trace_mem_detailed(uint64_t ip, uint64_t addr, uint16_t tid, uint32_t ref_type, uint64_t inscount, instrument_mode_t mode);
		int trace_mem_warmup(uint64_t ip, uint64_t addr, uint16_t tid, uint32_t ref_type, uint64_t inscount);
		
		void save_cold_miss_distribution(uint64_t window_count, uint64_t inscount);
};

#endif
