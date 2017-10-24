/*
*
*  This file is covered under the Interval Academic License, see LICENCE.interval
*
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
*  Authors: Wim Heirman, Ghent University
*           Trevor Carlson, Ghent University
*           Sam Van den Steen, Ghent University
*  Copyright: 2016, Ghent University
*/

#ifndef TOOLS_HPP_
#define TOOLS_HPP_

#include "micro_op.h"

#include <cstring>

class Tools {
public:
#if 0
  inline static const bool jumpBetween(const Instruction* inst1, const Instruction* inst2) {
    return (inst1->PC().phys + inst1->PC().length) != inst2->PC().phys;
  }
#endif

  inline static bool contains(const uint64_t* array, const int length, const uint64_t value) {
    for(int i = 0; i < length; i++)
      if (array[i] == value)
        return true;
    return false;
  }

  inline static uint64_t index(const uint64_t* array, const int length, const uint64_t value) {
    for(int i = 0; i < length; i++)
      if (array[i] == value)
        return i;
    return UINT64_MAX;
  }

  inline static void swap(uint64_t* array, const int idx1, const int idx2) {
    uint64_t temp = array[idx1];
    array[idx1] = array[idx2];
    array[idx2] = temp;
  }
};

#endif /* TOOLS_HPP_ */
