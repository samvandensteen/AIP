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
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "branch_file_util.h"
#include "branch_file.pb.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::map;
using std::vector;
using std::string;
using std::chrono::system_clock;
using std::chrono::duration;

using branch_file::IP_Bits;
using branch_file::Entropy_Window;

#define ENABLE_COMPRESSION true

Entropy_Window* generate_random(uint32_t count){
  srand(time(NULL));

	Entropy_Window* ew = make_entropy_window(count, "l", 100000);
	map<uint32_t, map<uint32_t, double>> temp_map;
	for(uint32_t i = 0; i < 10; i++){
		temp_map[i] = map<uint32_t, double>();
		for(uint32_t j = 0; j < 10; j++){
			temp_map[i][j] = ((double) (rand() % 100)) / 100;
		}
	}
	add_ip_bits(ew, temp_map);

	return ew;
}

int main(void){
	const uint32_t number = 1000000;
	cout << "This benchmark will write then read " << number << " entropy windows!" << endl;

	duration<double> elapsed_seconds_gen, elapsed_seconds_write;
	const string filename = "benchmark_branch";
	Proto_Writer output_file(filename, ENABLE_COMPRESSION);
	for(uint32_t i = 0; i < number; i++){
		auto start = system_clock::now();
		Entropy_Window *ew = generate_random(i);
		auto end = system_clock::now();
		elapsed_seconds_gen += end - start;

		start = system_clock::now();
		output_file.write_streaming(ew, true);
		end = system_clock::now();
		elapsed_seconds_write += end - start;

		delete ew;
	}
	output_file.close_file();

	auto start = system_clock::now();
	Proto_Reader input_file(filename + ".0", ENABLE_COMPRESSION);
	Entropy_Window *ew = new Entropy_Window();
	while(input_file.read_streaming(ew)){
		continue;
	}
	auto end = system_clock::now();
	duration<double> elapsed_seconds_read = end - start;

	cout << "Generation time: " << elapsed_seconds_gen.count() << 's' << endl;
	cout << "Write time: " << elapsed_seconds_write.count() << 's' << endl;
	cout << "Read time: " << elapsed_seconds_read.count() << 's' << endl;
}
