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
#include <map>
#include <vector>
#include <unordered_map>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "mlp_file.pb.h"
#include "mlp_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::map;
using std::unordered_map;
using std::deque;
using std::vector;
using std::chrono::system_clock;
using std::chrono::duration;

using mlp_file::Load_chain;
using mlp_file::PC_reuse;
using mlp_file::PC_stride;
using mlp_file::MLP_Trace;

#define ENABLE_COMPRESSION true

MLP_Trace* generate_random_trace(uint32_t trace){
	MLP_Trace* mlp = make_mlp_trace(trace);

	// generate data
	map<uint32_t, deque<uint32_t>> load_chains;
	for(int i = 1; i < 257; i += 16){
	  deque<uint32_t> chain;
	  for(int j = 0; j < (int) sqrt(i); j++){
	    chain.push_back((int) sqrt(i) - j);
	  }
	  load_chains[i] = chain;
	}
	//add data
	add_load_chains(mlp, load_chains);

	// generate data
	unordered_map<uint64_t, unordered_map<uint32_t, uint32_t>> pcs_RD;
	unordered_map<uint64_t, uint32_t> pc_first;
	for(uint64_t i = 1; i < 4228486; i += 422848){
	  pc_first[i] = rand() % 1024 + 1;
	unordered_map<uint32_t, uint32_t> rd_hist;
	  rd_hist[8] = 1;
	  rd_hist[16] = 2;
	  rd_hist[32] = 3;
	  pcs_RD[i] = rd_hist;
	}
	//add data
	add_pc_reuses(mlp, pcs_RD, pc_first);

	// generate data
	unordered_map<uint64_t, unordered_map<uint64_t, uint32_t>> pcs_stride;
	unordered_map<uint64_t, uint64_t> pcs_first_address;
	for(uint64_t i = 1; i < 4228486; i += 422848){
	  unordered_map<uint64_t, uint32_t> stride_hist;
	  stride_hist[4] = 4;
	  stride_hist[8] = 5;
	  stride_hist[12] = 6;
	  pcs_stride[i] = stride_hist;
		pcs_first_address[i] = 2 * i;
	}
	//add data
	add_pc_strides(mlp, pcs_stride, pcs_first_address);

	return mlp;
}

int main(void){
	const unsigned no_traces = 1000000;
	cout << "This benchmark will write then read " << no_traces << " mlp traces" << endl;

	const string filename = "benchmark_mlp";
	Proto_Writer output_file(filename, ENABLE_COMPRESSION);
	duration<double> elapsed_seconds_gen, elapsed_seconds_write;
	for(uint32_t i = 0; i < no_traces; i++){
		auto start = system_clock::now();
		MLP_Trace *trace = generate_random_trace(i);
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
	MLP_Trace *mlp = new MLP_Trace();
	while(input_file.read_streaming(mlp)){
		continue;
	}
	auto end = system_clock::now();
	duration<double> elapsed_seconds_read = end - start;

	cout << "Generation time: " << elapsed_seconds_gen.count() << 's' << endl;
	cout << "Write time: " << elapsed_seconds_write.count() << 's' << endl;
	cout << "Read time: " << elapsed_seconds_read.count() << 's' << endl;
}
