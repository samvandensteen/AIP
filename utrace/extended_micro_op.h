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

#ifndef Extended_Micro_Op_H_
#define Extended_Micro_Op_H_

#include "micro_op.h"
#include "extended_uop_type_enum.h"

/*	Not 100% sure how many producers an uop can have
	To the best of my best knowledge:
 		- can depend on up to 3 source registers
 		- can depend on up to 2 previous uop's (intra-instruction-dependencies)
 		- can be a load and depend on 1 previous store
*/
#define MAX_PRODUCERS 6

using namespace std;

class Extended_Micro_Op{
	private:
		MicroOp uop;
		int seq_nr;
		uint64_t address;
		int path_length;
		int producing_load_position;
		int no_producing_loads;
		int producers[MAX_PRODUCERS];
		extended_uop_type type;

		void set_extended_uop_type();

	public:
		Extended_Micro_Op();
		Extended_Micro_Op(MicroOp op, int nr);
		Extended_Micro_Op(const Extended_Micro_Op & rhs);

		Extended_Micro_Op& operator=(const Extended_Micro_Op& rhs);

		bool operator<(const Extended_Micro_Op& rhs);
		bool operator>(const Extended_Micro_Op& rhs);
		bool operator<=(const Extended_Micro_Op& rhs);
		bool operator>=(const Extended_Micro_Op& rhs);

		bool is_load();
		bool is_store();
		bool is_branch();

		void add_producer(Extended_Micro_Op* prod);

		int* get_producers() ;
		MicroOp get_uop();
		extended_uop_type get_extended_uop_type();
		int get_seq_nr();

		uint64_t get_address();
		void set_address(uint64_t addr);
		int get_path_length();
		void set_path_length(int path);
		int get_producing_load_position();
		int get_no_producing_loads();
};

#endif
