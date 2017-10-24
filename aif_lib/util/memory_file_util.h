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

#ifndef MEMORY_FILE_UTIL_H
#define MEMORY_FILE_UTIL_H

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include <cmath>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "memory_file.pb.h"

using std::string;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::setbase;
using std::ios;
using std::ios_base;

using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;

using memory_file::Access_Type;
using memory_file::Sample;
using memory_file::Memory_Access;
using memory_file::Burst;
using memory_file::Burst_Profile;

Memory_Access* make_memory_access(uint64_t burst_id, uint64_t addr, uint64_t access_counter, Access_Type access_type, uint32_t thread_id, uint64_t program_counter);

Sample* make_sample(Memory_Access *begin, Memory_Access *end);
Sample* make_dangling_sample(Memory_Access *begin);

void print_sample(Sample *sample);
void print_memory_access(const Memory_Access& access);

void modify_burst(uint64_t burst_id, uint32_t dangling_loads, uint32_t dangling_stores, Burst_Profile *bp);
Burst* add_burst(Burst_Profile *bp, uint64_t burst_counter, uint64_t inscount, uint64_t memcount);
void complete_burst(Burst *b, uint64_t inscount, uint64_t memcount, uint32_t takeoff_loads, uint32_t landing_loads, uint32_t takeoff_stores, uint32_t landing_stores);
void write_burst_profile(string filename, Burst_Profile *bp);
Burst_Profile* read_burst_profile(string filename);

void print_burst_profile(Burst_Profile *bp);

#endif /*MEMORY_FILE_UTIL_H*/
