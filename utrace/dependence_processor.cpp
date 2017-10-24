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

#include "dependence_processor.h"

// Commented out code calculates the uop mix on dependence chains, but this is slow
// Don't remove commented out code, this might prove useful in the future

Dependence_Processor::Dependence_Processor(bool approximate, int min_rob, int rob_step, int max_rob){
	trace_size = 0;
	this->approximate = approximate;
	this->min_rob = min_rob;
	this->rob_step = rob_step;
	this->max_rob = max_rob;
}

void Dependence_Processor::process_trace(uTrace *ut, vector<vector<int>> trace, vector<extended_uop_type> trace_uop_type, uint32_t trace_count){
	trace_size = trace.size();
	this->trace = trace;
	this->trace_uop_type = trace_uop_type;

	calculate_uop_chains();
	calculate_ROB_sizes();

	// fill vectors with default values
	for(int i = min_rob; i <= max_rob; i += rob_step){
		avg_paths.push_back(1);
		avg_branch_paths.push_back(1);
		crit_paths.push_back(1);
		// max_dependent_loads.push_back(1);

		// avg_path_mix.push_back(vector<double>(UOP_TYPE_INVALID, 0.0));
		// avg_branch_path_mix.push_back(vector<double>(UOP_TYPE_INVALID, 0.0));
		// crit_path_mix.push_back(vector<double>(UOP_TYPE_INVALID, 0.0));
	}
	// reserve space for ROB = 1 if needed
	if(min_rob != 1){
		avg_paths.push_back(1);
		avg_branch_paths.push_back(1);
		crit_paths.push_back(1);
		// max_dependent_loads.push_back(1);

		// avg_path_mix.push_back(vector<double>(UOP_TYPE_INVALID, 0.0));
		// avg_branch_path_mix.push_back(vector<double>(UOP_TYPE_INVALID, 0.0));
		// crit_path_mix.push_back(vector<double>(UOP_TYPE_INVALID, 0.0));
	}

	int loop_counter = avg_paths.size() - 1;
	if(approximate){
		for(int i = max_rob; i >= min_rob; i -= rob_step){
			approximate_dependence_chain_stats(i, avg_paths[loop_counter], avg_branch_paths[loop_counter], crit_paths[loop_counter]/*, max_dependent_loads[loop_counter], avg_path_mix[loop_counter], avg_branch_path_mix[loop_counter], crit_path_mix[loop_counter]*/);
			loop_counter--;
		}
		// always calculate this dependences for ROB = 1 (needed for interpolation in the analyzer)
		if(min_rob != 1){
			approximate_dependence_chain_stats(1, avg_paths[0], avg_branch_paths[0], crit_paths[0]/*, max_dependent_loads[0], avg_path_mix[0], avg_branch_path_mix[0], crit_path_mix[0]*/);
		}
	}
	else{
		for(int i = max_rob; i >= min_rob; i -= rob_step){
			calculate_dependence_chain_stats(i, avg_paths[loop_counter], avg_branch_paths[loop_counter], crit_paths[loop_counter]/*, max_dependent_loads[loop_counter], avg_path_mix[loop_counter], avg_branch_path_mix[loop_counter], crit_path_mix[loop_counter]*/);
			loop_counter--;
		}
		// always calculate this dependences for ROB = 1 (needed for interpolation in the analyzer)
		if(min_rob != 1){
			calculate_dependence_chain_stats(1, avg_paths[0], avg_branch_paths[0], crit_paths[0]/*, max_dependent_loads[0], avg_path_mix[0], avg_branch_path_mix[0], crit_path_mix[0]*/);
		}
	}

	vector<uint32_t> ROB;
	if(min_rob != 1){
		ROB.push_back(1);
	}
	for(int i = min_rob; i <= max_rob; i += rob_step){
		ROB.push_back(i);
	}
	add_utrace_dependences(ut, ROB, avg_paths, avg_branch_paths, crit_paths);

	// tp->save_dependence_stats(trace_count, min_rob, rob_step, avg_paths, avg_branch_paths, crit_paths, max_dependent_loads);
	// tp->save_uop_mix_stats(trace_count, min_rob, rob_step, avg_path_mix, avg_branch_path_mix, crit_path_mix);
	clear_data_structures();
}

void Dependence_Processor::calculate_ROB_sizes(){
	rob_sizes = vector<vector<int>>(trace_size, vector<int>(1, 1));
	for(int i = 0; i < trace_size; i++){
		// loop over all producers
		for(size_t j = 0; j < trace[i].size(); j++){
			int uop_spacing = i - trace[i][j], dependent_on = trace[i][j];
			if (uop_spacing < 0 || dependent_on >= int(uop_chains.size())){
				continue;
			}
			for(size_t k = 0; k < rob_sizes[dependent_on].size(); k++){
				// only push values that are smaller, pushing larger values would only lead to more unnecessary iterations in the next steps
				int min_rob_size = uop_spacing + rob_sizes[dependent_on][k];
				if(min_rob_size <= max_rob){
					if(rob_sizes[i].size() > (size_t) k + 1){
						if(rob_sizes[i][k + 1] > min_rob_size){
							rob_sizes[i][k + 1] = min_rob_size;
						}
					}
					else{
						rob_sizes[i].push_back(min_rob_size);
					}
				}
			}
		}
	}
}

void Dependence_Processor::calculate_uop_chains(){
	uop_chains = vector<map<int, vector<int>>>(trace_size, map<int, vector<int>>());
	for(int i = 0; i < trace_size; i++){
		// loop over all producers
		for(size_t j = 0; j < trace[i].size(); j++){
			int uop_spacing = i - trace[i][j], dependent_on = trace[i][j];
			// In case we depend on a uop outside of the uop trace size, ignore it
			if (uop_spacing < 0 || dependent_on >= int(uop_chains.size())){
				continue;
			}
			// analyze the micro-ops along the dependency chain
			for(map<int,vector<int>>::iterator it = uop_chains.at(dependent_on).begin(); it != uop_chains.at(dependent_on).end(); ++it){
				int min_rob_size = uop_spacing + it->first;
				if(min_rob_size <= max_rob){
					// in theory it's possible for a dependence chain to have the same producer twice through other instruction dependences
					//	-----------------
					//	|				|
					//	-----------------
					//	|	|	|	|	|
					//	A	B	C	D	E
					//	B, C, D are one instruction, three micro-ops, so D is dependent on both B and C, B is dependent on A and E is dependent on D and on A
					// in that case, take the longest chain as the correct one
					map<int,vector<int>>::iterator found = uop_chains[i].find(min_rob_size);
					if(found == uop_chains[i].end()){
						vector<int> producer_uops = it->second;
						uop_chains[i][min_rob_size] = vector<int>(producer_uops.begin(), producer_uops.end());
						uop_chains[i][min_rob_size][trace_uop_type[i]]++;
					}
					else{
						int old_sum = 0, new_sum = 1;
						vector<int> producer_uops = it->second;
						for(int n : uop_chains[i][min_rob_size]){
							old_sum += n;
						}
						for(int n : producer_uops){
							new_sum += n;
						}
						if(new_sum > old_sum){
							uop_chains[i][min_rob_size] = vector<int>(producer_uops.begin(), producer_uops.end());
							uop_chains[i][min_rob_size][trace_uop_type[i]]++;
						}
					}
				}
			}
		}
		// fill in the case for when the current instruction is an independent one
		uop_chains[i][1] = vector<int>(UOP_TYPE_INVALID, 0);
		uop_chains[i][1][trace_uop_type[i]]++;
	}
}

void Dependence_Processor::calculate_dependence_chain_stats(int ROB_size, double &AP, double &ABP, double &CP/*, double &MDL, vector<double> &APM, vector<double> &ABPM, vector<double> &CPM*/){
	if( ROB_size > trace_size){
		return;
	}

	int outer_loops = trace_size - ROB_size + 1;

	int sum_avg_paths = 0, sum_branch_paths = 0, branches = 0, sum_crit_paths = 0/*, max_dep_loads = 0*/;
	// vector<int> path_uop_mix = vector<int>(UOP_TYPE_INVALID, 0);
	// vector<int> branch_uop_mix = vector<int>(UOP_TYPE_INVALID, 0);
	// vector<int> path_uop_mix_max = vector<int>(UOP_TYPE_INVALID, 0);

	int current_ROB_begin = 0;
	vector<int> max_vector = vector<int>(UOP_TYPE_INVALID, 0)/*, curr_mix = vector<int>(UOP_TYPE_INVALID, 0)*/;
	vector<vector<int>>::iterator it_1 = rob_sizes.begin(), it_2 = rob_sizes.end() - ROB_size + 1;
	vector<map<int, vector<int>>>::iterator it_3 = uop_chains.begin(), it_4 = uop_chains.end() - ROB_size + 1;
	while(it_1 != it_2 && it_3 != it_4){
		// int max_path = 0, crit_max = 0, load_max = 0;
		int crit_max = 0;
		for(int i = 0; i < ROB_size; i++){
			vector<vector<int>>::iterator curr_uop = it_1 + i;
			int live_dependences = (it_1 + i)->size();
			// int req_rob_size = 0;

			vector<int>::reverse_iterator rit = curr_uop->rbegin();
			while(rit != curr_uop->rend()){
				if(i - (*rit - 1) >= 0){
					// req_rob_size = *rit;
					break;
				}
				live_dependences--;
				++rit;

				// delete this element after incrementing the reverse iterator to make sure we don't use it in next iterations
				if(*rit > ROB_size){
					rob_sizes[current_ROB_begin + i].pop_back();
				}
			}

			// // now calculate the uop mix on those dependence paths
			// vector<map<int, vector<int>>>::iterator curr_uop_map = it_3 + i;
			// map<int, vector<int>>::iterator it_5 = curr_uop_map->find(req_rob_size);
			// do{
			// 	++it_5;
			// } while(it_5 != curr_uop_map->end() && it_5->first < i);
			// curr_mix = (--it_5)->second;
			// // curr_mix = (it_3 + i)->find(req_rob_size)->second;

			// add current results
			sum_avg_paths += live_dependences;
			if(live_dependences > crit_max){
				crit_max = live_dependences;
			}

			// // calculate average and max uop mix stats
			// int sum_mix = 0;
			// for(int j = 0; j < UOP_TYPE_INVALID; j++){
			// 	path_uop_mix[j] += curr_mix[j];
			// 	sum_mix += curr_mix[j];
			// }
			// if(sum_mix > max_path){
			// 	max_path = sum_mix;
			// 	max_vector = curr_mix;
			// }

			// check if the current uop is a branch, if so, add the different statistics
			if(trace_uop_type[current_ROB_begin + i] == UOP_TYPE_COND_BRANCH){
				branches++;
				sum_branch_paths += live_dependences;
				// for(int j = 0; j < UOP_TYPE_INVALID; j++){
				// 	branch_uop_mix[j] += curr_mix[j];
				// }
			}

			// if(trace_uop_type[current_ROB_begin + i] == UOP_TYPE_LOAD){
			// 	if(curr_mix[UOP_TYPE_LOAD] > load_max){
			// 		load_max = curr_mix[UOP_TYPE_LOAD];
			// 	}
			// }
		}

		sum_crit_paths += crit_max;
		// max_dep_loads += load_max;

		// for(int i = 0; i < UOP_TYPE_INVALID; i++){
		// 	path_uop_mix_max[i] += max_vector[i];
		// }

		++current_ROB_begin;
		++it_1;
		++it_3;
	}

	// set to 1 value to prevent division by 0
	branches = branches > 0 ? branches : 1;
	// don't want to construct a vector to return multiple results, hence, use values that were passed by reference to save the results
	AP = (double) sum_avg_paths / (ROB_size * outer_loops);
	ABP = (double) sum_branch_paths / branches;
	CP = (double) sum_crit_paths / outer_loops;
	// MDL = (double) max_dep_loads / outer_loops;

	// for(int i = 0; i < UOP_TYPE_INVALID; i++){
	// 	APM[i] = (double) path_uop_mix[i] / (ROB_size * outer_loops);
	// 	ABPM[i] = (double) branch_uop_mix[i] / branches;
	// 	CPM[i] = (double) path_uop_mix_max[i] / outer_loops;
	// }
}

void Dependence_Processor::approximate_dependence_chain_stats(int ROB_size, double &AP, double &ABP, double &CP/*, double &MDL, vector<double> &APM, vector<double> &ABPM, vector<double> &CPM*/){
	if( ROB_size > trace_size){
		return;
	}

	int outer_loops = (trace_size - ROB_size + 1) / rob_step;

	int sum_avg_paths = 0, sum_branch_paths = 0, branches = 0, sum_crit_paths = 0/*, max_dep_loads = 0*/;
	// vector<int> path_uop_mix = vector<int>(UOP_TYPE_INVALID, 0);
	// vector<int> branch_uop_mix = vector<int>(UOP_TYPE_INVALID, 0);
	// vector<int> path_uop_mix_max = vector<int>(UOP_TYPE_INVALID, 0);

	int current_ROB_begin = 0;
	vector<int> max_vector = vector<int>(UOP_TYPE_INVALID, 0)/*, curr_mix = vector<int>(UOP_TYPE_INVALID, 0)*/;

	for(int i = 0; i < trace_size - ROB_size + 1; i += rob_step){
		// int max_path = 0, crit_max = 0, load_max = 0;
		int crit_max = 0;
		for(int j = 0; j < ROB_size; j++){
			vector<int> current = rob_sizes[i + j];
			int live_dependences = current.size();
			// int req_rob_size = 0;

			for(int k = live_dependences - 1; k > 0; k--){
				if(j - (current[k] - 1) >= 0){
					// req_rob_size = current[k];
					break;
				}
				live_dependences--;

				// delete this element after incrementing the reverse iterator to make sure we don't use it in next iterations
				if(current[k] > ROB_size){
					rob_sizes[i + j].pop_back();
				}
			}

			// // now calculate the uop mix on those dependence paths
			// map<int, vector<int>>* curr_uop_map = &uop_chains[i + j];
			// map<int, vector<int>>::iterator it = curr_uop_map->find(req_rob_size);
			// do{
			// 	++it;
			// } while(it != curr_uop_map->end() && it->first < j);
			// curr_mix = (--it)->second;
			// // curr_mix = (&uop_chains[i + j])->find(req_rob_size)->second;

			// add current results
			sum_avg_paths += live_dependences;
			if(live_dependences > crit_max){
				crit_max = live_dependences;
			}

			// // calculate average and max uop mix stats
			// int sum_mix = 0;
			// for(int k = 0; k < UOP_TYPE_INVALID; k++){
			// 	path_uop_mix[k] += curr_mix[k];
			// 	sum_mix += curr_mix[k];
			// }
			// if(sum_mix > max_path){
			// 	max_path = sum_mix;
			// 	max_vector = curr_mix;
			// }

			// check if the current uop is a branch, if so, add the different statistics
			if(trace_uop_type[current_ROB_begin + j] == UOP_TYPE_COND_BRANCH){
				branches++;
				sum_branch_paths += live_dependences;
				// for(int k = 0; k < UOP_TYPE_INVALID; k++){
				// 	branch_uop_mix[k] += curr_mix[k];
				// }
			}

			// if(trace_uop_type[current_ROB_begin + j] == UOP_TYPE_LOAD){
			// 	if(curr_mix[UOP_TYPE_LOAD] > load_max){
			// 		load_max = curr_mix[UOP_TYPE_LOAD];
			// 	}
			// }
		}

		sum_crit_paths += crit_max;
		// max_dep_loads += load_max;

		// for(int i = 0; i < UOP_TYPE_INVALID; i++){
		// 	path_uop_mix_max[i] += max_vector[i];
		// }

		++current_ROB_begin;
	}

	// don't want to construct a vector to return multiple results, hence, use values that were passed by reference to save the results
	AP = (double) sum_avg_paths / (ROB_size * outer_loops);
	ABP = (double) sum_branch_paths / branches;
	CP = (double) sum_crit_paths / outer_loops;
	// MDL = (double) max_dep_loads / outer_loops;

	// for(int i = 0; i < UOP_TYPE_INVALID; i++){
	// 	APM[i] = (double) path_uop_mix[i] / (ROB_size * outer_loops);
	// 	ABPM[i] = (double) branch_uop_mix[i] / branches;
	// 	CPM[i] = (double) path_uop_mix_max[i] / outer_loops;
	// }
}

void Dependence_Processor::clear_data_structures(){
	trace_uop_type.clear();
	trace.clear();
	rob_sizes.clear();
	avg_paths.clear();
	avg_branch_paths.clear();
	crit_paths.clear();
	// max_dependent_loads.clear();
	// avg_path_mix.clear();
	// avg_branch_path_mix.clear();
	// crit_path_mix.clear();
}
