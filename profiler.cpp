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

#include "profiler.h"

int Usage(){
	fprintf(stderr, "[PINTOOL] This tool analyzes the instruction flow by summarizing it in different profiles for branch, memory and instruction behaviour.\n");
	fprintf(stderr, "[PINTOOL] \n%s\n", KNOB_BASE::StringKnobSummary().c_str());
	return -1;
}

/* ===================================================================== */
// Phase analysis functions
/* ===================================================================== */
void track_phases(UINT32 ins_in_bbl){
	// makes most sense to count the instructions here because we won't switch to another level of instrumentation until the next basic block
	inscount += ins_in_bbl - memory_operations_counted;
	memory_operations_counted = 0;

	if(knobs->repeat && inscount >= finished_inscount){
		PIN_ExitApplication(1337);
	}

	// first state machine (switched between FASTFORWARD, WARMUP and DETAILED)
	if(inscount >= next_phase){
		if(curr_instr_mode == FASTFORWARD){
			#ifdef DETAILED_TIMING_ENABLED
				fastforward_time += clock() - start_time_fastforward;
			#endif

			f_inscount += inscount - start_ins_fastforward;

			if(knobs->warmup){
				#ifdef DETAILED_TIMING_ENABLED
					start_time_warmup = clock();
				#endif
				start_ins_warmup = inscount;

				next_phase += knobs->warmup;
				log_out << "[PINTOOL] Reached warmup phase at instruction " << inscount << ", warming up until instruction " << next_phase << endl;
				curr_instr_mode = WARMUP;
			}
			else if(knobs->detailed){
				#ifdef DETAILED_TIMING_ENABLED
					start_time_detailed = clock();
				#endif
				start_ins_detailed = inscount;

				next_phase += knobs->detailed;
				log_out << "[PINTOOL] Reached detailed phase at instruction " << inscount << ", instrumenting detailed until instruction " << next_phase << endl;
				log_out << "[PINTOOL] Window started at " << inscount << endl;

				curr_instr_mode = DETAILED_TRACE;
				next_trace = inscount + knobs->trace_window;
				utp->init_new_trace();
				trace_count++;

				if(knobs->pintool_log_level >= 2){
					log_out << "[PINTOOL] Trace " << trace_count << " reached at instruction " << inscount << endl;
				}
			}
			else{
				cerr << "[PINTOOL] ERROR -- Reached invalid state in the FASTFORWARD phase at inscount " << inscount << "!" << endl;
				log_out << "[PINTOOL] ERROR -- Reached invalid state in the FASTFORWARD phase at inscount " << inscount << "!" << endl;
				PIN_ExitProcess(-1);
			}
		}
		else if(curr_instr_mode == WARMUP){
			#ifdef DETAILED_TIMING_ENABLED
				warmup_time += clock() - start_time_warmup;
			#endif

			w_inscount += inscount - start_ins_warmup;

			if(knobs->detailed){
				#ifdef DETAILED_TIMING_ENABLED
					start_time_detailed = clock();
				#endif
				start_ins_detailed = inscount;

				next_phase += knobs->detailed;
				log_out << "[PINTOOL] Reached detailed phase at instruction " << inscount << ", instrumenting detailed until instruction " << next_phase << endl;
				log_out << "[PINTOOL] Window started at " << inscount << endl;

				curr_instr_mode = DETAILED_TRACE;
				next_trace = inscount + knobs->trace_window;
				utp->init_new_trace();
				trace_count++;

				if(knobs->pintool_log_level >= 2){
					log_out << "[PINTOOL] Trace " << trace_count << " reached at instruction " << inscount << endl;
				}
			}
			else{
				cerr << "[PINTOOL] ERROR -- Reached invalid state in the WARMUP phase at inscount " << inscount << "!" << endl;
				log_out << "[PINTOOL] ERROR -- Reached invalid state in the WARMUP phase at inscount " << inscount << "!" << endl;
				PIN_ExitProcess(-1);
			}
		}
		else if(curr_instr_mode == DETAILED){
			#ifdef DETAILED_TIMING_ENABLED
				detailed_time += clock() - start_time_detailed;
			#endif

			d_inscount += inscount - start_ins_detailed;

			st_ss->save_cold_miss_distribution(trace_count, inscount);

			bp->end_phase(trace_count);

			if(knobs->fastforward){
				log_out << "[PINTOOL] Window ended at " << inscount << endl;

				#ifdef DETAILED_TIMING_ENABLED
					start_time_fastforward = clock();
				#endif
				start_ins_fastforward = inscount;

				next_phase += knobs->fastforward;
				log_out << "[PINTOOL] Reached fastforward phase at instruction " << inscount << ", fasforwarding until instruction " << next_phase << endl;
				curr_instr_mode = FASTFORWARD;
			}
			else if(knobs->warmup){
				log_out << "[PINTOOL] Window ended at " << inscount << endl;

				#ifdef DETAILED_TIMING_ENABLED
					start_time_warmup = clock();
				#endif
				start_ins_warmup = inscount;

				next_phase += knobs->warmup;
				log_out << "[PINTOOL] Reached warmup phase at instruction " << inscount << ", warming up until instruction " << next_phase << endl;
				curr_instr_mode = WARMUP;
			}
			else{
				log_out << "[PINTOOL] Window ended at " << inscount << endl;

				#ifdef DETAILED_TIMING_ENABLED
					start_time_detailed = clock();
				#endif
				start_ins_detailed = inscount;

				next_phase += knobs->detailed;
				log_out << "[PINTOOL] Reached detailed phase at instruction " << inscount << ", instrumenting detailed until instruction " << next_phase << endl;
				log_out << "[PINTOOL] Window started at " << inscount << endl;

				curr_instr_mode = DETAILED_TRACE;
				next_trace = inscount + knobs->trace_window;
				utp->init_new_trace();
				trace_count++;

				if(knobs->pintool_log_level >= 2){
					log_out << "[PINTOOL] Trace " << trace_count << " reached at instruction " << inscount << endl;
				}
			}
		}
		else{
			cerr << "[PINTOOL] ERROR -- Reached invalid state in the DETAILED phase at inscount " << inscount << "!" << endl;
			log_out << "[PINTOOL] ERROR -- Reached invalid state in the DETAILED phase at inscount " << inscount << "!" << endl;
			PIN_ExitProcess(-1);
		}
	}

	// second state machine (switches between DETAILED and DETAILED_TRACE)
	if(curr_instr_mode == DETAILED && inscount >= next_trace){
		log_out << "[PINTOOL] Window ended at " << inscount << endl;

		curr_instr_mode = DETAILED_TRACE;
		next_trace += knobs->trace_window;

		st_ss->save_cold_miss_distribution(trace_count, inscount);

		bp->end_phase(trace_count);

		utp->init_new_trace();
		trace_count++;

		log_out << "[PINTOOL] Window started at " << inscount << endl;
	}
}

void count_instr(UINT32 ins_in_bbl){
	if(curr_instr_mode == FASTFORWARD){
		f_inscount += ins_in_bbl;
	}
	else if(curr_instr_mode == WARMUP){
		w_inscount += ins_in_bbl;
	}
	else if(curr_instr_mode == DETAILED || curr_instr_mode == DETAILED_TRACE){
		d_inscount += ins_in_bbl;
	}
}

instrument_mode_t get_current_instr_level(){
	return curr_instr_mode;
}

/* ===================================================================== */
// Analysis pass through functions
/* ===================================================================== */
void process_instr(ADDRINT ins_addr){
	bool trace_ended = utp->process_instr(ins_addr);
	if(trace_ended){
		// switch to detailed phase
		curr_instr_mode = DETAILED;
	}
}

void trace_branch(ADDRINT ip, BOOL* isTaken){
	bp->add_branch(ip, isTaken);
}

void trace_ins(ADDRINT ip, ADDRINT addr, THREADID threadid){
	int ret = st_ss->trace_ins(ip, addr, threadid, inscount);

	if (ret != 0) {
		cerr << "[PINTOOL] ERROR -- Invalid return code from trace_ins " <<endl;
		log_out << "[PINTOOL] ERROR -- Invalid return code from trace_ins " << ret << endl;
		PIN_ExitProcess(-1);
	}
}

void trace_mem_detailed(ADDRINT ip, ADDRINT addr, UINT32 op, THREADID threadid, UINT32 ref_type, instrument_mode_t bb_mode){
	if(ref_type == 0){
		utp->data_load(addr, op);
	}
	else if(ref_type == 1){
		utp->data_store(addr);
	}

	// we have to make sure the instrumentation mode we pass to statstack is the same as the actual one of the micro-trace (else there will be too many watchpoints)
	// we might switch to curr_instr_mode = DETAILED_TRACE at the start of a basic block, but the micro-trace will start the next basic block
	// likewise when a trace ends curr_instr_mode = DETAILED, but we might still be in a basic block with instrumentation mode DETAILED_TRACE
	int ret = 0;
	if(bb_mode == DETAILED_TRACE && curr_instr_mode == DETAILED){
		ret = st_ss->trace_mem_detailed(ip, addr, threadid, ref_type, inscount, DETAILED);
	}
	else{
		ret = st_ss->trace_mem_detailed(ip, addr, threadid, ref_type, inscount, bb_mode);
	}

	if (ret != 0) {
		cerr << "[PINTOOL] ERROR -- Invalid return code from trace_mem_detailed " << ret << endl;
		log_out << "[PINTOOL] ERROR -- Invalid return code from trace_mem_detailed " << ret << endl;
		PIN_ExitProcess(-1);
	}

	// for the DETAILED phases we approximate the instruction count by loads + stores to make sure the profiling of cold misses is approximately aligned with the instruction count
	// otherwise long basic blocks might contain to too many loads in one ROB (because ROB checking can only happen after basic block instruction count incrementation)
	inscount++;
	memory_operations_counted++;
}

void trace_mem_warmup(ADDRINT ip, ADDRINT addr, THREADID threadid, UINT32 ref_type){
	int ret = st_ss->trace_mem_warmup(ip, addr, threadid, ref_type, inscount);

	if (ret != 0) {
		cerr << "[PINTOOL] ERROR -- Invalid return code from trace_mem_warmup " << ret << endl;
		log_out << "[PINTOOL] ERROR -- Invalid return code from trace_mem_warmup " << ret << endl;
		PIN_ExitProcess(-1);
	}
}

void roi_function(){
	if(curr_instr_mode == FASTFORWARD || curr_instr_mode == WARMUP){
		fastforward_time += clock() - start_time_fastforward;
		#ifdef DETAILED_TIMING_ENABLED
			start_time_detailed = clock();
		#endif
		cout << "[PINTOOL] Start of ROI!" << endl;
		cout << "\tInstructions in fastforward mode: " << f_inscount << endl;
		cout << "\tInstructions in warmup mode: " <<  w_inscount << endl;
		cout << "\tInstructions in detailed mode: " << d_inscount << endl;
		curr_instr_mode = DETAILED;
	}
	else if(curr_instr_mode == DETAILED){
		detailed_time += clock() - start_time_detailed;
		#ifdef DETAILED_TIMING_ENABLED
			start_time_fastforward = clock();
		#endif
		cout << "[PINTOOL] End of ROI!" << endl;
		cout << "\tInstructions in fastforward mode: " << f_inscount << endl;
		cout << "\tInstructions in warmup mode: " <<  w_inscount << endl;
		cout << "\tInstructions in detailed mode: " << d_inscount << endl;
		curr_instr_mode = FASTFORWARD;
	}
}

/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */
void Trace(TRACE trace, void *v){
	instrument_mode_t current_mode = (instrument_mode_t) TRACE_Version(trace);

	BBL bbl_head = TRACE_BblHead(trace);
	INS ins_head = BBL_InsHead(bbl_head);

	// Make sure all version switches happen before any PinPlay instrumentation, to avoid PinPlay seeing some instructions twice
	CALL_ORDER call_order = (CALL_ORDER)(pinplay_engine.PinPlayFirstBeforeCallOrder() - 1);

	INS_InsertPredicatedCall(ins_head, IPOINT_BEFORE, (AFUNPTR)get_current_instr_level, IARG_RETURN_REGS, version_reg, IARG_CALL_ORDER, call_order, IARG_END);

	if(current_mode != FASTFORWARD){
		// first state machine
		INS_InsertVersionCase(ins_head, version_reg, 1, FASTFORWARD, IARG_CALL_ORDER, call_order, IARG_END);
	}
	if(current_mode != WARMUP){
		// first state machine
		INS_InsertVersionCase(ins_head, version_reg, 2, WARMUP, IARG_CALL_ORDER, call_order, IARG_END);
	}
	if(current_mode != DETAILED){
		// first state machine
		INS_InsertVersionCase(ins_head, version_reg, 3, DETAILED, IARG_CALL_ORDER, call_order, IARG_END);
	}
	if(current_mode != DETAILED_TRACE){
		// second state machine
		INS_InsertVersionCase(ins_head, version_reg, 4, DETAILED_TRACE, IARG_CALL_ORDER, call_order, IARG_END);
	}
	// Version 0 is only for startup / amnesia, don't do anything else there
	if (TRACE_Version(trace) == INVALID){
		return;
	}

	for(BBL bbl = bbl_head; BBL_Valid(bbl); bbl = BBL_Next(bbl)){
		if(instruction_based){
			BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR)track_phases, IARG_UINT32, BBL_NumIns(bbl), IARG_ADDRINT, BBL_Address(bbl), IARG_END);
		}
		else if(roi_based){
			BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR)count_instr, IARG_UINT32, BBL_NumIns(bbl), IARG_ADDRINT, BBL_Address(bbl), IARG_END);
			for(INS ins = BBL_InsHead(bbl); ; ins = INS_Next(ins)){
				if (INS_IsXchg(ins) && INS_OperandReg(ins, 0) == REG_BX && INS_OperandReg(ins, 1) == REG_BX){
					INS_InsertPredicatedCall(ins, IPOINT_AFTER, (AFUNPTR)roi_function, IARG_END);
				}
				if(ins == BBL_InsTail(bbl)){
					break;
				}
			}
		}
		else{
			log_out << "[PINTOOL] Switching instrumentation level either on ROI or instruction count!" << endl;
			PIN_ExitProcess(-1);
		}
		switch(current_mode){
			case FASTFORWARD:
				break;
			case WARMUP:
				for(INS ins = BBL_InsHead(bbl); ; ins = INS_Next(ins)){
					// sampler_i: analyze instructions
					INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_ins, IARG_INST_PTR, IARG_INST_PTR, IARG_THREAD_ID, IARG_END);

					// sampler_d: analyze memory instructions
					no_ops = INS_MemoryOperandCount(ins);
					for(UINT32 op = 0; op < no_ops; op++){
						const bool is_rd = INS_MemoryOperandIsRead(ins, op);
						const bool is_wr = INS_MemoryOperandIsWritten(ins, op);

						if(is_rd){
							INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_mem_warmup, IARG_INST_PTR, IARG_MEMORYOP_EA, op, IARG_UINT32, IARG_THREAD_ID, IARG_UINT32, 0, IARG_END);
						}
						if(is_wr){
							INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_mem_warmup, IARG_INST_PTR, IARG_MEMORYOP_EA, op, IARG_UINT32, IARG_THREAD_ID, IARG_UINT32, 1, IARG_END);
						}
					}

					if(ins == BBL_InsTail(bbl)){
						break;
					}
				}
				break;
			case DETAILED:
				for(INS ins = BBL_InsHead(bbl); ; ins = INS_Next(ins)){
					// sampler_i: analyze instructions
					INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_ins, IARG_INST_PTR, IARG_INST_PTR, IARG_THREAD_ID, IARG_END);

					// sampler_d: analyze memory instructions
					no_ops = INS_MemoryOperandCount(ins);
					for(UINT32 op = 0; op < no_ops; op++){
						const bool is_rd = INS_MemoryOperandIsRead(ins, op);
						const bool is_wr = INS_MemoryOperandIsWritten(ins, op);

						if(is_rd){
							INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_mem_detailed, IARG_INST_PTR, IARG_MEMORYOP_EA, op, IARG_UINT32, op, IARG_UINT32, IARG_THREAD_ID, IARG_UINT32, 0, IARG_UINT32, current_mode, IARG_END);
						}
						if(is_wr){
							INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_mem_detailed, IARG_INST_PTR, IARG_MEMORYOP_EA, op, IARG_UINT32, op, IARG_UINT32, IARG_THREAD_ID, IARG_UINT32, 1, IARG_UINT32, current_mode, IARG_END);
						}
					}

					// sample all branches, this appears to be pretty slow
					if(INS_IsBranch(ins) && INS_HasFallThrough(ins)){
						// Insert a call to printip before every instruction, and pass it the IP
						INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_branch, IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END);
					}

					if(ins == BBL_InsTail(bbl)){
						break;
					}
				}
				break;
			case DETAILED_TRACE:
				for(INS ins = BBL_InsHead(bbl); ; ins = INS_Next(ins)){
					// decode instructions into xed-format and uops
					utrace_processor::add_instr(INS_Address(ins), InstructionDecoder::decode(INS_Address(ins), INS_XedDec(ins), NULL));

					// sampler_i: analyze instructions
					INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_ins, IARG_INST_PTR, IARG_INST_PTR, IARG_THREAD_ID, IARG_END);

					// sampler_d: analyze memory instructions
					no_ops = INS_MemoryOperandCount(ins);
					for(UINT32 op = 0; op < no_ops; op++){
						const bool is_rd = INS_MemoryOperandIsRead(ins, op);
						const bool is_wr = INS_MemoryOperandIsWritten(ins, op);

						if(is_rd){
							INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_mem_detailed, IARG_INST_PTR, IARG_MEMORYOP_EA, op, IARG_UINT32, op, IARG_UINT32, IARG_THREAD_ID, IARG_UINT32, 0, IARG_UINT32, current_mode, IARG_END);
						}
						if(is_wr){
							INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_mem_detailed, IARG_INST_PTR, IARG_MEMORYOP_EA, op, IARG_UINT32, op, IARG_UINT32, IARG_THREAD_ID, IARG_UINT32, 1, IARG_UINT32, current_mode, IARG_END);
						}
					}

					// sample all branches, this appears to be pretty slow
					if(INS_IsBranch(ins) && INS_HasFallThrough(ins)){
						// Insert a call to printip before every instruction, and pass it the IP
						INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)trace_branch, IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END);
					}

					// analyze instruction flow in utrace
					INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)process_instr, IARG_INST_PTR, IARG_END);

					if(ins == BBL_InsTail(bbl)){
						break;
					}
				}
				break;
			default:
				assert(0);
				break;
		}
	}
}

// Call when done or interrupted
void Fini(INT32 code, void *v){
	stop = clock();

	// if pintool exits due to the limit we imposed on number of instructions to instrument, we signal this with code == 1
	// however, we still want to be able to catch application error codes also (if they happen before the instruction limit we imposed)
	// add last timing and inscount results
	if(curr_instr_mode == FASTFORWARD){
		#ifdef DETAILED_TIMING_ENABLED
			fastforward_time += clock() - start_time_fastforward;
		#endif

		f_inscount += inscount - start_ins_fastforward;
	}
	else if(curr_instr_mode == WARMUP){
		#ifdef DETAILED_TIMING_ENABLED
			warmup_time += clock() - start_time_warmup;
		#endif

		w_inscount += inscount - start_ins_warmup;
	}
	else if(curr_instr_mode == DETAILED || curr_instr_mode == DETAILED_TRACE){
		log_out << "[PINTOOL] Window ended at " << inscount << endl;

		st_ss->save_cold_miss_distribution(trace_count, inscount);

		bp->end_phase(trace_count);

		#ifdef DETAILED_TIMING_ENABLED
			detailed_time += clock() - start_time_detailed;
		#endif

		d_inscount += inscount - start_ins_detailed;
	}

	if(curr_instr_mode == DETAILED_TRACE){
		log_out << "[PINTOOL] Incomplete trace preempted (no output generated for this trace)!" << endl;
	}

	if(knobs->repeat){
		if(code == 1337){
			log_out << "[PINTOOL] Earlier end of profiling due to -repeat constraint!" << endl;
		}
	}
	else{
		if(code){
			log_out << "Application returned an invalid exit code [" << code << "], aborting." << std::endl;
		}
	}

	log_out << "[PINTOOL] End of benchmark execution!" << endl;

	log_out << endl << "Profiling speed: " << (double) inscount * CLOCKS_PER_SEC / (stop - start) / 1000000 << " MIPS" << endl;
	#ifdef DETAILED_TIMING_ENABLED
		if(f_inscount){
			log_out << "\tFastforward speed (" << f_inscount << " instr): " << (double) f_inscount * CLOCKS_PER_SEC / fastforward_time / 1000000 << " MIPS" << endl;
		}
		if(w_inscount){
			log_out << "\tWarmup speed (" << w_inscount << " instr): " << (double) w_inscount * CLOCKS_PER_SEC / warmup_time / 1000000 << " MIPS" << endl;
		}
		if(d_inscount){
			log_out << "\tDetailed speed (" << d_inscount << " instr): " << (double) d_inscount * CLOCKS_PER_SEC / detailed_time / 1000000 << " MIPS" << endl;
		}
	#endif

	log_out << endl << "Time taken to measure: " << (double) (stop - start) / CLOCKS_PER_SEC << " s" << endl;

	log_out.close();

	delete utp;
	delete st_ss;
	delete bp;
}

/*!
 * The main procedure of the tool. Initialize and fire up Pin.
 * @param[in]   argc			total number of elements in the argv array
 * @param[in]   argv			array of command line arguments, including pin -t <toolname> -- ...
 */
int main(int argc, char * argv[]){
	start = clock();

	PIN_InitSymbols();
	if (PIN_Init(argc, argv)){
		return Usage();
	}

	pinplay_engine.Activate(argc, argv, KnobPinPlayLogger, KnobPinPlayReplayer);

	// KNOBS
	knobs = new Knobs();

	knobs->parse_rob_string(knob_rob);
	knobs->approx_dep = (bool) knob_approx_dep;

	knobs->output_dir = knob_output;

	knobs->repeat = knob_repeat;
	knobs->fastforward = knob_fastforward;
	knobs->warmup = knob_warmup;
	knobs->detailed = knob_detailed;

	knobs->roi = knob_roi;

	knobs->trace_ins_length = knob_trace_ins_length;
	knobs->trace_mem_length = knob_trace_mem_length;
	knobs->trace_window = knob_trace_window;
	knobs->pintool_log_level = knob_pintool_log_level;

	knobs->enable_compression = knob_compression;

	knobs->max_bhr = knob_max_bhr;
	knobs->max_ip = knob_max_ipbits_bht;

	knobs->sample_period = knob_smp_period;
	knobs->smp_base = knob_smp_base.Value();
	knobs->burst_period = knob_burst_period;
	knobs->burst_size = knob_burst_size;
	knobs->smp_line_size_lg2 = knob_smp_line_size_lg2;
	knobs->burst_rnd = knob_burst_rnd.Value();
	knobs->smp_rnd = knob_smp_rnd.Value();
	knobs->seed = knob_seed;
	knobs->log_level = knob_statstack_log_level;

	inscount = 0, memory_operations_counted = 0;
	finished_inscount = knobs->repeat * (knobs->fastforward + knobs->warmup + knobs->detailed);
	start_ins_fastforward = 0, start_ins_warmup = 0, start_ins_detailed = 0;
	next_phase = 0, next_trace = 0, trace_count = 0;
	fastforward_time = 0, warmup_time = 0, detailed_time = 0;
	f_inscount = 0, w_inscount = 0, d_inscount = 0;

	instruction_based = false, roi_based = false;
	if(knobs->roi == "fastforward"){
		roi_based = true;
		curr_instr_mode = FASTFORWARD;
	}
	else if(knobs->roi == "warmup"){
		roi_based = true;
		curr_instr_mode = WARMUP;
	}
	else{
		instruction_based = true;
	}

	// create directory if it doesn't exist yet
	if(knobs->output_dir.length() > 0){
		struct stat st = {0};
		if(stat(knobs->output_dir.c_str(), &st) == -1) {
			mkdir(knobs->output_dir.c_str(), 0700);
		}
		stringstream ss;
		ss << "rsync -a . " << knobs->output_dir << " --exclude " << knobs->output_dir;
		int copy_all_files = system(ss.str().c_str());
		int chdir_to_output = chdir(knobs->output_dir.c_str());
		log_out.open("log.out");
		if(copy_all_files == -1){
			// always print errors, no matter the log level
			cerr << "[PINTOOL] ERROR -- Could not copy all files to directory specified with -output!" << endl;
			log_out << "[PINTOOL] ERROR -- Could not copy all files to directory specified with -output!" << endl;
			exit(1);

		}
		if(chdir_to_output == -1){
			// always print errors, no matter the log level
			cerr << "[PINTOOL] ERROR -- Could not change to directory specified with -output!" << endl;
			log_out << "[PINTOOL] ERROR -- Could not change to directory specified with -output!" << endl;
			exit(1);
		}
	}
	else{
		log_out.open("log.out");
	}
	//Print logfile
	log_out << knobs->get_log_header();
	log_out.flush();

	// WARNINGS
	if(knobs->trace_ins_length > 25000){
		// always print warnings, no matter the log level
		log_out << "[PINTOOL] WARNING -- Profiling traces longer than 25k instructions will take quite some time and memory!" << endl;
	}
	if(knobs->trace_mem_length > 10000){
		// always print warnings, no matter the log level
		log_out << "[PINTOOL] WARNING -- Profiling traces longer than 10k memory operations will take quite some time and memory!" << endl;
	}

	utp = new utrace_processor(knobs);
	st_ss = new ST_statstack(knobs);
	bp = new branch_profiler(knobs);

	log_out << "[PINTOOL] Start of benchmark execution!" << endl;

	// init first phase variables correctly
	if(knobs->fastforward){
		#ifdef DETAILED_TIMING_ENABLED
			start_time_fastforward = clock();
		#endif

		curr_instr_mode = FASTFORWARD;
		next_phase += knobs->fastforward;

		if(instruction_based){
			log_out << "[PINTOOL] Reached fastforward phase at instruction " << inscount << ", fasforwarding until instruction " << next_phase << endl;
		}
	}
	else if(knobs->warmup){
		#ifdef DETAILED_TIMING_ENABLED
			start_time_warmup = clock();
		#endif

		curr_instr_mode = WARMUP;
		next_phase += knobs->warmup;

		if(instruction_based){
			log_out << "[PINTOOL] Reached warmup phase at instruction " << inscount << ", warming up until instruction " << next_phase << endl;
		}
	}
	else if(knobs->detailed){
		#ifdef DETAILED_TIMING_ENABLED
			start_time_detailed = clock();
		#endif
		start_ins_detailed = inscount;

		next_phase += knobs->detailed;
		if(instruction_based){
			log_out << "[PINTOOL] Reached detailed phase at instruction " << inscount << ", instrumenting detailed until instruction " << next_phase << endl;
			log_out << "[PINTOOL] Window started at " << inscount << endl;
		}

		curr_instr_mode = DETAILED_TRACE;
		next_trace = inscount + knobs->trace_window;
		utp->init_new_trace();

		if(knobs->pintool_log_level >= 2){
			log_out << "[PINTOOL] Trace " << trace_count << " reached at instruction " << inscount << endl;
		}

		trace_count++;
	}
	else{
		cerr << "[PINTOOL] ERROR -- Reached invalid state for fastforward-warmup-detailed phases! Check that at least one of the phase lengths is different from 0." << endl;
		log_out << "[PINTOOL] ERROR -- Reached invalid state for fastforward-warmup-detailed phases! Check that at least one of the phase lengths is different from 0." << endl;
		exit(1);
	}

	version_reg = PIN_ClaimToolRegister();

	// Register Instruction to be called to instrument instructions
	TRACE_AddInstrumentFunction(Trace, 0);

	PIN_AddFiniFunction(Fini, 0); // exit function

	PIN_StartProgram(); // Start the program (never returns!)
}
