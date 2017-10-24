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
#include <cstdint>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "memory_file.pb.h"
#include "memory_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::ios;

using memory_file::Memory_Access;
using memory_file::Access_Type;
using memory_file::Sample;

using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;

void test_sample_file(){
  void* addr1 = (void*) 0xaaaaaaaaaaaaaaaa;
  void* addr2 = (void*) 0xbbbbbbbbbbbbbbbb;
  void* addr3 = (void*) 0xcccccccccccccccc;
  void* addr4 = (void*) 0xdddddddddddddddd;

  Memory_Access *access_1 = make_memory_access(0, (uint64_t) addr1, 1337, Access_Type::LOAD, 42, 0xdeadbeef);
  Memory_Access *access_2 = make_memory_access(1, (uint64_t) addr2, 2048, Access_Type::STORE, 3, 0xdeadbeef);
  Sample *sample1 = make_sample(access_1, access_2);

  Memory_Access *access_3 = make_memory_access(1, (uint64_t) addr3, 1337, Access_Type::LOAD, 42, 0xdeadbeef);
  Memory_Access *access_4 = make_memory_access(2, (uint64_t) addr4, 2048, Access_Type::STORE, 3, 0xdeadbeef);
  Sample *sample2 = make_sample(access_3, access_4);

  Proto_Writer *sample_writer = new Proto_Writer("memory_file", false);
  sample_writer->write_streaming(sample1, true);
  sample_writer->write_streaming(sample2, true);
  delete sample_writer;

  Proto_Reader *sample_reader = new Proto_Reader("memory_file.0", false);
  Sample *sample = new Sample();
  while(sample_reader->read_streaming(sample)){
  	print_sample(sample);
  	delete sample;
  	sample = new Sample();
  }
  delete sample_reader;
}

void test_burst_file(){
  Burst_Profile* bp_w = new Burst_Profile();
  bp_w->set_line_size(6);
  Burst* temp = add_burst(bp_w, 0, 0, 0);
  complete_burst(temp, 1000, 400, 100, 90, 50, 40);
  write_burst_profile("burst_test.0", bp_w);

  Burst_Profile* bp_r = read_burst_profile("burst_test.0");
  print_burst_profile(bp_r);
}

int main(void){
  cout << "Sample File:" << endl;
  test_sample_file();
  cout << endl;
  cout << "Burst File:" << endl;
  test_burst_file();
  return 0;
}
