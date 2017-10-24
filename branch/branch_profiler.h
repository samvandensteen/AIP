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
*						Sander De Pestel, Ghent University
*  Copyright: 2016, Ghent University
*/

#ifndef BRANCH_PROFILER_H_
#define BRANCH_PROFILER_H_

#include "knobs.h"
#include "entropy.h"
#include "branch_file_util.h"
#include "proto_writer.h"

class branch_profiler{
	public:
		branch_profiler(Knobs *knobs);
		~branch_profiler();
		void reset();
		void add_branch(uint64_t pc, bool isTaken);

		void end_phase(uint32_t trace_count);

	private:
		EntropySimulator* simulator_global;
		EntropySimulator* simulator_local;
		TourEntropySimulator* simulator_tournament;

		map<uint32_t, map<uint32_t, double>> output_to_proto(map<int, map<uint64_t, EntroNumber*>> output);
		Proto_Writer *pw;
};

#endif /*BRANCH_PROFILER_H_*/
