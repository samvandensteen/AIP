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

#ifndef BRANCH_FILE_UTIL_H
#define BRANCH_FILE_UTIL_H

#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <map>
#include <iostream>
#include <iomanip>
#include <ios>
#include <assert.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "branch_file.pb.h"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::map;

using branch_file::IP_Bits;
using branch_file::Entropy_Window;

void make_ip_bits(map<uint32_t, double> bhr_entropy);

Entropy_Window* make_entropy_window(uint32_t ew_id, string t, uint32_t b);
void add_ip_bits(Entropy_Window* ew, map<uint32_t, map<uint32_t, double>> ips);

void print_entropy_window(Entropy_Window *ew);

#endif /*BRANCH_FILE_UTIL_H*/
