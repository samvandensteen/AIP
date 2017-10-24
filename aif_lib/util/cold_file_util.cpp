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

#include "cold_file_util.h"

void make_ROB_cold_distribution(ROB_cold_distribution *rcd, uint32_t cacheblock, uint32_t rob, vector<uint64_t> occurences){
  rcd->set_cacheblock_size(cacheblock);
  rcd->set_rob_size(rob);
  vector<uint64_t>::iterator occurences_it = occurences.begin();
  uint32_t cold_misses_in_ROB = 1;
  for(; occurences_it != occurences.end(); ++occurences_it){
    if(*occurences_it != 0){
      rcd->add_misses_in_rob(cold_misses_in_ROB);
      rcd->add_occurences(*occurences_it);
    }
    cold_misses_in_ROB++;
  }
}

Cold_Window* make_cold_window(uint32_t window_id){
  Cold_Window *cw = new Cold_Window();

  cw->set_window_id(window_id);

  return cw;
}

void add_ROB_cold_distribution(Cold_Window* cw, uint32_t cacheblock, vector<uint32_t> robs, vector<vector<uint64_t>> occurences){
  vector<uint32_t>::iterator rob_it = robs.begin();
  vector<vector<uint64_t>>::iterator occurences_it = occurences.begin();
  for(; rob_it != robs.end() && occurences_it != occurences.end(); ++rob_it, ++occurences_it){
    ROB_cold_distribution *rcd = cw->add_cold_distribution();
    make_ROB_cold_distribution(rcd, cacheblock, *rob_it, *occurences_it);
  }
}

void print_cold_window(Cold_Window *cw){
  cout << "Window " << cw->window_id() << endl;
  for(int i = 0; i < cw->cold_distribution_size(); i++){
    ROB_cold_distribution rcd = cw->cold_distribution(i);
    cout << "Cacheblock = " << rcd.cacheblock_size() << " - ROB = " << rcd.rob_size() << ":\t";
    for(int j = 0; j < rcd.misses_in_rob_size(); j++){
      cout << "(" << rcd.misses_in_rob(j) << "," << rcd.occurences(j) << ")\t";
    }
    cout << endl;
  }
  cout << endl;
}
