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

#include "instruction.h"

// Instruction

Instruction::StaticInstructionCosts Instruction::m_instruction_costs;

Instruction::Instruction(InstructionType type, OperandList &operands)
   : m_type(type)
   , m_uops(NULL)
   , m_addr(0)
   , m_operands(operands)
{
}

Instruction::Instruction(InstructionType type)
   : m_type(type)
   , m_uops(NULL)
   , m_addr(0)
{
}

InstructionType Instruction::getType() const
{
    return m_type;
}

String Instruction::getTypeName() const
{
   return String( INSTRUCTION_NAMES[ getType() ] );
}

// An instruction cost is the number of cycles it takes to execute the instruction, times the
// period of the processor that we are currently running on
SubsecondTime Instruction::getCost(Core *core) const
{
   return SubsecondTime::Zero();
}

void Instruction::initializeStaticInstructionModel(){}

// DynamicInstruction

DynamicInstruction::DynamicInstruction(SubsecondTime cost, InstructionType type)
   : Instruction(type)
   , m_cost(cost)
{
}

DynamicInstruction::~DynamicInstruction()
{
}

SubsecondTime DynamicInstruction::getCost(Core *core) const
{
   return m_cost;
}

// StringInstruction

StringInstruction::StringInstruction(OperandList &ops)
   : Instruction(INST_STRING, ops)
{
}

SubsecondTime StringInstruction::getCost(Core *core) const
{
   return SubsecondTime::Zero();
}


// SyncInstruction

SyncInstruction::SyncInstruction(SubsecondTime time, sync_type_t sync_type)
   : Instruction(INST_SYNC)
   , m_time(time)
   , m_sync_type(sync_type)
{ }

SubsecondTime SyncInstruction::getCost(Core *core) const
{
   return SubsecondTime::Zero();
}


// SpawnInstruction

SpawnInstruction::SpawnInstruction(SubsecondTime time)
   : Instruction(INST_SPAWN)
   , m_time(time)
{ }

SubsecondTime SpawnInstruction::getCost(Core *core) const
{
   return SubsecondTime::Zero();
}

SubsecondTime SpawnInstruction::getTime() const
{
   return m_time;
}

// BranchInstruction

BranchInstruction::BranchInstruction(OperandList &l)
   : Instruction(INST_BRANCH, l)
   , m_is_mispredict(false)
   , m_is_taken(false)
   , m_target_address(-1)
{ }

SubsecondTime BranchInstruction::getCost(Core *core) const
{

   return SubsecondTime::Zero();
}
