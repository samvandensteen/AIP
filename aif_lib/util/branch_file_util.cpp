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

#include "branch_file_util.h"

void make_ip_bits(IP_Bits* ip, uint32_t bits, map<uint32_t, double> bhr_entropy){
  ip->set_bits(bits);
  for(map<uint32_t, double>::iterator ii = bhr_entropy.begin(); ii != bhr_entropy.end(); ++ii){
    ip->add_bhr_bits(ii->first);
    ip->add_entropy(ii->second);
  }
}

Entropy_Window* make_entropy_window(uint32_t ew_id, string t, uint32_t b){
  Entropy_Window *ew = new Entropy_Window();

  ew->set_entropy_window_id(ew_id);
  ew->set_type(t);
  ew->set_branches(b);

  return ew;
}

void add_ip_bits(Entropy_Window* ew, map<uint32_t, map<uint32_t, double>> ips){
  for(map<uint32_t, map<uint32_t, double>>::iterator ii = ips.begin(); ii != ips.end(); ++ii){
    IP_Bits* ip = ew->add_ips();
    make_ip_bits(ip, ii->first, ii->second);
    ii->second.clear();
  }
}

void print_entropy_window(Entropy_Window *ew){
  cout << "Entropy Window " << ew->entropy_window_id() << " -- "  << ew->type() << " -- " << ew->branches() << endl;
  cout << "IP\\BHR" << "\t";
  for(int i = 0; i < ew->ips(0).bhr_bits_size(); i++){
    cout << ew->ips(0).bhr_bits(i) << "\t";
  }
  cout << endl;
  for(int i = 0; i < ew->ips_size(); i++){
    cout << ew->ips(i).bits() << "\t";
    for(int j = 0; j < ew->ips(i).bhr_bits_size(); j++){
      cout << ew->ips(i).entropy(j) << "\t";
    }
    cout << endl;
  }
}
