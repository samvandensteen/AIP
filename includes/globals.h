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

#ifndef GLOBALS_H
#define GLOBALS_H

// pin includes
#include "pin.H"
#include "pinplay.H"
#include "instlib.H"
extern "C" {
	#include "xed-interface.h"
}

typedef enum{
	INVALID = 0,
	FASTFORWARD,
	WARMUP,
	DETAILED,
	DETAILED_TRACE
} instrument_mode_t;

/* ===================================================================== */
// Pinplay knobs and engine
/* ===================================================================== */

extern PINPLAY_ENGINE pinplay_engine;
extern KNOB<BOOL> KnobPinPlayLogger;
extern KNOB<BOOL> KnobPinPlayReplayer;

/* ===================================================================== */
// Command line switches pintool
/* ===================================================================== */

// general knobs
extern KNOB<UINT32> knob_debug;
extern KNOB<string> knob_output;
extern KNOB<UINT32> knob_pintool_log_level;

extern KNOB<UINT64> knob_fastforward;
extern KNOB<UINT64> knob_warmup;
extern KNOB<UINT64> knob_detailed;
extern KNOB<UINT32> knob_repeat;

extern KNOB<string> knob_roi;

// trace knobs
extern KNOB<UINT32> knob_trace_ins_length;
extern KNOB<UINT32> knob_trace_mem_length;
extern KNOB<UINT32> knob_trace_window;

// ROB knobs
extern KNOB<string> knob_rob;
extern KNOB<BOOL> knob_approx_dep;

// branch knobs
extern KNOB<UINT32> knob_max_bhr;
extern KNOB<UINT32> knob_max_ipbits_bht;

extern KNOB<BOOL> knob_compression;

//statstack knobs
extern KNOB<string> knob_smp_base;
extern KNOB<UINT32> knob_smp_period;
extern KNOB<string> knob_smp_rnd;
extern KNOB<UINT32> knob_burst_period;
extern KNOB<string> knob_burst_rnd;
extern KNOB<UINT32> knob_burst_size;
extern KNOB<UINT32> knob_smp_line_size_lg2;
extern KNOB<UINT32> knob_seed;
extern KNOB<UINT32> knob_statstack_log_level;

#endif
