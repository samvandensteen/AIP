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
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "utrace_file.pb.h"
#include "utrace_file_util.h"
#include "proto_writer.h"
#include "proto_reader.h"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::ios;

using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::IstreamInputStream;

using utrace_file::uTrace;
using utrace_file::Stats;
using utrace_file::Dependences;
using utrace_file::Histogram;

void test_utrace_file(){
  // trace stats file
  vector<string> uop_string_id{"COND_BRANCH", "UNCOND_BRANCH", "CHNGREG", "I_ADD", "I_SUB", "I_MUL", "I_DIV", "FP_ADD", "FP_SUB", "FP_MUL", "FP_DIV", "FP_SQRT", "GENERIC_1", "GENERIC_2", "GENERIC_3", "GENERIC_5", "LOAD", "OTHER", "STORE"};
  ID_string* ids = make_id_string(uop_string_id);

  // utrace 0
  uTrace *ut_0 = make_utrace(0);
  add_utrace_stats(ut_0, 900, 1000, 250, 100);
  add_utrace_dependences(ut_0, {1, 16, 32, 48, 64, 80, 96, 112, 128}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}, {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5}, {2.0, 4.0, 6.0, 8.0, 10.0, 12.0, 14.0, 16.0, 18.0});
  add_utrace_uop_hist(ut_0, {1, 2, 3, 5, 6, 7, 8, 9, 11, 12, 14, 15, 16, 17}, {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100});

  // utrace 1
  uTrace *ut_1 = make_utrace(1);
  add_utrace_stats(ut_1, 1000, 1100, 300, 150);
  add_utrace_dependences(ut_1, {1, 16, 32, 48, 64, 80, 96, 112, 128}, {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5}, {2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5}, {5.0, 7.0, 9.0, 11.0, 13.0, 15.0, 17.0, 19.0, 21.0});
  add_utrace_uop_hist(ut_1, {0, 1, 3, 4, 7, 8, 9, 10, 12, 13, 14, 17, 18}, {200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200});

  // write data
  Proto_Writer *utrace_writer = new Proto_Writer("utrace", false);
  utrace_writer->write_streaming(ids, true);
  utrace_writer->write_streaming(ut_0, true);
  utrace_writer->write_streaming(ut_1, true);
  delete ids;
  delete utrace_writer;

  // read data
  Proto_Reader *utrace_reader = new Proto_Reader("utrace.0", false);

  ids = new ID_string();
  utrace_reader->read_streaming(ids);
  vector<string> read_uop_string_id;
  for(int i = 0; i < ids->uop_id_to_string_size(); i++){
    read_uop_string_id.push_back(ids->uop_id_to_string(i));
  }

  uTrace *ut = new uTrace();
  while(utrace_reader->read_streaming(ut)){
    print_utrace(ut, read_uop_string_id);
    delete ut;
    ut = new uTrace();
  }
  delete utrace_reader;
}

int main(void){
  test_utrace_file();
  return 0;
}
