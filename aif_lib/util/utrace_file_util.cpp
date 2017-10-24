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

#include "utrace_file_util.h"

ID_string* make_id_string(vector<string> uop_string_id){
  ID_string *ids = new ID_string();
  for(vector<string>::iterator it = uop_string_id.begin(); it != uop_string_id.end(); ++it){
    ids->add_uop_id_to_string(*it);
  }
  return ids;
}

uTrace* make_utrace(uint64_t utrace_id){
  uTrace *ut = new uTrace();
  ut->set_utrace_id(utrace_id);
  return ut;
}

void add_utrace_stats(uTrace* ut, uint32_t trace_inscount, uint32_t trace_uopcount, uint32_t trace_load_count, uint32_t trace_store_count){
  Stats *s = ut->mutable_stats();
  s->set_inscount(trace_inscount);
  s->set_uopcount(trace_uopcount);
  s->set_loads(trace_load_count);
  s->set_stores(trace_store_count);
}

void add_utrace_dependences(uTrace* ut, vector<uint32_t> ROB, vector<double> AP, vector<double> BP, vector<double> CP){
  Dependences *trace = ut->mutable_trace();
  vector<uint32_t>::iterator it_1 = ROB.begin();
  vector<double>::iterator it_2 = AP.begin();
  vector<double>::iterator it_3 = BP.begin();
  vector<double>::iterator it_4 = CP.begin();
  for(;it_1 != ROB.end() && it_2 != AP.end() && it_3 != BP.end() && it_4 != CP.end(); ++it_1,++it_2,++it_3,++it_4){
    trace->add_rob(*it_1);
    trace->add_average_path(*it_2);
    trace->add_branch_path(*it_3);
    trace->add_critical_path(*it_4);
  }
}

void add_utrace_uop_hist(uTrace* ut, vector<uint32_t> uop_id, vector<uint32_t> uop_freq){
  Histogram *hist = ut->mutable_hist();
  vector<uint32_t>::iterator it_1 = uop_id.begin();
  vector<uint32_t>::iterator it_2 = uop_freq.begin();
  for(;it_1 != uop_id.end() && it_2 != uop_freq.end(); ++it_1,++it_2){
    hist->add_uop_id(*it_1);
    hist->add_uop_freq(*it_2);
  }
}

void print_utrace(uTrace* ut, vector<string> id_to_string){
  cout << "ID: " << ut->utrace_id() << endl;
  cout << "STATS: " << endl;
  print_utrace_stats(ut->stats());
  cout << "DEPENDENCES: " << endl;
  print_utrace_dependences(ut->trace());
  cout << "HIST: " << endl;
  print_utrace_uop_hist(ut->hist(), id_to_string);
  cout << endl;
}

void print_utrace_stats(const Stats &s){
  cout << "I: " << s.inscount() << " - U: " << s.uopcount() << " - L: " << s.loads()  << " - S: " << s.stores() << endl;
}

void print_utrace_dependences(const Dependences &d){
  for(int i = 0; i < d.rob_size(); i++){
    cout << d.rob(i) << "\t" << d.average_path(i) << "\t" << d.branch_path(i) << "\t" << d.critical_path(i) << endl;
  }
}

void print_utrace_uop_hist(const Histogram &uh, vector<string> id_to_string){
  for(int i = 0; i < uh.uop_id_size(); i++){
    cout << id_to_string[uh.uop_id(i)] << ": " << uh.uop_freq(i) << endl;
  }
}
