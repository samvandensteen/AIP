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

#ifndef DEPENDENCE_PROCESSOR_H
#define DEPENDENCE_PROCESSOR_H

#include <vector>
#include <map>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <sstream>
#include <algorithm>

#include "define_includes.h"
#include "extended_uop_type_enum.h"
#include "utrace_file.pb.h"
#include "utrace_file_util.h"

using std::map;
using std::vector;

using utrace_file::uTrace;

class Dependence_Processor{
	private:
		bool approximate;
		int min_rob, rob_step, max_rob;
		int trace_size;
		vector<extended_uop_type> trace_uop_type;
		vector<vector<int>> trace;
		vector<vector<int>> rob_sizes;
		vector<map<int, vector<int>>> uop_chains;

		vector<double> avg_paths, avg_branch_paths, crit_paths/*, max_dependent_loads*/;
		// vector<vector<double>> avg_path_mix, avg_branch_path_mix, crit_path_mix;

		void calculate_ROB_sizes();
		void calculate_uop_chains();

		void clear_data_structures();

	public:
		Dependence_Processor(bool approximate, int min_rob, int rob_step, int max_rob);

		void process_trace(uTrace *ut, vector<vector<int>> trace, vector<extended_uop_type> trace_uop_type, uint32_t trace_count);

		void calculate_dependence_chain_stats(int ROB_size, double &AP, double &ABP, double &CP/*, double &MDL, vector<double> &APM, vector<double> &ABPM, vector<double> &CPM*/);
		void approximate_dependence_chain_stats(int ROB_size, double &AP, double &ABP, double &CP/*, double &MDL, vector<double> &APM, vector<double> &ABPM, vector<double> &CPM*/);
};

#endif /*DEPENDENCE_PROCESSOR_H*/
