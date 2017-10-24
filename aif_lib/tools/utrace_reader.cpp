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
#include <vector>

#include "utrace_file.pb.h"
#include "utrace_file_util.h"
#include "proto_reader.h"

using std::vector;
using std::endl;
using std::cout;
using std::ifstream;
using std::ios;

using utrace_file::uTrace;

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Usage: ./utrace_reader <path to utrace mlp file> <compression used (0 or 1)>" << endl;
		exit(1);
	}

	bool compression_used;
	if(atoi(argv[2]) == 0){
		compression_used = false;
	}
	else{
		compression_used = true;
	}

	Proto_Reader input_file(argv[1], compression_used);

	ID_string *ids = new ID_string();
  input_file.read_streaming(ids);
  vector<string> read_uop_string_id;
  for(int i = 0; i < ids->uop_id_to_string_size(); i++){
    read_uop_string_id.push_back(ids->uop_id_to_string(i));
  }

	uTrace *ut = new uTrace();
	while(input_file.read_streaming(ut)){
		print_utrace(ut, read_uop_string_id);
    delete ut;
		ut = new uTrace();
	}
}
