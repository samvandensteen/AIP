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

#ifndef UTRACE_PROCESSOR_H_
#define UTRACE_PROCESSOR_H_

#include <vector>
#include <unordered_map>
#include <map>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "pin.H"

#include "define_includes.h"
#include "mlp_processor.h"
#include "dependence_processor.h"
#include "knobs.h"
#include "extended_micro_op.h"
#include "utrace_file_util.h"
#include "proto_writer.h"

using std::vector;
using std::map;
using std::unordered_map;
using std::ofstream;

using google::protobuf::io::OstreamOutputStream;

class utrace_processor{
	public:
		static unordered_map<uint64_t, const vector<const MicroOp*>* > instructions;

	private:
		// all instrumented instructions
		void add_dependencies(bool is_load, bool is_store);
		void map_to_vectors(map<extended_uop_type, uint32_t> uop_hist, vector<uint32_t> &uop_ids, vector<uint32_t> &uop_freqs);

		// keep track of some trace variables
		uint32_t trace_counter, trace_inscount, trace_uopcount, trace_load_count, trace_store_count;
		uint32_t ins_trace_length, mem_trace_length;
		// trace ROB
		vector<Extended_Micro_Op> trace_buffer;
		// trace histogram to see which micro-op types are in the trace
		map<extended_uop_type, uint32_t> trace_hist_uops;
		// per uop, save it's position in a dependency chain
		vector<vector<int>> dependence_position;
		// save loads that depend on each other
		vector<int> trace_dependent_loads;
		// save if this instruction is a branch to a branch
		vector<extended_uop_type> trace_uop_type;
		// variables to save current load and store addresses
		uint64_t load_addr_1, load_addr_2, store_addr;
		// save stores to find load-store dependences
		unordered_map<uint64_t, int> trace_store_buffer;
		// save producers for register values to track register to register dependencies (RAW)
		uint64_t trace_producers[XED_REG_LAST];
		// flag to check if trace ended
		bool trace_ended;

		Proto_Writer *pw;

		Dependence_Processor *dependence_processor;
		MLP_Processor *mlp_proc;

	public:
		utrace_processor(Knobs *k);
		~utrace_processor();

		void init_new_trace();
		void end_trace();

		// save all instructions we see in detailed_trace mode when we instrument them the first time
		static void add_instr(uint64_t insAddr, const vector<const MicroOp*>* ins_uops);
		// for each instruction we see in a trace, call this function
		bool process_instr(uint64_t insAddr);

		void data_load(uint64_t load_addr, int op);
		void data_store(uint64_t store_addr);
};

#endif /*UTRACE_PROCESSOR_H_*/
