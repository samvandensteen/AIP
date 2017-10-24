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
#include <cstdint>
#include <fstream>

#include "memory_file.pb.h"
#include "memory_file_util.h"
#include "proto_reader.h"

using std::endl;
using std::cout;

using memory_file::Sample;

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Usage: ./sample_reader <path to sample file> <compression used (0 or 1)>" << endl;
		exit(1);
	}

	bool compression_used;
	if(atoi(argv[2]) == 0){
		compression_used = false;
	}
	else{
		compression_used = true;
	}

	Proto_Reader input_sample_file(argv[1], compression_used);

	Sample *sample = new Sample();
	while(input_sample_file.read_streaming(sample)){
		print_sample(sample);
		delete sample;
		sample = new Sample();
	}
}
