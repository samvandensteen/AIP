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
#include <cmath>

#include "memory_file.pb.h"
#include "memory_file_util.h"

using std::endl;
using std::cout;
using std::ifstream;
using std::ios;

using memory_file::Burst_Profile;

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "Usage: ./burst_reader <path to burst file>" << endl;
		exit(1);
	}

	Burst_Profile* bp = read_burst_profile(argv[1]);
	print_burst_profile(bp);
}
