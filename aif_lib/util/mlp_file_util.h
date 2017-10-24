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

#ifndef MLP_FILE_UTIL_H
#define MLP_FILE_UTIL_H

#include <fstream>
#include <string>
#include <sstream>
#include <cstdint>
#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <iostream>
#include <iomanip>
#include <ios>
#include <assert.h>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "mlp_file.pb.h"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::ios;
using std::ios_base;
using std::deque;
using std::vector;
using std::unordered_map;
using std::map;

using mlp_file::MLP_Trace;
using mlp_file::Load_chain;
using mlp_file::PC_reuse;
using mlp_file::PC_stride;

void make_load_chain(Load_chain *lc, uint32_t rob, deque<uint32_t> load_chain_distr);
void make_pc_reuse(PC_reuse* pr, uint32_t pc_first, unordered_map<uint32_t, uint32_t>& pc_RD);
void make_pc_stride(PC_stride *ps, unordered_map<uint64_t, uint32_t>& pc_stride, uint64_t first_address);

MLP_Trace* make_mlp_trace(uint32_t trace_id);
void add_load_chains(MLP_Trace* mlp, map<uint32_t, deque<uint32_t>>& load_chains);
void add_pc_reuses(MLP_Trace* mlp, unordered_map<uint64_t, unordered_map<uint32_t, uint32_t>>& pcs_RD, unordered_map<uint64_t, uint32_t>& pc_first);
void add_pc_strides(MLP_Trace* mlp, unordered_map<uint64_t, unordered_map<uint64_t, uint32_t>>& pcs_stride, unordered_map<uint64_t, uint64_t>& pcs_first_address);

void print_mlp_trace(MLP_Trace *mlp);

#endif /*MLP_FILE_UTIL_H*/
