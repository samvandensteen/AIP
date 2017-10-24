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

#include "globals.h"

/* ===================================================================== */
// Pinplay knobs and engine
/* ===================================================================== */

PINPLAY_ENGINE pinplay_engine;
KNOB<BOOL> KnobPinPlayLogger(KNOB_MODE_WRITEONCE, "pintool", "log", "0", "Activate the pinplay logger");
KNOB<BOOL> KnobPinPlayReplayer(KNOB_MODE_WRITEONCE, "pintool", "replay", "0", "Activate the pinplay replayer");

/* ===================================================================== */
// Command line switches pintool
/* ===================================================================== */

// general knobs
KNOB<UINT32> knob_debug(KNOB_MODE_WRITEONCE, "pintool", "debug", "0", "enable debugging");
KNOB<string> knob_output(KNOB_MODE_WRITEONCE, "pintool", "output", "", "set output directory (relative to current directory or absolute path)");
KNOB<UINT32> knob_pintool_log_level(KNOB_MODE_WRITEONCE, "pintool", "tool_log", "1", "Define log level, 0 = only errors and warnings, 1 = phase information, 2 = trace information");

KNOB<UINT64> knob_fastforward(KNOB_MODE_WRITEONCE, "pintool", "ff", "800000000", "Number of instructions to fastforward");
KNOB<UINT64> knob_warmup(KNOB_MODE_WRITEONCE, "pintool", "w", "100000000", "Number of instructions to warmup");
KNOB<UINT64> knob_detailed(KNOB_MODE_WRITEONCE, "pintool", "d", "100000000", "Number of instructions for detailed analysis");
KNOB<UINT32> knob_repeat(KNOB_MODE_WRITEONCE, "pintool", "repeat", "0", "Number of iterations to repeat the phases fastforward-warmup-detailed (0: until the end of the program)");

KNOB<string> knob_roi(KNOB_MODE_WRITEONCE, "pintool", "roi", "", "Set instrumentation mode before roi (fastforward / warmup).");

// trace knobs
KNOB<UINT32> knob_trace_ins_length(KNOB_MODE_WRITEONCE, "pintool", "trace_ins_length", "1000", "Minimum length of one trace in instructions");
KNOB<UINT32> knob_trace_mem_length(KNOB_MODE_WRITEONCE, "pintool", "trace_mem_length", "250", "Minimum length of one trace in memory operations");
KNOB<UINT32> knob_trace_window(KNOB_MODE_WRITEONCE, "pintool", "trace_window", "1000000", "Length of one window wherein traces are taken");

// ROB knobs
KNOB<string> knob_rob(KNOB_MODE_WRITEONCE, "pintool", "rob", "32-16-256", "Minimum ROB size, maximum ROB size and steps inbetween for which to calculate dependences");
KNOB<BOOL>   knob_approx_dep(KNOB_MODE_WRITEONCE, "pintool", "approx_dep", "0", "Choose whether you want to calculate approximate dependence paths (1) or not (0)");

// branch knobs
KNOB<UINT32> knob_max_bhr(KNOB_MODE_WRITEONCE, "pintool", "max_bhr", "10", "Maximum BHR length");
KNOB<UINT32> knob_max_ipbits_bht(KNOB_MODE_WRITEONCE, "pintool", "max_ip", "10", "Maximum number of bits taken from the Instruction Pointer to index the BHT");

// use compression in protobuf files
KNOB<BOOL> knob_compression(KNOB_MODE_WRITEONCE, "pintool", "compression", "1", "Use compression when writing the protobuf files (~1.5x slowdown, ~5x less data)");

//statstack knobs
KNOB<string> knob_smp_base(KNOB_MODE_WRITEONCE, "pintool", "O", "sample", "Output file base name.");
KNOB<UINT32> knob_smp_period(KNOB_MODE_WRITEONCE, "pintool", "p", "1000", "Cache: Average time (in memory accesses) before taking a new sample within a window");
KNOB<string> knob_smp_rnd(KNOB_MODE_WRITEONCE, "pintool", "r", "exp", "Random generator exp/const");
KNOB<UINT32> knob_burst_period(KNOB_MODE_WRITEONCE, "pintool", "P", "0", "Cache: Average time (in memory accesses) before starting a new window");
KNOB<string> knob_burst_rnd(KNOB_MODE_WRITEONCE, "pintool", "R", "const", "Random generator exp/const");
KNOB<UINT32> knob_burst_size(KNOB_MODE_WRITEONCE, "pintool", "b", "600000", "Cache: Average size (in memory accesses) of a window");
KNOB<UINT32> knob_smp_line_size_lg2(KNOB_MODE_WRITEONCE, "pintool", "l", "6", "Line size (log 2)");
KNOB<UINT32> knob_seed(KNOB_MODE_WRITEONCE, "pintool", "s", "0", "Random seed");
KNOB<UINT32> knob_statstack_log_level(KNOB_MODE_WRITEONCE, "pintool", "v", "0", "Log level");
