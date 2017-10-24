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

#include "branch_file.pb.h"
#include "branch_file_util.h"
#include "proto_reader.h"

using std::endl;
using std::cout;

using branch_file::Entropy_Window;

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Usage: ./branch_reader <path to branch file> <compression used (0 or 1)>" << endl;
		exit(1);
	}

	bool compression_used;
	if(atoi(argv[2]) == 0){
		compression_used = false;
	}
	else{
		compression_used = true;
	}

	Proto_Reader input_branch_file(argv[1], compression_used);

	Entropy_Window *ew = new Entropy_Window();
	while(input_branch_file.read_streaming(ew)){
		print_entropy_window(ew);
    delete ew;
		ew = new Entropy_Window();
	}
}
