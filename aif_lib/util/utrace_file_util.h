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

#ifndef UTRACE_FILE_UTIL_H
#define UTRACE_FILE_UTIL_H

#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <ios>
#include <assert.h>
#include <string>
#include <vector>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "utrace_file.pb.h"

using std::ofstream;
using std::ifstream;
using std::ios;
using std::string;
using std::cout;
using std::endl;
using std::vector;

using google::protobuf::io::OstreamOutputStream;

using utrace_file::uTrace;
using utrace_file::ID_string;
using utrace_file::Stats;
using utrace_file::Dependences;
using utrace_file::Histogram;

ID_string* make_id_string(vector<string> uop_string_id);
uTrace* make_utrace(uint64_t utrace_id);
void add_utrace_stats(uTrace* ut, uint32_t trace_inscount, uint32_t trace_uopcount, uint32_t trace_load_count, uint32_t trace_store_count);
void add_utrace_dependences(uTrace* ut, vector<uint32_t> ROB, vector<double> AP, vector<double> BP, vector<double> CP);
void add_utrace_uop_hist(uTrace* ut, vector<uint32_t> uop_id, vector<uint32_t> uop_freq);

void print_utrace(uTrace* ut, vector<string> id_to_string);
void print_utrace_stats(const Stats &s);
void print_utrace_dependences(const Dependences &d);
void print_utrace_uop_hist(const Histogram &uh, vector<string> id_to_string);

#endif /*UTRACE_FILE_UTIL_H*/
