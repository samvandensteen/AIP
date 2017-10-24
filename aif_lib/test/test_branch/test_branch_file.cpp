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

#include "branch_file_util.h"
#include "branch_file.pb.h"
#include "proto_writer.h"
#include "proto_reader.h"

#define ENABLE_COMPRESSION true

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::map;
using std::unordered_map;
using std::deque;

using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;

using branch_file::IP_Bits;
using branch_file::Entropy_Window;

void test_branch_file(){
  // trace stats file
  Entropy_Window* ew_w = make_entropy_window(0, "l", 100000);
  map<uint32_t, map<uint32_t, double>> ew_map;
  for(uint32_t j = 0; j < 10; j++){
    ew_map[j] = map<uint32_t, double>();
    for(uint32_t k = 0; k < 10; k++){
      ew_map[j][k] = 0.1;
    }
  }
  add_ip_bits(ew_w, ew_map);

  const string filename = "entropy";
  Proto_Writer output_file(filename, ENABLE_COMPRESSION);
  output_file.write_streaming(ew_w, true);
  output_file.close_file();

  // read data
  Proto_Reader input_file(filename + ".0", ENABLE_COMPRESSION);
  Entropy_Window *ew_r = new Entropy_Window();
  while (input_file.read_streaming(ew_r)){
    print_entropy_window(ew_r);
    delete ew_r;
    ew_r = new Entropy_Window();
  }
}

int main(void){
  test_branch_file();
  return 0;
}
