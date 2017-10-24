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

#include "knobs.h"
#include "extended_uop_type_enum.h"

void Knobs::parse_rob_string(std::string rbs){
	int first_dash = rbs.find("-"), second_dash = rbs.rfind("-");
	min_rob = atoi(rbs.substr(0, first_dash).c_str());
	step_rob = atoi(rbs.substr(first_dash + 1, second_dash - first_dash - 1).c_str());
	max_rob = atoi(rbs.substr(second_dash + 1).c_str());
}

string Knobs::generate_uop_string(){
	stringstream ss;
	for(int i = 0; i < UOP_TYPE_INVALID; i++){
		ss << uop_to_string(static_cast<extended_uop_type>(i));
		if(i < UOP_TYPE_INVALID - 1){
			ss << ",";
		}
	}
	return ss.str();
}

string get_time(){
	time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );

	stringstream ss;
	ss << "Date: " << (now->tm_year + 1900) << "/";
	if(now->tm_mon + 1 < 10){
		ss << "0";
	}
	ss << (now->tm_mon + 1) << "/";
	if(now->tm_mday < 10){
		ss << "0";
	}
	ss << now->tm_mday << "_";
	if(now->tm_hour < 10){
		ss << "0";
	}
	ss << now->tm_hour << ":";
	if(now->tm_min < 10){
		ss << "0";
	}
	ss << now->tm_min << ":";
	if(now->tm_sec < 10){
		ss << "0";
	}
	ss << now->tm_sec << endl;

	return ss.str();
}

string Knobs::get_log_header(){

	stringstream log_header;
	log_header << "--BEGIN METADATA--" << endl;
	log_header << "VERSION 2.0" << endl;
	log_header << get_time() << endl;

	log_header << "General configuration:" << endl;
	log_header << "\tfastforward " << fastforward << endl;
	log_header << "\twarmup " << warmup << endl;
	log_header << "\tdetailed " << detailed << endl;
	log_header << "\trepeat " << repeat << endl;
	log_header << "\tinstrumentation_switching ";
	if(roi == ""){
		log_header << "\tinstruction_count" << endl;
	}
	else{
		log_header << "\troi_based_" << roi << endl;
	}
	log_header << "\tused_uops " << generate_uop_string() << std::endl;
	log_header << "\tenable_compression " << enable_compression << endl;

	log_header << "Trace configuration:" << endl;
	log_header << "\ttrace_ins_length " << trace_ins_length << endl;
	log_header << "\ttrace_mem_length " << trace_mem_length << endl;
	log_header << "\ttrace_window " << trace_window << endl;

	log_header << "StatStack configuration:" << endl;
	log_header << "\tO " << smp_base << endl;
	log_header << "\tp " << sample_period << endl;
	log_header << "\tr " << smp_rnd << endl;
	log_header << "\tP " << burst_period << endl;
	log_header << "\tR " << burst_rnd << endl;
	log_header << "\tb " << burst_size << endl;
	log_header << "\tl " << smp_line_size_lg2 << endl;
	log_header << "\ts " << seed << endl;
	log_header << "\tv " << log_level << endl;
	log_header << "--END METADATA--" << endl;
	log_header << endl;

	return log_header.str();
}
