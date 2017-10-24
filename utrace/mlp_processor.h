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

#ifndef MLP_PROCESSOR_H_
#define MLP_PROCESSOR_H_

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <algorithm>

#include "define_includes.h"
#include "proto_writer.h"
#include "mlp_file_util.h"

using std::string;
using std::map;
using std::deque;
using std::vector;
using std::unordered_map;

class MLP_Processor{
	private:
		int trace_size, loads_in_trace;
		vector<int> trace_loads;
		vector<vector<int> > rob_sizes;

		map<uint32_t, deque<uint32_t> > load_chain_distr;

		// reuse distance in uops of PC's
		unordered_map<uint64_t, uint32_t> pc_ref_utrace;
		unordered_map<uint64_t, uint32_t> pc_ref_utrace_first;
		unordered_map<uint64_t, unordered_map<uint32_t, uint32_t> > pc_ref_RD_hist;
		// strides between mem references per PC
		unordered_map<uint64_t, uint64_t> pc_mem_utrace;
		unordered_map<uint64_t, uint64_t> pc_mem_utrace_first;
		unordered_map<uint64_t, unordered_map<uint64_t, uint32_t> > pc_mem_stride_hist;

		Proto_Writer *pw;

		int count_non_zero_elements(vector<int> trace_loads);
		void calculate_ROB_sizes();
		void calculate_load_distr();
		void clear_data_structures();

	public:
		MLP_Processor(bool enable_compression);
		~MLP_Processor();

		void process_load_trace(vector<int> trace_loads, int trace_load_count, int trace_count);

		void log_stride_and_reuse_PC(uint32_t trace_uopcount, uint64_t ip, uint64_t addr);
};

#endif
