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

#ifndef COLD_FILE_UTIL_H
#define COLD_FILE_UTIL_H

#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <ios>
#include <assert.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "cold_file.pb.h"

using std::string;
using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::vector;

using cold_file::ROB_cold_distribution;
using cold_file::Cold_Window;

void make_ROB_cold_distribution(ROB_cold_distribution *rcd, uint32_t cacheblock, uint32_t rob, vector<uint64_t> occurences);

Cold_Window* make_cold_window(uint32_t window_id);
void add_ROB_cold_distribution(Cold_Window* cw, uint32_t cacheblock, vector<uint32_t> robs, vector<vector<uint64_t>> occurences);

void print_cold_window(Cold_Window *cw);

#endif /*COLD_FILE_UTIL_H*/
