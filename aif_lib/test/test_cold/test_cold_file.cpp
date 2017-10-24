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
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "cold_file.pb.h"
#include "cold_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::vector;

using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;

using cold_file::Cold_Window;

void test_cold_file(){
  srand(time(NULL));

  Cold_Window* cw1 = make_cold_window(1);
  // generate data
  vector<uint32_t> robs;
  vector<vector<uint64_t>> cold_miss_occurences;
  uint32_t counter = 0;
  for(int i = 1; i < 257; i += 16){
    robs.push_back(i);
    cold_miss_occurences.push_back(vector<uint64_t>(i, 0));
    for(int j = 0; j < (int) sqrt(i); j++){
      cold_miss_occurences[counter][j] = rand() % 256;
    }
    counter++;
  }
  //add data
  add_ROB_cold_distribution(cw1, 32, robs, cold_miss_occurences);

  Cold_Window* cw2 = make_cold_window(2);
  // generate data
  robs.clear();
  cold_miss_occurences.clear();
  counter = 0;
  for(int i = 256; i < 513; i += 16){
    robs.push_back(i);
    cold_miss_occurences.push_back(vector<uint64_t>(i, 0));
    for(int j = 0; j < (int) sqrt(i); j++){
      cold_miss_occurences[counter][j] = rand() % 256;
    }
    counter++;
  }
  //add data
  add_ROB_cold_distribution(cw2, 64, robs, cold_miss_occurences);

  // write data
  Proto_Writer *cold_writer = new Proto_Writer("cold", false);
  cold_writer->write_streaming(cw1, true);
  cold_writer->write_streaming(cw2, true);
  delete cold_writer;

  // read data
  Proto_Reader *cold_reader = new Proto_Reader("cold.0", false);
  Cold_Window *cw = new Cold_Window();
  while(cold_reader->read_streaming(cw)){
    print_cold_window(cw);
    delete cw;
    cw = new Cold_Window();
  }
  delete cold_reader;
}

int main(void){
  test_cold_file();
  return 0;
}
