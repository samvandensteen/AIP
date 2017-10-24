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

#ifndef DEFINE_INCLUDES_H_
#define DEFINE_INCLUDES_H_

#include <fstream>

// this is not multithread safe ofcourse
#define LOG_WARNING(message) do { \
								std::ofstream warning_file("warnings.log", std::ios_base::app); \
								warning_file << message << std::endl; \
								warning_file.close(); \
							 } while(0)

#define MOD(a,b) ((a % b) >= 0 ? a % b : (a % b) + b)
#define UNSIGNED_SUB(X, Y) (X < Y ? Y - X : X - Y)

// maximum number of load or stores in one instruction
#define MAX_LOAD 2
#define MAX_STORE 1
#define MAX_UOPS_PER_INS 3

#define UOPS_PER_INSTR 1.2

#define DETAILED_TIMING_ENABLED

// max file size in bytes: 1024*1024*1024 = 1GB
#define MAX_FILE_SIZE 1024*1024*1024

#endif
