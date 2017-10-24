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
*           Moncef Mechri, Uppsala University
*  Copyright: 2016, Ghent University and Moncef Mechri
*/

#ifndef SAMPLER_H
#define SAMPLER_H

#include <cstdint>
#include <cstdlib>
#include <string>
#include <iostream>
#include <climits>
#include <utility>
#include <vector>
#include <algorithm>
#include <random>
#include <map>
#ifdef USE_STL
	#include <unordered_map>
#else
	#include <sparsehash/dense_hash_map>
#endif

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "memory_file.pb.h"
#include "memory_file_util.h"
#include "proto_writer.h"
#include "knobs.h"

#define HASH_MAP_SIZE 512

using std::size_t;
using std::string;
using std::ios_base;
using std::ofstream;
#ifdef USE_STL
	using std::unordered_map;
#else
	using google::dense_hash_map;
#endif
using std::vector;
using std::sort;
using std::pair;
using std::make_pair;
using std::default_random_engine;
using std::uniform_real_distribution;
using std::map;
using std::max;

using google::protobuf::io::OstreamOutputStream;

using memory_file::Access_Type;
using memory_file::Memory_Access;
using memory_file::Sample;
using memory_file::Burst_Profile;
using memory_file::Burst;

struct HashFunction{
	size_t operator()(const uint64_t &k) const{
		// http://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
		uint32_t hash = ((k >> 16) ^ k) * 0x45d9f3b;
		hash = ((hash >> 16) ^ hash) * 0x45d9f3b;
	  hash = ((hash >> 16) ^ hash);
		return hash;
  }
};

struct begin_less_than{
	inline bool operator()(Sample *i, Sample *j){
		return (i->begin().burst_id() < j->begin().burst_id());
	}
};

struct end_less_than{
	inline bool operator()(Sample *i, Sample *j){
		return (i->end().burst_id() < j->end().burst_id());
	}
};

class sampler{
	private:
		string base_file_name;
		Proto_Writer *sample_writer, *ooo_writer, *dangling_writer;

		// saves all arguments related to statstack
		uint32_t thread_id;
		bool burst_rnd, sample_rnd;
		uint64_t burst_period, sample_period;
		uint64_t burst_ins_size, burst_mem_size;
		unsigned short line_size_lg2;

    // counting variables
		bool log_watchpoint, in_burst;
    uint64_t burst_counter, burst_written;
		uint64_t last_inscount, last_mem_count;
    uint64_t mem_burst_begin, mem_burst_end;
    uint64_t ins_burst_begin, ins_burst_end;
    uint64_t next_watchpoint;
		uint64_t takeoff_loads, landing_loads, takeoff_stores, landing_stores;

		map<uint64_t, pair<uint32_t, uint32_t>> dangling_load_stores;

		vector<Sample*> ooo_samples;
		uint64_t vector_size;
		bool compressed;

		bool is_ins_sampler, is_mem_sampler; // statstack sampler samples memory accesses or saves memory accesses in a trace, depending on this value

		// random number generator, use this as an instantiation in the class to always get the same random numbers no matter how many samplers we have
    default_random_engine generator;
    uniform_real_distribution<double> distribution;

		Burst_Profile *bp;
		Burst *curr_burst;

    // map to keep watchpoints
		#ifdef USE_STL
    	unordered_map<uint64_t, Memory_Access*> watchpoints;
		#else
			#ifdef HASH
				dense_hash_map<uint64_t, Memory_Access*, HashFunction> watchpoints;
			#else
				dense_hash_map<uint64_t, Memory_Access*> watchpoints;
			#endif
		#endif

    // functions
    void start_burst(uint64_t inscount, uint64_t memcount);
	  void end_burst(uint64_t inscount, uint64_t memcount);

		void dump_ooo_samples(bool final);
		void dump_dangling_samples();

		void check_ooo_vector_size(Sample *ooo_sample);

    void check_memory_access(uint64_t addr, uint64_t access_counter, Access_Type at, uint16_t tid, uint64_t ip);
    void log_memory_access(Memory_Access *ma);

		uint64_t rnd_exp(uint64_t period);

  public:
    sampler(string base_file_name, Knobs *k, uint32_t threadid, bool mem_based, bool ins_based);
		~sampler();

    void sample_memory_access(uint64_t addr, uint64_t access_counter, Access_Type at, uint16_t tid, uint64_t ip, uint64_t inscount);

    void in_trace();
};

#endif /* SAMPLER_H */
