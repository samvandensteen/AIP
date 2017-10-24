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
*  Authors: Wim Heirman, Ghent University
*           Trevor Carlson, Ghent University
*           Sam Van den Steen, Ghent University
*  Copyright: 2016, Ghent University
*/
#ifndef INSTRUCTION_INFO_HPP_
#define INSTRUCTION_INFO_HPP_

#include "fixed_types.h"

extern "C" {
#include <xed-decoded-inst.h>
}

#include <vector>
#include <set>

class Instruction;
class MicroOp;

class InstructionDecoder {
private:
   static void addSrcs(std::set<xed_reg_enum_t> regs, MicroOp *uop);
   static void addAddrs(std::set<xed_reg_enum_t> regs, MicroOp *uop);
   static void addDsts(std::set<xed_reg_enum_t> regs, MicroOp *uop);
   static unsigned int getNumExecs(const xed_decoded_inst_t *ins, int numLoads, int numStores);
public:
   static const std::vector<const MicroOp*>* decode(IntPtr address, const xed_decoded_inst_t *ins, Instruction *ins_ptr);
};

#endif /* INSTRUCTION_INFO_HPP_ */
