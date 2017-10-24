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

#ifndef KNOB_H
#define KNOB_H

#include <cstdint>
#include <string>
#include <sstream>

using std::endl;
using std::string;
using std::stringstream;

// The class is used to hold all parameters that are adjustable.
class Knobs{
	public:
		// general
		uint64_t fastforward, warmup, detailed;
		int pintool_log_level;
		string roi;

		// paths
		uint64_t sample_length, sample_rate;

		// branches
		int max_ip, max_bhr;

		//trace stuff
		uint32_t repeat;
		uint32_t trace_ins_length, trace_mem_length, current_trace_length, trace_window;

		//output files
		string output_dir;

		bool enable_compression;

		int min_rob, step_rob, max_rob;
		bool approx_dep;

		// statstack
		string smp_base;
		uint64_t sample_period;
		string smp_rnd;
		uint64_t burst_period;
		string burst_rnd;
		uint64_t burst_size;
		uint64_t smp_line_size_lg2;
		uint64_t seed;
		uint64_t log_level;

		void parse_rob_string(string rob_string);
		string generate_uop_string();
		string get_log_header();
};

#endif
