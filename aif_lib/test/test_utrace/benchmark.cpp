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

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <vector>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "utrace_file.pb.h"
#include "utrace_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::chrono::system_clock;
using std::chrono::duration;

#define ENABLE_COMPRESSION true

uTrace* generate_random_trace(uint32_t trace_id){
  srand(time(NULL));

  uTrace *ut = make_utrace(trace_id);

	uint32_t instr = rand() % 1024;
	add_utrace_stats(ut, instr, (uint32_t) instr * 1.2, (uint32_t) instr * 0.3, (uint32_t) instr * 0.2);

	vector<uint32_t> ROB{1};
	vector<double> AP, BP, CP;
	double start_AP = (double) (rand() % 10) / 10 + 1.0;
	double start_BP = 1.5 * start_AP;
	double start_CP = 2 * start_AP;
	for(int j = 16; j <= 256; j += 16){
		ROB.push_back(j);
		AP.push_back(start_AP + j / 16);
		BP.push_back(start_BP + j / 8);
		CP.push_back(start_CP + j / 4);
	}
	add_utrace_dependences(ut, ROB, AP, BP, CP);

	uint32_t no_ops = rand() % 19;
	vector<uint32_t> op_id, op_freq;
	for(uint32_t j = 0; j < no_ops; j++){
		op_id.push_back(j);
		op_freq.push_back(rand() % 250);
	}
	add_utrace_uop_hist(ut, op_id, op_freq);

  return ut;
}

int main(void){
	const unsigned no_traces = 1000000;
	cout << "This benchmark will write and then read " << no_traces << " traces from the utrace file!" << endl;

  vector<string> uop_string_id{"COND_BRANCH", "UNCOND_BRANCH", "CHNGREG", "I_ADD", "I_SUB", "I_MUL", "I_DIV", "FP_ADD", "FP_SUB", "FP_MUL", "FP_DIV", "FP_SQRT", "GENERIC_1", "GENERIC_2", "GENERIC_3", "GENERIC_5", "LOAD", "OTHER", "STORE"};
  ID_string* ids = make_id_string(uop_string_id);

	const string filename = "benchmark_utrace";
	Proto_Writer output_file(filename, ENABLE_COMPRESSION);
  output_file.write_streaming(ids, true);
  delete ids;
	duration<double> elapsed_seconds_gen, elapsed_seconds_write;
	for(uint32_t i = 0; i < no_traces; i++){
		auto start = system_clock::now();
		uTrace *trace = generate_random_trace(i);
		auto end = system_clock::now();
		elapsed_seconds_gen += end - start;

		start = system_clock::now();
		output_file.write_streaming(trace, true);
		end = system_clock::now();
		elapsed_seconds_write += end - start;

		delete trace;
	}
	output_file.close_file();

	auto start = system_clock::now();
	Proto_Reader input_file(filename + ".0", ENABLE_COMPRESSION);
  ids = new ID_string();
  input_file.read_streaming(ids);
	uTrace *trace = new uTrace();
	while(input_file.read_streaming(trace)){
		continue;
	}
	auto end = system_clock::now();
	duration<double> elapsed_seconds_read = end - start;

	cout << "Generation time: " << elapsed_seconds_gen.count() << 's' << endl;
	cout << "Write time: " << elapsed_seconds_write.count() << 's' << endl;
	cout << "Read time: " << elapsed_seconds_read.count() << 's' << endl;
}
