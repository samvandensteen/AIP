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

#include "memory_file_util.h"

Memory_Access* make_memory_access(uint64_t burst_id, uint64_t addr, uint64_t access_counter, Access_Type access_type, uint32_t thread_id, uint64_t program_counter){
	Memory_Access *ma = new Memory_Access();

  ma->set_burst_id(burst_id);
  ma->set_memory_address(addr);
  ma->set_access_counter(access_counter);
  ma->set_access_type(access_type);
  ma->set_thread_id(thread_id);
  ma->set_program_counter(program_counter);

	return ma;
}

Sample* make_sample(Memory_Access *begin, Memory_Access *end){
	Sample *sample = new Sample();

	sample->set_allocated_begin(begin);
	sample->set_allocated_end(end);

	return sample;
}

Sample* make_dangling_sample(Memory_Access *begin){
	Sample *sample = new Sample();

	sample->set_allocated_begin(begin);

	return sample;
}

void print_sample(Sample *sample){
	const bool has_end = sample->has_end();
	if(has_end){
		cout << "==Sample== ";
	}
	else{
		cout << "==Dangling== ";
	}

	cout << "MA_1: ";
	print_memory_access(sample->begin());

	if(has_end){
		cout << " -- MA_2: ";
		print_memory_access(sample->end());
	}
	cout << endl;
}

void print_memory_access(const Memory_Access& access){
	cout << "burst: " << access.burst_id();
	cout << " time: " << access.access_counter();
	cout << " PC: " << access.program_counter();
	cout << " address: " << access.memory_address();
	if(access.access_type() == 0){
		cout << " type: " << access.access_type() << " (L)";
	}
	else if(access.access_type() == 1){
		cout << " type: " << access.access_type() << " (S)";
	}
	else if(access.access_type() == 3){
		cout << " type: " << access.access_type() << " (I)";
	}
	cout << " tid: " << access.thread_id();
}

void modify_burst(uint64_t burst_id, uint32_t dangling_loads, uint32_t dangling_stores, Burst_Profile *bp){
	Burst* modify = bp->mutable_bursts(burst_id);
	modify->set_landing_loads(modify->landing_loads() + dangling_loads);
	modify->set_landing_stores(modify->landing_stores() + dangling_stores);
}

Burst* add_burst(Burst_Profile *bp, uint64_t burst_counter, uint64_t inscount, uint64_t memcount){
	Burst *b = bp->add_bursts();

	b->set_burst_id(burst_counter);
	b->set_instr_begin(inscount);
	b->set_memaccess_begin(memcount);

	return b;
}

void complete_burst(Burst *b, uint64_t inscount, uint64_t memcount, uint32_t takeoff_loads, uint32_t landing_loads, uint32_t takeoff_stores, uint32_t landing_stores){
	b->set_instr_end(inscount);
	b->set_memaccess_end(memcount);

	b->set_takeoff_loads(takeoff_loads);
	b->set_landing_loads(landing_loads);
	b->set_takeoff_stores(takeoff_stores);
	b->set_landing_stores(landing_stores);
}

void write_burst_profile(string filename, Burst_Profile *bp){
	ofstream of(filename);
  OstreamOutputStream* oos = new OstreamOutputStream(&of);
	// this file is too small to compress
  bp->SerializeToOstream(&of);
  delete oos;
  of.close();
  delete bp;
}

Burst_Profile* read_burst_profile(string filename){
	ifstream input(filename, ios::binary);
	Burst_Profile *b = new Burst_Profile();
	b->ParseFromIstream(&input);
	return b;
}

void print_burst_profile(Burst_Profile *bp){
	cout << "Profile line size: " << pow(2, bp->line_size()) << endl;
	for(int i = 0; i < bp->bursts_size(); i++){
		Burst cb = bp->bursts(i);
		cout << "ID: " << cb.burst_id() << ", MAB: " << cb.memaccess_begin() << ", MAE: " << cb.memaccess_end() << ", IB: " << cb.instr_begin() << ", IE: " << cb.instr_end() << ", TL: " << cb.takeoff_loads() << ", TS: " << cb.takeoff_stores() << ", LL: " << cb.landing_loads() << ", LS: " << cb.landing_stores() << endl;
	}
}
