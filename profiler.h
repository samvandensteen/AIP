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

#include "instruction_decoder.h"
#include "pin_includes.h"
#include "define_includes.h"

//Include all super classes
#include "utrace_processor.h"
#include "branch_profiler.h"
#include "st_statstack.h"

//include all help classes
#include "knobs.h"

Knobs *knobs;
// single thread applications:
utrace_processor *utp;
branch_profiler *bp;
ST_statstack *st_ss;

/* ===================================================================== */
// VARIABLES
/* ===================================================================== */
uint64_t inscount, memory_operations_counted, finished_inscount;
uint64_t start_ins_fastforward, start_ins_warmup, start_ins_detailed;
uint64_t next_phase, next_trace, trace_count;
uint64_t fastforward_time, warmup_time, detailed_time;
uint64_t f_inscount, w_inscount, d_inscount;

// versioning
static REG version_reg;
instrument_mode_t curr_instr_mode;
uint32_t no_ops;

bool instruction_based, roi_based;

// timing
clock_t start, stop;
clock_t start_time_fastforward, start_time_warmup, start_time_detailed, stop_time_phase;

//logging
ofstream log_out;
