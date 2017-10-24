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
*  Authors: Wim Heirman, Ghent University
*           Trevor Carlson, Ghent University
*           Sam Van den Steen, Ghent University
*  Copyright: 2016, Ghent University
*/

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "fixed_types.h"
#include "subsecond_time.h"
#include "operand.h"
#include <vector>
#include <sstream>

class Core;
class MicroOp;

enum InstructionType
{
   INST_GENERIC,
   INST_ADD,
   INST_SUB,
   INST_MUL,
   INST_DIV,
   INST_FADD,
   INST_FSUB,
   INST_FMUL,
   INST_FDIV,
   INST_JMP,
   INST_STRING,
   INST_BRANCH,
   INST_DYNAMIC_MISC, // All instructions above and including this one are dynamic
   INST_RECV,
   INST_SYNC,
   INST_SPAWN,
   INST_TLB_MISS,
   INST_MEM_ACCESS, // Not a regular memory access.  These are added latencies and do not correspond to a particular instruction.  Dynamic Instruction
   INST_DELAY,
   INST_UNKNOWN,
   MAX_INSTRUCTION_COUNT
};

__attribute__ ((unused)) static const char * INSTRUCTION_NAMES [] =
{"generic","add","sub","mul","div","fadd","fsub","fmul","fdiv","jmp","string", "branch", "dynamic_misc","recv","sync","spawn","tlb_miss","mem_access","delay","unknown"};

class Instruction
{
public:
   Instruction(InstructionType type,
               OperandList &operands);

   Instruction(InstructionType type);

   virtual ~Instruction() { };
   virtual SubsecondTime getCost(Core *core) const;

   InstructionType getType() const;
   String getTypeName() const;
   bool isDynamic() const
   { return getType() >= INST_DYNAMIC_MISC; }
   bool isIdle() const
   { return getType() == INST_SYNC || getType() == INST_DELAY || getType() == INST_RECV; }

   static void initializeStaticInstructionModel();

   const OperandList& getOperands() const
   { return m_operands; }

   void setAddress(IntPtr addr) { m_addr = addr; }
   IntPtr getAddress() const { return m_addr; }
   void setSize(UInt32 size) { m_size = size; }
   UInt32 getSize() const { return m_size; }

   void setAtomic(bool atomic) { m_atomic = atomic; }
   bool isAtomic() const { return m_atomic; }

   void setDisassembly(String str) { m_disas = str; }
   const String& getDisassembly(void) const { return m_disas; }

   void setMicroOps(const std::vector<const MicroOp *> *uops)
   { m_uops = uops; }

   const std::vector<const MicroOp *>* getMicroOps(void) const
   { return m_uops; }

private:
   typedef std::vector<unsigned int> StaticInstructionCosts;
   static StaticInstructionCosts m_instruction_costs;

   InstructionType m_type;
   String m_disas;

   const std::vector<const MicroOp *> *m_uops;

   IntPtr m_addr;
   UInt32 m_size;
   bool m_atomic;

protected:
   OperandList m_operands;
};

class GenericInstruction : public Instruction
{
public:
   GenericInstruction(OperandList &operands)
      : Instruction(INST_GENERIC, operands)
   {}
};

class ArithInstruction : public Instruction
{
public:
   ArithInstruction(InstructionType type, OperandList &operands)
      : Instruction(type, operands)
   {}
};

class JmpInstruction : public Instruction
{
public:
   JmpInstruction(OperandList &dest)
      : Instruction(INST_JMP, dest)
   {}
};

// CMPSB or SCASB instruction
class StringInstruction : public Instruction
{
public:
   StringInstruction(OperandList &ops);

   SubsecondTime getCost(Core *core) const;
};

// for operations not associated with the binary -- such as processing
// a packet
class DynamicInstruction : public Instruction
{
public:
   DynamicInstruction(SubsecondTime cost, InstructionType type = INST_DYNAMIC_MISC);
   ~DynamicInstruction();

   SubsecondTime getCost(Core *core) const;

private:
   SubsecondTime m_cost;
};

class RecvInstruction : public DynamicInstruction
{
public:
   RecvInstruction(SubsecondTime cost)
      : DynamicInstruction(cost, INST_RECV)
   {}
};

// wake up after synchronization
// we may have been rescheduled, so this sets an absolute time
class SyncInstruction : public Instruction
{
public:
   enum sync_type_t {
      FUTEX,
      PTHREAD_MUTEX,
      PTHREAD_COND,
      PTHREAD_BARRIER,
      JOIN,
      PAUSE,
      SLEEP,
      SYSCALL,
      UNSCHEDULED,
      NUM_TYPES
   };

   SyncInstruction(SubsecondTime time, sync_type_t sync_type);
   SubsecondTime getCost(Core *core) const;
   SubsecondTime getTime() const { return m_time; }
   sync_type_t getSyncType() const { return m_sync_type; }

private:
   SubsecondTime m_time;
   sync_type_t m_sync_type;
};

// set clock to particular time
class SpawnInstruction : public Instruction
{
public:
   SpawnInstruction(SubsecondTime time);
   SubsecondTime getCost(Core *core) const;
   SubsecondTime getTime() const;

private:
   SubsecondTime m_time;
};

// conditional branches
class BranchInstruction : public Instruction
{
private:
   bool m_is_mispredict;
   bool m_is_taken;
   IntPtr m_target_address;
public:
   BranchInstruction(OperandList &l);

   SubsecondTime getCost(Core *core) const;
   bool getIsMispredict() const { return m_is_mispredict; }
   bool getIsTaken() const { return m_is_taken; }
   IntPtr getTargetAddress() const { return m_target_address; }
};

// TLB misses
class TLBMissInstruction : public DynamicInstruction
{
   bool m_is_ifetch;
public:
   TLBMissInstruction(SubsecondTime cost, bool is_ifetch)
      : DynamicInstruction(cost, INST_TLB_MISS)
      , m_is_ifetch(is_ifetch)
   {}
   bool isIfetch() const { return m_is_ifetch; }
};

class MemAccessInstruction : public DynamicInstruction
{
private:
   IntPtr m_address;
   UInt32 m_data_size;
   bool m_is_fence;
public:
   MemAccessInstruction(SubsecondTime cost, IntPtr address, UInt32 data_size, bool is_fence)
      : DynamicInstruction(cost, INST_MEM_ACCESS)
      , m_address(address)
      , m_data_size(data_size)
      , m_is_fence(is_fence)
   {}
   IntPtr getDataAddress() const { return m_address; }
   UInt32 getDataSize() const { return m_data_size; }
   bool isFence() const { return m_is_fence; }
};

class DelayInstruction : public DynamicInstruction
{
public:
   enum delay_type_t {
      DVFS_TRANSITION,
      NUM_TYPES
   };
   DelayInstruction(SubsecondTime cost, delay_type_t delay_type)
      : DynamicInstruction(cost, INST_DELAY)
      , m_delay_type(delay_type)
   { }
   delay_type_t getDelayType() const { return m_delay_type; }
private:
   delay_type_t m_delay_type;
};

class UnknownInstruction : public DynamicInstruction
{
public:
   UnknownInstruction(SubsecondTime cost)
      : DynamicInstruction(cost, INST_UNKNOWN)
   { }
};

#endif
