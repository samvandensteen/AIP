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

#include "extended_micro_op.h"

// empty constructor
Extended_Micro_Op::Extended_Micro_Op(){
	uop = MicroOp();
	address = 0;
	seq_nr = 0;
	type = UOP_TYPE_INVALID;
	path_length = 0;

	producing_load_position = -1;
	no_producing_loads = 0;

	for(int i = 0; i < MAX_PRODUCERS; i++){
		producers[i] = 0;
	}
}

// constructor
Extended_Micro_Op::Extended_Micro_Op(MicroOp op, int nr){
	uop = op;
	seq_nr = nr;
	address = 0;
	path_length = 1;

	producing_load_position = -1;
	no_producing_loads = 0;

	// at construction time, there can be no dependencies
	for(int i = 0; i < MAX_PRODUCERS; i++){
		producers[i] = 0;
	}

	set_extended_uop_type();
}

// copy constructor
Extended_Micro_Op::Extended_Micro_Op(const Extended_Micro_Op & rhs){
	uop = rhs.uop;
	address = rhs.address;
	seq_nr = rhs.seq_nr;
	type = rhs.type;
	path_length = rhs.path_length;

	producing_load_position = rhs.producing_load_position;
	no_producing_loads = rhs.no_producing_loads;

	for(int i = 0; i < MAX_PRODUCERS; i++){
		producers[i] = rhs.producers[i];
	}
}

// assignment operator
Extended_Micro_Op& Extended_Micro_Op::operator=(const Extended_Micro_Op& rhs){
	if(this != &rhs){
		uop = rhs.uop;
		address = rhs.address;
		seq_nr = rhs.seq_nr;
		type = rhs.type;
		path_length = rhs.path_length;

		producing_load_position = rhs.producing_load_position;
		no_producing_loads = rhs.no_producing_loads;

		for(int i = 0; i < MAX_PRODUCERS; i++){
			producers[i] = rhs.producers[i];
		}
	}
	return *this;
}

// smaller than operator
bool Extended_Micro_Op::operator<(const Extended_Micro_Op& rhs){
	return seq_nr < rhs.seq_nr;
}

bool Extended_Micro_Op::operator>(const Extended_Micro_Op& rhs){
	return seq_nr > rhs.seq_nr;
}

bool Extended_Micro_Op::operator<=(const Extended_Micro_Op& rhs){
	return seq_nr <= rhs.seq_nr;
}

bool Extended_Micro_Op::operator>=(const Extended_Micro_Op& rhs){
	return seq_nr >= rhs.seq_nr;
}

bool Extended_Micro_Op::is_load(){
	return uop.isLoad();
}

bool Extended_Micro_Op::is_store(){
	return uop.isStore();
}

bool Extended_Micro_Op::is_branch(){
	return uop.isBranch();
}

void Extended_Micro_Op::add_producer(Extended_Micro_Op* prod){
	int i = 0;
	while(producers[i] != 0){
		if(producers[i] == prod->get_seq_nr())
			return;
		i++;
	}
	producers[i] = prod->get_seq_nr();

	// depending on a load?
	if(prod->is_load()){
		producing_load_position = prod->get_seq_nr();
		no_producing_loads = prod->get_no_producing_loads() + 1;
	}
	else if(prod->get_producing_load_position()){
		producing_load_position = prod->get_producing_load_position();
		no_producing_loads = prod->get_no_producing_loads();
	}
}

int* Extended_Micro_Op::get_producers(){
	return producers;
}

MicroOp Extended_Micro_Op::get_uop(){
	return uop;
}

extended_uop_type Extended_Micro_Op::get_extended_uop_type(){
	return type;
}

int Extended_Micro_Op::get_seq_nr(){
	return seq_nr;
}

uint64_t Extended_Micro_Op::get_address(){
	return address;
}

void Extended_Micro_Op::set_address(uint64_t addr){
	address = addr;
}

int Extended_Micro_Op::get_path_length(){
	return path_length;
}

void Extended_Micro_Op::set_path_length(int path){
	path_length = path;
}

int Extended_Micro_Op::get_producing_load_position(){
	return producing_load_position;
}

int Extended_Micro_Op::get_no_producing_loads(){
	return no_producing_loads;
}

void Extended_Micro_Op::set_extended_uop_type(){
	if(uop.isLoad()){
		type = UOP_TYPE_LOAD;
	}
	else if(uop.isStore()){
		type = UOP_TYPE_STORE;
	}
	else if(uop.isBranch()){
		type = UOP_TYPE_COND_BRANCH;
	}
	else{
		switch(uop.getInstructionOpcode()){
			case XED_ICLASS_JNB:		// Jump if not below
			case XED_ICLASS_JNBE:		// Jump if not below or equal
			case XED_ICLASS_JNL:		// Jump if not less
			case XED_ICLASS_JNLE:		// Jump is not less or equal
			case XED_ICLASS_JNO:		// Jump if no overflow
			case XED_ICLASS_JNP:		// Jump if not parity
			case XED_ICLASS_JNS:		// Jump if no sign (=positive)
			case XED_ICLASS_JNZ:		// Jump if not zero
			case XED_ICLASS_JO:			// Jump if overflow
			case XED_ICLASS_JP:			// Jump if parity
			case XED_ICLASS_JRCXZ:		// Check RCX registers, respectively, and pass control to the target instruction when the value of rCX register reaches 0
			case XED_ICLASS_JS:			// Jump if sign (=negative)
			case XED_ICLASS_JZ:			// Jump if zero
			case XED_ICLASS_JB:			// Jump if below
			case XED_ICLASS_JBE:		// Jump if below or equal
			case XED_ICLASS_JL:			// Jump if less
			case XED_ICLASS_JLE:		// Jump if less or equal
				std::cerr << "This shouldn't happen!" << std::endl;
				exit(1337);
				type = UOP_TYPE_COND_BRANCH;
				break;
			case XED_ICLASS_CALL_FAR:	// Far call
			case XED_ICLASS_CALL_NEAR:	// Near call
			case XED_ICLASS_JMP:		// Jump
			case XED_ICLASS_JMP_FAR:	// Jump far
			case XED_ICLASS_RET_FAR:	// Far return to calling procedure
			case XED_ICLASS_RET_NEAR:	// Near return to calling procedure
				type = UOP_TYPE_UNCOND_BRANCH;
				break;
			case XED_ICLASS_STOSQ:		// Copy register RAX to RDI
			//Conditional moves are executed if source is from memory no matter the condition
			case XED_ICLASS_CMOVB: 		// Conditional move if below
			case XED_ICLASS_CMOVBE: 	// Conditional move if below or equal
			case XED_ICLASS_CMOVL: 		// Conditional move if less
			case XED_ICLASS_CMOVLE: 	// Conditional move if less or equal
			case XED_ICLASS_CMOVNB: 	// Conditional move if not below
			case XED_ICLASS_CMOVNBE: 	// Conditional move if not below or equal
			case XED_ICLASS_CMOVNL: 	// Conditional move if not less
			case XED_ICLASS_CMOVNLE: 	// Conditional move if not less or equal
			case XED_ICLASS_CMOVNS:		// Conditional move if not sign
			case XED_ICLASS_CMOVNZ: 	// Conditional move if not zero
			case XED_ICLASS_CMOVS: 		// Conditional move if sign
			case XED_ICLASS_CMOVZ: 		// Conditional move if zero
			case XED_ICLASS_MOV: 		// Move
			case XED_ICLASS_MOVAPD:		// Move packed double-precision floating-point numbers from xmm2/m128 to xmm1 (or reverse)
			case XED_ICLASS_MOVAPS:		// Move packed single-precision floating-point numbers from xmm2/m128 to xmm1 (or reverse)
			case XED_ICLASS_MOVDQA:		// Move aligned double quadword from xmm2/m128 to xmm1 (or reverse)
			case XED_ICLASS_MOVHPD:		// Move double-precision floating-point value from m64 to high quadword of xmm (or reverse)
			case XED_ICLASS_MOVQ: 		// Move quadword from mm/m64 to mm (or reverse); Move quadword from xmm2/mem64 to xmm1 (or reverse)
			case XED_ICLASS_MOVSQ:		// Moves source to destination operand. Both the source and destination operands are located in memory.
			case XED_ICLASS_MOVSX: 		// Sign-extend byte to word or word to double word
			case XED_ICLASS_MOVSXD:		// Read 32bit source operand and sign-extend it to 64 bit
			case XED_ICLASS_MOVZX:		// Move with zero extend
			//Set destination operand to O or 1 (byte register or byte in memory)
			case XED_ICLASS_SETB: 		// Set if below
			case XED_ICLASS_SETBE:		// Set if below or equal
			case XED_ICLASS_SETLE:		// Set if less of equal
			case XED_ICLASS_SETNB:		// Set if not below
			case XED_ICLASS_SETNBE:		// Set if not below or equal
			case XED_ICLASS_SETNL:		// Set if not less
			case XED_ICLASS_SETNLE:		// Set if not less or equal
			case XED_ICLASS_SETNP:		// Set if no parity
			case XED_ICLASS_SETNZ:		// Set if not zero
			case XED_ICLASS_SETP: 		// Set if parity
			case XED_ICLASS_SETZ:		// Set if zero
				type = UOP_TYPE_CHNGREG;
				break;
			case XED_ICLASS_ADC:		// Standard integer addition with carry
			case XED_ICLASS_ADD:		// Standard integer addition
			case XED_ICLASS_INC:		// Increment by 1
			case XED_ICLASS_LEA:		// Load Effective Address (calculates sum of 2 registers and stores in third)
			case XED_ICLASS_PADDB:
			case XED_ICLASS_PADDD:
			case XED_ICLASS_PADDQ:
			case XED_ICLASS_PADDSB:
			case XED_ICLASS_PADDSW:
			case XED_ICLASS_PADDUSB:
			case XED_ICLASS_PADDUSW:
			case XED_ICLASS_PADDW:
			case XED_ICLASS_PHADDD:
			case XED_ICLASS_PHADDSW:
			case XED_ICLASS_PHADDW:
			case XED_ICLASS_XADD:
				type = UOP_TYPE_I_ADD;
				break;
			case XED_ICLASS_DEC:		// Decrement with 1
			case XED_ICLASS_PHSUBD:
			case XED_ICLASS_PHSUBSW:
			case XED_ICLASS_PHSUBW:
			case XED_ICLASS_PSUBB:
			case XED_ICLASS_PSUBD:
			case XED_ICLASS_PSUBQ:
			case XED_ICLASS_PSUBSB:
			case XED_ICLASS_PSUBSW:
			case XED_ICLASS_PSUBUSB:
			case XED_ICLASS_PSUBUSW:
			case XED_ICLASS_PSUBW:
			case XED_ICLASS_SBB:
			case XED_ICLASS_SUB:		// Standard integer subtraction
				type = UOP_TYPE_I_SUB;
				break;
			case XED_ICLASS_MUL:
			case XED_ICLASS_IMUL:
			case XED_ICLASS_PCLMULQDQ:
			case XED_ICLASS_PMULDQ:
			case XED_ICLASS_PMULHUW:
			case XED_ICLASS_PMULHW:
			case XED_ICLASS_PMULLD:
			case XED_ICLASS_PMULLW:
			case XED_ICLASS_PMULUDQ:
				type = UOP_TYPE_I_MUL;
				break;
			case XED_ICLASS_DIV:		// Standard integer division
			case XED_ICLASS_IDIV:		// Standard integer division
				type = UOP_TYPE_I_DIV;
				break;
			case XED_ICLASS_ADDPD:
			case XED_ICLASS_ADDPS:
			case XED_ICLASS_ADDSD:
			case XED_ICLASS_ADDSS:
			case XED_ICLASS_ADDSUBPD: // Add and subtract
			case XED_ICLASS_ADDSUBPS: // Add and subtract
			case XED_ICLASS_FIADD:
			case XED_ICLASS_FADD:
			case XED_ICLASS_FADDP:
			case XED_ICLASS_HADDPD:
			case XED_ICLASS_HADDPS:
			case XED_ICLASS_PFADD:
			case XED_ICLASS_VADDPD:
			case XED_ICLASS_VADDPS:
			case XED_ICLASS_VADDSD:
			case XED_ICLASS_VADDSS:
			case XED_ICLASS_VADDSUBPD: // Add and subtract
			case XED_ICLASS_VADDSUBPS: // Add and subtract
				type = UOP_TYPE_FP_ADD;
				break;
			case XED_ICLASS_FISUB:
			case XED_ICLASS_FISUBR:
			case XED_ICLASS_FSUB:
			case XED_ICLASS_FSUBP:
			case XED_ICLASS_FSUBR:
			case XED_ICLASS_FSUBRP:
			case XED_ICLASS_HSUBPD:
			case XED_ICLASS_HSUBPS:
			case XED_ICLASS_PFSUB:
			case XED_ICLASS_PFSUBR:
			case XED_ICLASS_SUBPD:
			case XED_ICLASS_SUBPS:
			case XED_ICLASS_SUBSD:
			case XED_ICLASS_SUBSS:
			case XED_ICLASS_VSUBPD:
			case XED_ICLASS_VSUBPS:
			case XED_ICLASS_VSUBSD:
			case XED_ICLASS_VSUBSS:
				type = UOP_TYPE_FP_SUB;
				break;
			case XED_ICLASS_MULPD:
			case XED_ICLASS_MULPS:
			case XED_ICLASS_MULSD:
			case XED_ICLASS_MULSS:
			case XED_ICLASS_FIMUL:
			case XED_ICLASS_FMUL:
			case XED_ICLASS_FMULP:
			case XED_ICLASS_PFMUL:
			case XED_ICLASS_VMULPD:
			case XED_ICLASS_VMULPS:
			case XED_ICLASS_VMULSD:
			case XED_ICLASS_VMULSS:
				type = UOP_TYPE_FP_MUL;
				break;
			case XED_ICLASS_VDIVPD:
			case XED_ICLASS_VDIVPS:
			case XED_ICLASS_VDIVSD:
			case XED_ICLASS_VDIVSS:
			case XED_ICLASS_DIVPD:
			case XED_ICLASS_DIVPS:
			case XED_ICLASS_DIVSD:
			case XED_ICLASS_DIVSS:
			case XED_ICLASS_FDIV:
			case XED_ICLASS_FDIVP:
			case XED_ICLASS_FDIVR:
			case XED_ICLASS_FDIVRP:
			case XED_ICLASS_FIDIV:
			case XED_ICLASS_FIDIVR:
				type = UOP_TYPE_FP_DIV;
				break;
			case XED_ICLASS_FSQRT:
			case XED_ICLASS_PFSQRT:
			case XED_ICLASS_RSQRTPS:
			case XED_ICLASS_RSQRTSS:
			case XED_ICLASS_SQRTPD:
			case XED_ICLASS_SQRTPS:
			case XED_ICLASS_SQRTSD:
			case XED_ICLASS_SQRTSS:
			case XED_ICLASS_VSQRTSD:
			case XED_ICLASS_VSQRTPD:
				type = UOP_TYPE_FP_SQRT;
				break;
			case XED_ICLASS_AND:
			case XED_ICLASS_ANDNPD:
			case XED_ICLASS_ANDNPS:
			case XED_ICLASS_ANDPD:
			case XED_ICLASS_ANDPS:
			case XED_ICLASS_CLD:
			case XED_ICLASS_CMP:
			case XED_ICLASS_CMPSB:
			case XED_ICLASS_CMPSD_XMM:
			case XED_ICLASS_CMPXCHG:
			case XED_ICLASS_LEAVE:
			case XED_ICLASS_NEG:
			case XED_ICLASS_NOP:
			case XED_ICLASS_NOT:
			case XED_ICLASS_OR:
			case XED_ICLASS_PAND:
			case XED_ICLASS_PANDN:
			case XED_ICLASS_PCMPGTD:
			case XED_ICLASS_POP:
			case XED_ICLASS_POR:
			case XED_ICLASS_PSRLDQ:
			case XED_ICLASS_PUSH:
			case XED_ICLASS_PXOR:
			case XED_ICLASS_ROL:
			case XED_ICLASS_ROR:
			case XED_ICLASS_SAR:
			case XED_ICLASS_SHL:
			case XED_ICLASS_SHR:
			case XED_ICLASS_TEST:
			case XED_ICLASS_XOR:
			case XED_ICLASS_XORPD:
			case XED_ICLASS_XORPS:
			// Sniper puts these in the 1-cycle class
			case XED_ICLASS_CVTSS2SD:
			case XED_ICLASS_UNPCKLPD:
			case XED_ICLASS_UNPCKLPS:
			case XED_ICLASS_CDQE:
				type = UOP_TYPE_GENERIC_1;
				break;
			case XED_ICLASS_CVTPS2PD:
				type = UOP_TYPE_GENERIC_2;
				break;
			case XED_ICLASS_COMISS:
			case XED_ICLASS_COMISD:
			case XED_ICLASS_UCOMISS:
			case XED_ICLASS_UCOMISD:
			case XED_ICLASS_MAXSS:
			case XED_ICLASS_MAXSD:
			case XED_ICLASS_MAXPS:
			case XED_ICLASS_MAXPD:
			case XED_ICLASS_MINSS:
			case XED_ICLASS_MINSD:
			case XED_ICLASS_MINPS:
			case XED_ICLASS_MINPD:
				type = UOP_TYPE_GENERIC_3;
				break;
			case XED_ICLASS_CVTPD2PS:
			case XED_ICLASS_CVTSD2SS:
			case XED_ICLASS_CVTDQ2PS:
			case XED_ICLASS_CVTPS2DQ:
			case XED_ICLASS_CVTTPS2DQ:
			case XED_ICLASS_CVTDQ2PD:
			case XED_ICLASS_CVTPD2DQ:
			case XED_ICLASS_CVTTPD2DQ:
			case XED_ICLASS_CVTPI2PS:
			case XED_ICLASS_CVTPS2PI:
			case XED_ICLASS_CVTTPS2PI:
			case XED_ICLASS_CVTPI2PD:
			case XED_ICLASS_CVTPD2PI:
			case XED_ICLASS_CVTTPD2PI:
			case XED_ICLASS_CVTSI2SS:
			case XED_ICLASS_CVTSS2SI:
			case XED_ICLASS_CVTTSS2SI:
			case XED_ICLASS_CVTSI2SD:
			case XED_ICLASS_CVTSD2SI:
			case XED_ICLASS_CVTTSD2SI:
				type = UOP_TYPE_GENERIC_5;
				break;
			default:
				type = UOP_TYPE_OTHER;
				break;
		}
	}
}
