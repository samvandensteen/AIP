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

#include "sampler.h"

sampler::sampler(string base_file_name, Knobs *k, uint32_t thread_id, bool mem_based, bool ins_based){
  this->base_file_name = base_file_name;
  sample_writer = new Proto_Writer("sample_" + this->base_file_name, k->enable_compression);
  ooo_writer = new Proto_Writer("ooo_" + this->base_file_name, k->enable_compression);
  dangling_writer = new Proto_Writer("dangling_" + this->base_file_name, k->enable_compression);

  this->thread_id = thread_id;
  if(k->burst_rnd == "exp"){
    burst_rnd = true;
  }
  else{
    burst_rnd = false;
  }
  if(k->smp_rnd == "exp"){
    sample_rnd = true;
  }
  else{
    sample_rnd = false;
  }
  burst_period = k->burst_period;
  sample_period = k->sample_period;
  burst_ins_size = k->trace_window;
  burst_mem_size = k->burst_size;
  line_size_lg2 = k->smp_line_size_lg2;

  log_watchpoint = false, in_burst = false;
  burst_counter = 0, burst_written = 0;
  last_inscount = 0, last_mem_count = 0;
  mem_burst_begin = 0, mem_burst_end = burst_mem_size;
  if(is_mem_sampler){
    ins_burst_begin = k->fastforward;
    ins_burst_end = ins_burst_begin + burst_ins_size;
  }
  else{
    ins_burst_begin = k->fastforward + k->warmup;
    ins_burst_end = ins_burst_begin + burst_ins_size;
  }
  next_watchpoint = 0;
  takeoff_loads = 0, landing_loads = 0, takeoff_stores = 0, landing_stores = 0;

  vector_size = 0;
  compressed = k->enable_compression;

  is_mem_sampler = mem_based, is_ins_sampler = ins_based;

  distribution = uniform_real_distribution<double>(0.0,1.0);

  bp = new Burst_Profile();
  bp->set_line_size(line_size_lg2);

  #ifdef USE_STL
    watchpoints.reserve(HASH_MAP_SIZE);
  #else
    #ifdef HASH
      watchpoints = dense_hash_map<uint64_t, Memory_Access*, HashFunction>(HASH_MAP_SIZE);
    #else
      watchpoints = dense_hash_map<uint64_t, Memory_Access*>(HASH_MAP_SIZE);
    #endif
    watchpoints.set_empty_key(0);
    watchpoints.set_deleted_key(ULLONG_MAX);
  #endif
}

sampler::~sampler(){
  if(in_burst){
    end_burst(last_inscount, last_mem_count);
  }
  if(ooo_samples.size() > 0){
    dump_ooo_samples(true);
  }
  dump_dangling_samples();
  delete sample_writer;
  delete ooo_writer;
  delete dangling_writer;

  if(dangling_load_stores.size() > 0){
    // we found dangling samples and have to increment the load/store counters in the registered bursts (for the landing events)
    for(map<uint64_t, pair<uint32_t, uint32_t>>::iterator it = dangling_load_stores.begin(); it != dangling_load_stores.end(); ++it){
      modify_burst(it->first, it->second.first, it->second.second, bp);
    }
  }

  write_burst_profile("burst_" + base_file_name + ".0", bp);
}

void sampler::start_burst(uint64_t inscount, uint64_t memcount){
  in_burst = true;

  curr_burst = add_burst(bp, burst_counter, inscount, memcount);

  // needs no rounding
  if(burst_rnd){
    uint64_t random = max(rnd_exp(burst_period), (uint64_t) 1);
    mem_burst_begin += burst_mem_size + random;
    mem_burst_end = mem_burst_begin - random;
  }
  else{
    mem_burst_begin += burst_mem_size + burst_period;
    mem_burst_end = mem_burst_begin - burst_period;
  }

  if(is_ins_sampler){
    // floor the numbers here for easy alignment in the analyzer
    uint64_t floor_inscount = inscount - inscount % burst_ins_size;
    ins_burst_begin = floor_inscount + burst_ins_size + burst_period;
  }
}

void sampler::end_burst(uint64_t inscount, uint64_t memcount){
  in_burst = false;

  complete_burst(curr_burst, inscount, memcount, takeoff_loads, landing_loads, takeoff_stores, landing_stores);
  takeoff_loads = 0, landing_loads = 0, takeoff_stores = 0, landing_stores = 0;

  if(is_ins_sampler){
    // floor the numbers here for easy alignment in the analyzer
    uint64_t floor_inscount = inscount - inscount % burst_ins_size;
    ins_burst_end = floor_inscount  + burst_ins_size + burst_period;
  }

  burst_counter++;
}

void sampler::dump_ooo_samples(bool final){
  if(is_ins_sampler){
    sort(ooo_samples.begin(), ooo_samples.end(), begin_less_than());
  }
  else{
    sort(ooo_samples.begin(), ooo_samples.end(), end_less_than());
  }
  for(vector<Sample*>::iterator it = ooo_samples.begin(); it != ooo_samples.end(); ++it){
    ooo_writer->write_streaming(*it, false);
    delete &(*it)->begin();
    delete &(*it)->end();
  }
  ooo_samples.clear();

  if(!final){
    ooo_writer->close_file();
    ooo_writer->open_file();
    vector_size = 0;
  }
}

void sampler::dump_dangling_samples(){
  // no need to free memory, this is at the end of the execution of our profiler, so the process will be killed soon anyway
  #ifdef USE_STL
    unordered_map<uint64_t, Memory_Access*>::iterator it_w;
  #else
    #ifdef HASH
      dense_hash_map<uint64_t, Memory_Access*, HashFunction>::iterator it_w;
    #else
      dense_hash_map<uint64_t, Memory_Access*>::iterator it_w;
    #endif
  #endif

  vector<Sample*> to_sort;
  for(it_w = watchpoints.begin(); it_w != watchpoints.end(); ++it_w){
    // first count loads and stores per burst
    uint64_t takeoff_burst = it_w->second->burst_id();
    map<uint64_t, pair<uint32_t, uint32_t>>::iterator it_dls = dangling_load_stores.find(takeoff_burst);
    if(it_dls != dangling_load_stores.end()){
      Access_Type at = it_w->second->access_type();
      if(at == Access_Type::LOAD){
        dangling_load_stores[takeoff_burst].first += 1;
      }
      else if(at == Access_Type::STORE){
        dangling_load_stores[takeoff_burst].second += 1;
      }
    }
    else{
      Access_Type at = it_w->second->access_type();
      if(at == Access_Type::LOAD){
        dangling_load_stores[takeoff_burst] = make_pair(1,0);
      }
      else if(at == Access_Type::STORE){
        dangling_load_stores[takeoff_burst] = make_pair(0,1);
      }
    }

    // make dangling sample
    to_sort.push_back(make_dangling_sample(it_w->second));
  }
  sort(to_sort.begin(), to_sort.end(), begin_less_than());

  for(vector<Sample*>::iterator it = to_sort.begin(); it != to_sort.end(); ++it){
    dangling_writer->write_streaming(*it, true);
  }
}

void sampler::in_trace(){
  log_watchpoint = true;
}

void sampler::check_ooo_vector_size(Sample *ooo_sample){
  if(compressed){
    // approximation for when we use GZIP such that the file sizes are approximately the same
    vector_size += ooo_sample->ByteSize() / 3;
  }
  else{
    vector_size += ooo_sample->ByteSize();
  }

  if(vector_size > MAX_FILE_SIZE){
    dump_ooo_samples(false);
  }
}

void sampler::check_memory_access(uint64_t addr, uint64_t access_counter, Access_Type at, uint16_t tid, uint64_t ip){
  #ifdef USE_STL
    unordered_map<uint64_t, Memory_Access*>::iterator it = watchpoints.find(addr >> line_size_lg2);
  #else
    #ifdef HASH
      dense_hash_map<uint64_t, Memory_Access*, HashFunction>::iterator it = watchpoints.find(addr >> line_size_lg2);
    #else
      dense_hash_map<uint64_t, Memory_Access*>::iterator it = watchpoints.find(addr >> line_size_lg2);
    #endif
  #endif
  if(it != watchpoints.end()){
    Memory_Access *ma_e = make_memory_access(burst_counter, addr, access_counter, at, tid, ip);

    if(at == Access_Type::LOAD){
      landing_loads++;
    }
    else if(at == Access_Type::STORE){
      landing_stores++;
    }

    if(it->second->burst_id() > burst_written){
      burst_written = it->second->burst_id();
    }
    if(it->second->burst_id() < burst_written){
      Sample *ooo_sample = make_sample(it->second, ma_e);
      ooo_samples.push_back(ooo_sample);
      check_ooo_vector_size(ooo_sample);
    }
    else{
      sample_writer->write_streaming(make_sample(it->second, ma_e), true);
      delete it->second;
      delete ma_e;
    }

    watchpoints.erase(it);
  }
}

void sampler::log_memory_access(Memory_Access *ma){
  watchpoints[ma->memory_address() >> line_size_lg2] = ma;

  if(ma->access_type() == Access_Type::LOAD){
    takeoff_loads++;
  }
  else if(ma->access_type() == Access_Type::STORE){
    takeoff_stores++;
  }
}

uint64_t sampler::rnd_exp(uint64_t period){
    double r = distribution(generator);
    return (uint64_t) (period * -log(1 - r));
}

void sampler::sample_memory_access(uint64_t addr, uint64_t access_counter, Access_Type at, uint16_t tid, uint64_t ip, uint64_t inscount){
  last_inscount = inscount;
  last_mem_count = access_counter;

  check_memory_access(addr, access_counter, at, tid, ip);

  if(is_mem_sampler){
    if(access_counter >= mem_burst_end){
      end_burst(inscount, access_counter);
    }
    if(access_counter >= mem_burst_begin){
      start_burst(inscount, access_counter);
      next_watchpoint = access_counter; // always sample first memory access in a burst
    }
  }
  else{
    // trace doesn't start at an exact boundary because we count and instrument on basic block level, make sure the burst doesn't start too early
    if(log_watchpoint){
      if(inscount >= ins_burst_end){
        end_burst(inscount, access_counter);
      }
      if(inscount >= ins_burst_begin){
        // due to warmup, which is ignored in the trace sampler, we can ignore a part of the memory accesses, so adjust them here, before we start a burst
        mem_burst_begin = access_counter;
        start_burst(inscount, access_counter);
        next_watchpoint = access_counter;
      }
    }
  }

  if(next_watchpoint == access_counter){
    if(is_mem_sampler){
      Memory_Access *ma = make_memory_access(burst_counter, addr, access_counter, at, tid, ip);

      log_memory_access(ma);

      if(sample_rnd){
        next_watchpoint += max(rnd_exp(sample_period), (uint64_t) 1);
      }
      else{
        next_watchpoint += sample_period;
      }
    }
    else if(is_ins_sampler){
      if(log_watchpoint){
        Memory_Access *ma = make_memory_access(burst_counter, addr, access_counter, at, tid, ip);

        log_memory_access(ma);

        next_watchpoint++;
        log_watchpoint = false;
      }
    }
  }
}
