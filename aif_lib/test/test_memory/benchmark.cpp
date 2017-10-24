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

#include <vector>
#include <random>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <fstream>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "memory_file.pb.h"
#include "memory_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

#define ENABLE_COMPRESSION true

using std::vector;
using std::chrono::system_clock;
using std::chrono::duration;

using memory_file::Memory_Access;
using memory_file::Access_Type;
using memory_file::Sample;

Sample* generate_sample(uint32_t sample){
	Memory_Access *begin = make_memory_access(0, sample, sample, Access_Type::LOAD, 42, sample);
	Memory_Access *end = make_memory_access(1, begin->memory_address(), begin->access_counter() + 10, Access_Type::STORE, 42, sample);
	Sample* s = make_sample(begin, end);
	return s;
}

Burst_Profile* generate_burst_profile(uint32_t number){
  Burst_Profile* bp_w = new Burst_Profile();
  bp_w->set_line_size(6);

	for(uint32_t i = 0; i < number; i++){
	  Burst* temp = add_burst(bp_w, i, i * 1000, i * 1500);
	  complete_burst(temp, (i + 1) * 1000, (i + 1) * 1500, 100, 90, 50, 40);
	}

	return bp_w;
}

int main(void){
	const unsigned nb_samples = 1000000;
	cout << "This benchmark will write then read " << nb_samples << " samples and bursts!" << endl;

	cout << endl << "Sample file: " << endl;
	const string filename = "benchmark_sample";
	Proto_Writer output_sample_file(filename, ENABLE_COMPRESSION);
	duration<double> elapsed_seconds, elapsed_seconds_gen, elapsed_seconds_write;
	for(uint32_t i = 0; i < nb_samples; i++){
		auto start = system_clock::now();
		Sample* sample = generate_sample(i);
		auto end = system_clock::now();
		elapsed_seconds_gen += end - start;

		start = system_clock::now();
		output_sample_file.write_streaming(sample, true);
		end = system_clock::now();
		elapsed_seconds_write += end - start;

		delete sample;
	}
	output_sample_file.close_file();

	Proto_Reader input_sample_file(filename + ".0", ENABLE_COMPRESSION);
	auto start = system_clock::now();
	Sample *sample = new Sample();
	while(input_sample_file.read_streaming(sample)){
		continue;
	}
	auto end = system_clock::now();
	duration<double> elapsed_seconds_read = end - start;

	cout << "Generation time: " << elapsed_seconds_gen.count() << 's' << endl;
	cout << "Write time: " << elapsed_seconds_write.count() << 's' << endl;
	cout << "Read time: " << elapsed_seconds_read.count() << 's' << endl;

	cout << endl << "Burst file: " << endl;
	start = system_clock::now();
	Burst_Profile *bp_w = generate_burst_profile(nb_samples);
	end = system_clock::now();
	elapsed_seconds = end - start;
	cout << "Generation time: " << elapsed_seconds.count() << 's' << endl;

	start = system_clock::now();
	write_burst_profile("benchmark_burst.0", bp_w);
	end = system_clock::now();
	elapsed_seconds = end - start;
	cout << "Write time: " << elapsed_seconds.count() << 's' << endl;

	start = system_clock::now();
	read_burst_profile("burst_test.0");
	end = system_clock::now();
	elapsed_seconds = end - start;
	cout << "Read time: " << elapsed_seconds.count() << 's' << endl;
}
