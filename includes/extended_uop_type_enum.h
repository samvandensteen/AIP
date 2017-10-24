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

#ifndef EXTENDED_UOP_TYPE_ENUM_H_
#define EXTENDED_UOP_TYPE_ENUM_H_

#include <string>

#include "pin.H"

typedef enum extended_uop_type{
	// not all branches are conditional branches (e.g. call or ret aren't), we only care about the conditional ones for the branch prediction
	UOP_TYPE_COND_BRANCH,
	UOP_TYPE_UNCOND_BRANCH,
	UOP_TYPE_CHNGREG,
	UOP_TYPE_I_ADD,
	UOP_TYPE_I_SUB,
	UOP_TYPE_I_MUL,
	UOP_TYPE_I_DIV,
	UOP_TYPE_FP_ADD,
	UOP_TYPE_FP_SUB,
	UOP_TYPE_FP_MUL,
	UOP_TYPE_FP_DIV,
	UOP_TYPE_FP_SQRT,
	UOP_TYPE_GENERIC_1,
	UOP_TYPE_GENERIC_2,
	UOP_TYPE_GENERIC_3,
	UOP_TYPE_GENERIC_5,
	UOP_TYPE_LOAD,
	UOP_TYPE_OTHER,
	UOP_TYPE_STORE,
	UOP_TYPE_INVALID
} extended_uop_type;

std::string uop_to_string(extended_uop_type uop_t);
std::string convert_uop_to_xedstring(xed_iclass_enum_t uop);

#endif
