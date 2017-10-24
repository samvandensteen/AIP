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

#include "mlp_file_util.h"

void make_load_chain(Load_chain *lc, uint32_t rob, deque<uint32_t> load_chain_distr){
  lc->set_rob_size(rob);
  for(deque<uint32_t>::iterator it = load_chain_distr.begin(); it != load_chain_distr.end(); ++it){
    lc->add_frequency(*it);
  }
}

void make_pc_reuse(PC_reuse* pr, uint32_t pc_first, unordered_map<uint32_t, uint32_t>& pc_RD){
  pr->set_first_reference(pc_first);
  for(unordered_map<uint32_t, uint32_t>::iterator it = pc_RD.begin(); it != pc_RD.end(); ++it){
    pr->add_reuse(it->first);
    pr->add_times(it->second);
  }
}

void make_pc_stride(PC_stride *ps, unordered_map<uint64_t, uint32_t>& pc_stride, uint64_t first_address){
  ps->set_first_address(first_address);
	for(unordered_map<uint64_t, uint32_t>::iterator it = pc_stride.begin(); it != pc_stride.end(); ++it){
		ps->add_stride(it->first);
    ps->add_times(it->second);
	}
}

MLP_Trace* make_mlp_trace(uint32_t trace_id){
  MLP_Trace *mlp = new MLP_Trace();

  mlp->set_trace_id(trace_id);

  return mlp;
}

void add_load_chains(MLP_Trace* mlp, map<uint32_t, deque<uint32_t>>& load_chains){
  for(map<uint32_t, deque<uint32_t>>::iterator it = load_chains.begin(); it != load_chains.end(); ++it){
    Load_chain *lc = mlp->add_chain_stats();
    make_load_chain(lc, it->first, it->second);
  }
}

void add_pc_reuses(MLP_Trace* mlp, unordered_map<uint64_t, unordered_map<uint32_t, uint32_t>>& pcs_RD, unordered_map<uint64_t, uint32_t>& pc_first){
  unordered_map<uint64_t, unordered_map<uint32_t, uint32_t>>::iterator it_1 = pcs_RD.begin();
  unordered_map<uint64_t, uint32_t>::iterator it_2 = pc_first.begin();
  for(; it_1 != pcs_RD.end() && it_2 != pc_first.end(); ++it_1, ++it_2){
    mlp->add_pc(it_1->first);
    PC_reuse *pr = mlp->add_reuse_stats();
    make_pc_reuse(pr, it_2->second, it_1->second);
  }
}

void add_pc_strides(MLP_Trace* mlp, unordered_map<uint64_t, unordered_map<uint64_t, uint32_t>>& pcs_stride, unordered_map<uint64_t, uint64_t>& pcs_first_address){
  unordered_map<uint64_t, unordered_map<uint64_t, uint32_t>>::iterator it_1 = pcs_stride.begin();
  unordered_map<uint64_t, uint64_t>::iterator it_2 = pcs_first_address.begin();
  for(; it_1 != pcs_stride.end() && it_2 != pcs_first_address.end(); ++it_1, ++it_2){
    PC_stride *ps = mlp->add_stride_stats();
    make_pc_stride(ps, it_1->second, it_2->second);
  }
}

void print_mlp_trace(MLP_Trace *mlp){
  cout << "Trace " << mlp->trace_id() << endl;
  cout << "ROB load chain distribution: " << endl;
  for(int i = 0; i < mlp->chain_stats_size(); i++){
    Load_chain chain = mlp->chain_stats(i);
    cout << chain.rob_size() << "\t";
    for(int j = 0; j < chain.frequency_size(); j++){
      cout << chain.frequency(j) << "\t";
    }
    cout << endl;
  }
  cout << endl << "PC reuse distribution: " << endl;
  for(int i = 0; i < mlp->pc_size(); i++){
    cout << mlp->pc(i) << "\t";

    PC_reuse reuse = mlp->reuse_stats(i);
    cout << reuse.first_reference() << "\t";
    for(int j = 0; j < reuse.reuse_size(); j++){
      cout << "(" << reuse.reuse(j) << ";" << reuse.times(j) << ")\t";
    }
    cout << endl;
  }
  cout << endl << "PC stride distribution: " << endl;
  for(int i = 0; i < mlp->pc_size(); i++){
    cout << mlp->pc(i) << "\t";

    PC_stride stride = mlp->stride_stats(i);
    cout << stride.first_address() << "\t";
    for(int j = 0; j < stride.stride_size(); j++){
      cout << "(" << stride.stride(j) << ";" << stride.times(j) << ")\t";
    }
    cout << endl;
  }
  cout << endl << endl;
}
