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
#include <fstream>
#include <map>
#include <deque>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "mlp_file.pb.h"
#include "mlp_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::map;
using std::unordered_map;
using std::deque;

using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;

using mlp_file::Load_chain;
using mlp_file::PC_reuse;
using mlp_file::PC_stride;
using mlp_file::MLP_Trace;

void test_mlp_file(){
  srand(time(NULL));

  MLP_Trace* mlp1 = make_mlp_trace(1);

  // generate data
  map<uint32_t, deque<uint32_t>> load_chains_1;
  for(int i = 1; i < 257; i += 16){
    deque<uint32_t> chain;
    for(int j = 0; j < (int) sqrt(i); j++){
      chain.push_back((int) sqrt(i) - j);
    }
    load_chains_1[i] = chain;
  }
  //add data
  add_load_chains(mlp1, load_chains_1);

  // generate data
  unordered_map<uint64_t, unordered_map<uint32_t, uint32_t>> pcs_RD_1;
  unordered_map<uint64_t, uint32_t> pc_first_1;
  for(uint64_t i = 1; i < 4228486; i += 422848){
    pc_first_1[i] = rand() % 1024 + 1;
    unordered_map<uint32_t, uint32_t> rd_hist;
    rd_hist[8] = 1;
    rd_hist[16] = 2;
    rd_hist[32] = 3;
    pcs_RD_1[i] = rd_hist;
  }
  //add data
  add_pc_reuses(mlp1, pcs_RD_1, pc_first_1);

  // generate data
  unordered_map<uint64_t, unordered_map<uint64_t, uint32_t>> pcs_stride_1;
  unordered_map<uint64_t, uint64_t> pcs_first_address_1;
  for(uint64_t i = 1; i < 4228486; i += 422848){
    unordered_map<uint64_t, uint32_t> stride_hist;
    stride_hist[4] = 4;
    stride_hist[8] = 5;
    stride_hist[12] = 6;
    pcs_stride_1[i] = stride_hist;
    pcs_first_address_1[i] = 2 * i;
  }
  //add data
  add_pc_strides(mlp1, pcs_stride_1, pcs_first_address_1);

  MLP_Trace* mlp2 = make_mlp_trace(2);

  // generate data
  map<uint32_t, deque<uint32_t>> load_chains_2;
  for(int i = 257; i < 513; i += 16){
    deque<uint32_t> chain;
    for(int j = 0; j < (int) sqrt(i); j++){
      chain.push_back((int) sqrt(i) - j);
    }
    load_chains_2[i] = chain;
  }
  //add data
  add_load_chains(mlp2, load_chains_2);

  // generate data
  unordered_map<uint64_t, unordered_map<uint32_t, uint32_t>> pcs_RD_2;
  unordered_map<uint64_t, uint32_t> pc_first_2;
  for(uint64_t i = 1; i < 5228486; i += 522848){
    pc_first_2[i] = rand() % 1024 + 1;
    unordered_map<uint32_t, uint32_t> rd_hist;
    rd_hist[8] = 1;
    rd_hist[16] = 2;
    rd_hist[32] = 3;
    pcs_RD_2[i] = rd_hist;
  }
  //add data
  add_pc_reuses(mlp2, pcs_RD_2, pc_first_2);

  // generate data
  unordered_map<uint64_t, unordered_map<uint64_t, uint32_t>> pcs_stride_2;
  unordered_map<uint64_t, uint64_t> pcs_first_address_2;
  for(uint64_t i = 1; i < 5228486; i += 522848){
    unordered_map<uint64_t, uint32_t> stride_hist;
    stride_hist[4] = 4;
    stride_hist[8] = 5;
    stride_hist[12] = 6;
    pcs_stride_2[i] = stride_hist;
    pcs_first_address_2[i] = 2 * i;
  }
  //add data
  add_pc_strides(mlp2, pcs_stride_2, pcs_first_address_2);

  // write data
  Proto_Writer *mlp_writer = new Proto_Writer("mlp", false);
  mlp_writer->write_streaming(mlp1, true);
  mlp_writer->write_streaming(mlp2, true);
  delete mlp_writer;

  // read data
  Proto_Reader *mlp_reader = new Proto_Reader("mlp.0", false);
  MLP_Trace *mlp = new MLP_Trace();
  while(mlp_reader->read_streaming(mlp)){
    print_mlp_trace(mlp);
    delete mlp;
    mlp = new MLP_Trace();
  }
  delete mlp_reader;
}

int main(void){
  test_mlp_file();
  return 0;
}
