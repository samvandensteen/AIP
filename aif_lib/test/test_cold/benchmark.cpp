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
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "cold_file.pb.h"
#include "cold_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::chrono::system_clock;
using std::chrono::duration;

using cold_file::Cold_Window;

#define ENABLE_COMPRESSION true

Cold_Window* generate_random_window(uint32_t window){
	Cold_Window* cw = make_cold_window(window);

	// generate data
  vector<uint32_t> robs;
  vector<vector<uint64_t>> cold_miss_occurences;
  uint32_t counter = 0;
  for(int i = 1; i < 257; i += 16){
    robs.push_back(i);
    cold_miss_occurences.push_back(vector<uint64_t>(i, 0));
    for(int j = 0; j < (int) sqrt(i); j++){
      cold_miss_occurences[counter][j] = rand() % 256;
    }
    counter++;
  }
  //add data
  add_ROB_cold_distribution(cw, 32, robs, cold_miss_occurences);

	return cw;
}

int main(void){
	const unsigned no_windows = 1000000;
	cout << "This benchmark will write then read " << no_windows << " cold windows" << endl;

	const string filename = "benchmark_cold";
	Proto_Writer output_file(filename, ENABLE_COMPRESSION);
	duration<double> elapsed_seconds_gen, elapsed_seconds_write;
	for(uint32_t i = 0; i < no_windows; i++){
		auto start = system_clock::now();
		Cold_Window *cw = generate_random_window(i);
		auto end = system_clock::now();
		elapsed_seconds_gen += end - start;

		start = system_clock::now();
		output_file.write_streaming(cw, true);
		end = system_clock::now();
		elapsed_seconds_write += end - start;

		delete cw;
	}
	output_file.close_file();

	auto start = system_clock::now();
	Proto_Reader input_file(filename + ".0", ENABLE_COMPRESSION);
	Cold_Window *cw = new Cold_Window();
	while(input_file.read_streaming(cw)){
		continue;
	}
	auto end = system_clock::now();
	duration<double> elapsed_seconds_read = end - start;

	cout << "Generation time: " << elapsed_seconds_gen.count() << 's' << endl;
	cout << "Write time: " << elapsed_seconds_write.count() << 's' << endl;
	cout << "Read time: " << elapsed_seconds_read.count() << 's' << endl;
}
