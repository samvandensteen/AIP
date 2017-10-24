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

#include "branch_profiler.h"

branch_profiler::branch_profiler(Knobs *knobs){
	simulator_global = new EntropySimulator(32, 0, "Global", knobs->max_bhr, knobs->max_ip);
	simulator_local = new EntropySimulator(32, 32, "Local", knobs->max_bhr, knobs->max_ip);
	simulator_tournament = new TourEntropySimulator();

	pw = new Proto_Writer("entropy", knobs->enable_compression);
}

branch_profiler::~branch_profiler(){
	delete pw;
}

void branch_profiler::reset(){}

void branch_profiler::add_branch(uint64_t pc, bool isTaken){
	int taken = isTaken ? 1 : 0;
	simulator_global->addBranch(pc, taken, true);
	simulator_local->addBranch(pc, taken, true);
	simulator_tournament->addBranch();
}

map<uint32_t, map<uint32_t, double>> branch_profiler::output_to_proto(map<int, map<uint64_t, EntroNumber*>> output){
	map<uint32_t, map<uint32_t, double>> output_proto;
	for(map<int, map<uint64_t, EntroNumber*>>::iterator it_1 = output.begin(); it_1 != output.end(); ++it_1){
		output_proto[(uint32_t) it_1->first] = map<uint32_t, double>();
		for(map<uint64_t, EntroNumber*>::iterator it_2 = it_1->second.begin(); it_2 != it_1->second.end(); ++it_2){
			// this conversion is safe since the entries weren't 64 bits anyway, but I'm not feeling like modifying all of Sander's source code
			output_proto[(uint32_t) it_1->first][(uint32_t) it_2->first] = it_2->second->getEntropy();
			delete it_2->second;
		}
	}
	return output_proto;
}

void branch_profiler::end_phase(uint32_t trace_count){
	//calculate entorpy values
	map<int, map<uint64_t, EntroNumber*>> local_output, global_output, tour_output;
	map<int, map<uint64_t, map<uint64_t, EntroNumber*>>> local_per_branch_output, global_per_branch_output;

	simulator_local->getOutput(local_output, local_per_branch_output);
	map<uint32_t, map<uint32_t, double>> local_output_proto = output_to_proto(local_output);
	uint64_t branches_local = local_output[0][0]->getNumber();
	Entropy_Window* ewl = make_entropy_window(trace_count, "l", branches_local);
	add_ip_bits(ewl, local_output_proto);
	pw->write_streaming(ewl, true);
	delete ewl;

	simulator_global->getOutput(global_output, global_per_branch_output);
	map<uint32_t, map<uint32_t, double>> global_output_proto = output_to_proto(global_output);
	uint64_t branches_global = global_output[0][0]->getNumber();
	Entropy_Window* ewg = make_entropy_window(trace_count, "g", branches_global);
	add_ip_bits(ewg, global_output_proto);
	pw->write_streaming(ewg, true);
	delete ewg;

	simulator_tournament->getOutput(tour_output, global_per_branch_output, local_per_branch_output);
	map<uint32_t, map<uint32_t, double>> tour_output_proto = output_to_proto(tour_output);
	uint64_t branches_tournament = tour_output[0][0]->getNumber();
	Entropy_Window* ewt = make_entropy_window(trace_count, "t", branches_tournament);
	add_ip_bits(ewt, tour_output_proto);
	pw->write_streaming(ewt, true);
	delete ewt;

	simulator_local->reset();
	simulator_global->reset();
	simulator_tournament->reset();
}
