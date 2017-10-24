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

#include "extended_uop_type_enum.h"

std::string uop_to_string(extended_uop_type uop_t){
	switch(uop_t){
		case UOP_TYPE_COND_BRANCH:
			return "COND_BRANCH";
		case UOP_TYPE_UNCOND_BRANCH:
			return "UNCOND_BRANCH";
		case UOP_TYPE_CHNGREG:
			return "CHNGREG";
		case UOP_TYPE_I_ADD:
			return "I_ADD";
		case UOP_TYPE_I_SUB:
			return "I_SUB";
		case UOP_TYPE_I_MUL:
			return "I_MUL";
		case UOP_TYPE_I_DIV:
			return "I_DIV";
		case UOP_TYPE_FP_ADD:
			return "FP_ADD";
		case UOP_TYPE_FP_SUB:
			return "FP_SUB";
		case UOP_TYPE_FP_MUL:
			return "FP_MUL";
		case UOP_TYPE_FP_DIV:
			return "FP_DIV";
		case UOP_TYPE_FP_SQRT:
			return "FP_SQRT";
		case UOP_TYPE_GENERIC_1:
			return "GENERIC_1";
		case UOP_TYPE_GENERIC_2:
			return "GENERIC_2";
		case UOP_TYPE_GENERIC_3:
			return "GENERIC_3";
		case UOP_TYPE_GENERIC_5:
			return "GENERIC_5";
		case UOP_TYPE_LOAD:
			return "LOAD";
		case UOP_TYPE_OTHER:
			return "OTHER";
		case UOP_TYPE_STORE:
			return "STORE";
		default:
			return "INVALID";
	}
}

// source http://www.cs.virginia.edu/kim/publicity/pin/docs/20751/Xed/html/xed-iclass-enum_8h.html#aac14887efb58dc90badb7126257354380a08a4fa855dc688f09e671e6105ee9
std::string convert_uop_to_xedstring(xed_iclass_enum_t uop){
	switch(uop){
		case XED_ICLASS_INVALID:
			return "XED_ICLASS_INVALID";
		case XED_ICLASS_AAA:
			return "XED_ICLASS_AAA";
		case XED_ICLASS_AAD:
			return "XED_ICLASS_AAD";
		case XED_ICLASS_AAM:
			return "XED_ICLASS_AAM";
		case XED_ICLASS_AAS:
			return "XED_ICLASS_AAS";
		case XED_ICLASS_ADC:
			return "XED_ICLASS_ADC";
		case XED_ICLASS_ADD:
			return "XED_ICLASS_ADD";
		case XED_ICLASS_ADDPD:
			return "XED_ICLASS_ADDPD";
		case XED_ICLASS_ADDPS:
			return "XED_ICLASS_ADDPS";
		case XED_ICLASS_ADDSD:
			return "XED_ICLASS_ADDSD";
		case XED_ICLASS_ADDSS:
			return "XED_ICLASS_ADDSS";
		case XED_ICLASS_ADDSUBPD:
			return "XED_ICLASS_ADDSUBPD";
		case XED_ICLASS_ADDSUBPS:
			return "XED_ICLASS_ADDSUBPS";
		case XED_ICLASS_AESDEC:
			return "XED_ICLASS_AESDEC";
		case XED_ICLASS_AESDECLAST:
			return "XED_ICLASS_AESDECLAST";
		case XED_ICLASS_AESENC:
			return "XED_ICLASS_AESENC";
		case XED_ICLASS_AESENCLAST:
			return "XED_ICLASS_AESENCLAST";
		case XED_ICLASS_AESIMC:
			return "XED_ICLASS_AESIMC";
		case XED_ICLASS_AESKEYGENASSIST:
			return "XED_ICLASS_AESKEYGENASSIST";
		case XED_ICLASS_AND:
			return "XED_ICLASS_AND";
		case XED_ICLASS_ANDNPD:
			return "XED_ICLASS_ANDNPD";
		case XED_ICLASS_ANDNPS:
			return "XED_ICLASS_ANDNPS";
		case XED_ICLASS_ANDPD:
			return "XED_ICLASS_ANDPD";
		case XED_ICLASS_ANDPS:
			return "XED_ICLASS_ANDPS";
		case XED_ICLASS_ARPL:
			return "XED_ICLASS_ARPL";
		case XED_ICLASS_BLENDPD:
			return "XED_ICLASS_BLENDPD";
		case XED_ICLASS_BLENDPS:
			return "XED_ICLASS_BLENDPS";
		case XED_ICLASS_BLENDVPD:
			return "XED_ICLASS_BLENDVPD";
		case XED_ICLASS_BLENDVPS:
			return "XED_ICLASS_BLENDVPS";
		case XED_ICLASS_BOUND:
			return "XED_ICLASS_BOUND";
		case XED_ICLASS_BSF:
			return "XED_ICLASS_BSF";
		case XED_ICLASS_BSR:
			return "XED_ICLASS_BSR";
		case XED_ICLASS_BSWAP:
			return "XED_ICLASS_BSWAP";
		case XED_ICLASS_BT:
			return "XED_ICLASS_BT";
		case XED_ICLASS_BTC:
			return "XED_ICLASS_BTC";
		case XED_ICLASS_BTR:
			return "XED_ICLASS_BTR";
		case XED_ICLASS_BTS:
			return "XED_ICLASS_BTS";
		case XED_ICLASS_CALL_FAR:
			return "XED_ICLASS_CALL_FAR";
		case XED_ICLASS_CALL_NEAR:
			return "XED_ICLASS_CALL_NEAR";
		case XED_ICLASS_CBW:
			return "XED_ICLASS_CBW";
		case XED_ICLASS_CDQ:
			return "XED_ICLASS_CDQ";
		case XED_ICLASS_CDQE:
			return "XED_ICLASS_CDQE";
		case XED_ICLASS_CLC:
			return "XED_ICLASS_CLC";
		case XED_ICLASS_CLD:
			return "XED_ICLASS_CLD";
		case XED_ICLASS_CLFLUSH:
			return "XED_ICLASS_CLFLUSH";
		case XED_ICLASS_CLGI:
			return "XED_ICLASS_CLGI";
		case XED_ICLASS_CLI:
			return "XED_ICLASS_CLI";
		case XED_ICLASS_CLTS:
			return "XED_ICLASS_CLTS";
		case XED_ICLASS_CMC:
			return "XED_ICLASS_CMC";
		case XED_ICLASS_CMOVB:
			return "XED_ICLASS_CMOVB";
		case XED_ICLASS_CMOVBE:
			return "XED_ICLASS_CMOVBE";
		case XED_ICLASS_CMOVL:
			return "XED_ICLASS_CMOVL";
		case XED_ICLASS_CMOVLE:
			return "XED_ICLASS_CMOVLE";
		case XED_ICLASS_CMOVNB:
			return "XED_ICLASS_CMOVNB";
		case XED_ICLASS_CMOVNBE:
			return "XED_ICLASS_CMOVNBE";
		case XED_ICLASS_CMOVNL:
			return "XED_ICLASS_CMOVNL";
		case XED_ICLASS_CMOVNLE:
			return "XED_ICLASS_CMOVNLE";
		case XED_ICLASS_CMOVNO:
			return "XED_ICLASS_CMOVNO";
		case XED_ICLASS_CMOVNP:
			return "XED_ICLASS_CMOVNP";
		case XED_ICLASS_CMOVNS:
			return "XED_ICLASS_CMOVNS";
		case XED_ICLASS_CMOVNZ:
			return "XED_ICLASS_CMOVNZ";
		case XED_ICLASS_CMOVO:
			return "XED_ICLASS_CMOVO";
		case XED_ICLASS_CMOVP:
			return "XED_ICLASS_CMOVP";
		case XED_ICLASS_CMOVS:
			return "XED_ICLASS_CMOVS";
		case XED_ICLASS_CMOVZ:
			return "XED_ICLASS_CMOVZ";
		case XED_ICLASS_CMP:
			return "XED_ICLASS_CMP";
		case XED_ICLASS_CMPPD:
			return "XED_ICLASS_CMPPD";
		case XED_ICLASS_CMPPS:
			return "XED_ICLASS_CMPPS";
		case XED_ICLASS_CMPSB:
			return "XED_ICLASS_CMPSB";
		case XED_ICLASS_CMPSD:
			return "XED_ICLASS_CMPSD";
		case XED_ICLASS_CMPSD_XMM:
			return "XED_ICLASS_CMPSD_XMM";
		case XED_ICLASS_CMPSQ:
			return "XED_ICLASS_CMPSQ";
		case XED_ICLASS_CMPSS:
			return "XED_ICLASS_CMPSS";
		case XED_ICLASS_CMPSW:
			return "XED_ICLASS_CMPSW";
		case XED_ICLASS_CMPXCHG:
			return "XED_ICLASS_CMPXCHG";
		case XED_ICLASS_CMPXCHG16B:
			return "XED_ICLASS_CMPXCHG16B";
		case XED_ICLASS_CMPXCHG8B:
			return "XED_ICLASS_CMPXCHG8B";
		case XED_ICLASS_COMISD:
			return "XED_ICLASS_COMISD";
		case XED_ICLASS_COMISS:
			return "XED_ICLASS_COMISS";
		case XED_ICLASS_CPUID:
			return "XED_ICLASS_CPUID";
		case XED_ICLASS_CQO:
			return "XED_ICLASS_CQO";
		case XED_ICLASS_CRC32:
			return "XED_ICLASS_CRC32";
		case XED_ICLASS_CVTDQ2PD:
			return "XED_ICLASS_CVTDQ2PD";
		case XED_ICLASS_CVTDQ2PS:
			return "XED_ICLASS_CVTDQ2PS";
		case XED_ICLASS_CVTPD2DQ:
			return "XED_ICLASS_CVTPD2DQ";
		case XED_ICLASS_CVTPD2PI:
			return "XED_ICLASS_CVTPD2PI";
		case XED_ICLASS_CVTPD2PS:
			return "XED_ICLASS_CVTPD2PS";
		case XED_ICLASS_CVTPI2PD:
			return "XED_ICLASS_CVTPI2PD";
		case XED_ICLASS_CVTPI2PS:
			return "XED_ICLASS_CVTPI2PS";
		case XED_ICLASS_CVTPS2DQ:
			return "XED_ICLASS_CVTPS2DQ";
		case XED_ICLASS_CVTPS2PD:
			return "XED_ICLASS_CVTPS2PD";
		case XED_ICLASS_CVTPS2PI:
			return "XED_ICLASS_CVTPS2PI";
		case XED_ICLASS_CVTSD2SI:
			return "XED_ICLASS_CVTSD2SI";
		case XED_ICLASS_CVTSD2SS:
			return "XED_ICLASS_CVTSD2SS";
		case XED_ICLASS_CVTSI2SD:
			return "XED_ICLASS_CVTSI2SD";
		case XED_ICLASS_CVTSI2SS:
			return "XED_ICLASS_CVTSI2SS";
		case XED_ICLASS_CVTSS2SD:
			return "XED_ICLASS_CVTSS2SD";
		case XED_ICLASS_CVTSS2SI:
			return "XED_ICLASS_CVTSS2SI";
		case XED_ICLASS_CVTTPD2DQ:
			return "XED_ICLASS_CVTTPD2DQ";
		case XED_ICLASS_CVTTPD2PI:
			return "XED_ICLASS_CVTTPD2PI";
		case XED_ICLASS_CVTTPS2DQ:
			return "XED_ICLASS_CVTTPD2PI";
		case XED_ICLASS_CVTTPS2PI:
			return "XED_ICLASS_CVTTPS2PI";
		case XED_ICLASS_CVTTSD2SI:
			return "XED_ICLASS_CVTTSD2SI";
		case XED_ICLASS_CVTTSS2SI:
			return "XED_ICLASS_CVTTSS2SI";
		case XED_ICLASS_CWD:
			return "XED_ICLASS_CWD";
		case XED_ICLASS_CWDE:
			return "XED_ICLASS_CWDE";
		case XED_ICLASS_DAA:
			return "XED_ICLASS_DAA";
		case XED_ICLASS_DAS:
			return "XED_ICLASS_DAS";
		case XED_ICLASS_DEC:
			return "XED_ICLASS_DEC";
		case XED_ICLASS_DIV:
			return "XED_ICLASS_DIV";
		case XED_ICLASS_DIVPD:
			return "XED_ICLASS_DIVPD";
		case XED_ICLASS_DIVPS:
			return "XED_ICLASS_DIVPS";
		case XED_ICLASS_DIVSD:
			return "XED_ICLASS_DIVSD";
		case XED_ICLASS_DIVSS:
			return "XED_ICLASS_DIVSS";
		case XED_ICLASS_DPPD:
			return "XED_ICLASS_DPPD";
		case XED_ICLASS_DPPS:
			return "XED_ICLASS_DPPS";
		case XED_ICLASS_EMMS:
			return "XED_ICLASS_EMMS";
		case XED_ICLASS_ENTER:
			return "XED_ICLASS_ENTER";
		case XED_ICLASS_EXTRACTPS:
			return "XED_ICLASS_EXTRACTPS";
		case XED_ICLASS_EXTRQ:
			return "XED_ICLASS_EXTRQ";
		case XED_ICLASS_F2XM1:
			return "XED_ICLASS_F2XM1";
		case XED_ICLASS_FABS:
			return "XED_ICLASS_FABS";
		case XED_ICLASS_FADD:
			return "XED_ICLASS_FADD";
		case XED_ICLASS_FADDP:
			return "XED_ICLASS_FADDP";
		case XED_ICLASS_FBLD:
			return "XED_ICLASS_FBLD";
		case XED_ICLASS_FBSTP:
			return "XED_ICLASS_FBSTP";
		case XED_ICLASS_FCHS:
			return "XED_ICLASS_FCHS";
		case XED_ICLASS_FCMOVB:
			return "XED_ICLASS_FCMOVB";
		case XED_ICLASS_FCMOVBE:
			return "XED_ICLASS_FCMOVBE";
		case XED_ICLASS_FCMOVE:
			return "XED_ICLASS_FCMOVE";
		case XED_ICLASS_FCMOVNB:
			return "XED_ICLASS_FCMOVNB";
		case XED_ICLASS_FCMOVNBE:
			return "XED_ICLASS_FCMOVNBE";
		case XED_ICLASS_FCMOVNE:
			return "XED_ICLASS_FCMOVNE";
		case XED_ICLASS_FCMOVNU:
			return "XED_ICLASS_FCMOVNU";
		case XED_ICLASS_FCMOVU:
			return "XED_ICLASS_FCMOVU";
		case XED_ICLASS_FCOM:
			return "XED_ICLASS_FCOM";
		case XED_ICLASS_FCOMI:
			return "XED_ICLASS_FCOMI";
		case XED_ICLASS_FCOMIP:
			return "XED_ICLASS_FCOMIP";
		case XED_ICLASS_FCOMP:
			return "XED_ICLASS_FCOMP";
		case XED_ICLASS_FCOMPP:
			return "XED_ICLASS_FCOMPP";
		case XED_ICLASS_FCOS:
			return "XED_ICLASS_FCOS";
		case XED_ICLASS_FDECSTP:
			return "XED_ICLASS_FDECSTP";
		case XED_ICLASS_FDIV:
			return "XED_ICLASS_FDIV";
		case XED_ICLASS_FDIVP:
			return "XED_ICLASS_FDIVP";
		case XED_ICLASS_FDIVR:
			return "XED_ICLASS_FDIVR";
		case XED_ICLASS_FDIVRP:
			return "XED_ICLASS_FDIVRP";
		case XED_ICLASS_FEMMS:
			return "XED_ICLASS_FEMMS";
		case XED_ICLASS_FFREE:
			return "XED_ICLASS_FFREE";
		case XED_ICLASS_FFREEP:
			return "XED_ICLASS_FFREEP";
		case XED_ICLASS_FIADD:
			return "XED_ICLASS_FIADD";
		case XED_ICLASS_FICOM:
			return "XED_ICLASS_FICOM";
		case XED_ICLASS_FICOMP:
			return "XED_ICLASS_FICOMP";
		case XED_ICLASS_FIDIV:
			return "XED_ICLASS_FIDIV";
		case XED_ICLASS_FIDIVR:
			return "XED_ICLASS_FIDIVR";
		case XED_ICLASS_FILD:
			return "XED_ICLASS_FILD";
		case XED_ICLASS_FIMUL:
			return "XED_ICLASS_FIMUL";
		case XED_ICLASS_FINCSTP:
			return "XED_ICLASS_FINCSTP";
		case XED_ICLASS_FIST:
			return "XED_ICLASS_FIST";
		case XED_ICLASS_FISTP:
			return "XED_ICLASS_FISTP";
		case XED_ICLASS_FISTTP:
			return "XED_ICLASS_FISTTP";
		case XED_ICLASS_FISUB:
			return "XED_ICLASS_FISUB";
		case XED_ICLASS_FISUBR:
			return "XED_ICLASS_FISUBR";
		case XED_ICLASS_FLD:
			return "XED_ICLASS_FLD";
		case XED_ICLASS_FLD1:
			return "XED_ICLASS_FLD1";
		case XED_ICLASS_FLDCW:
			return "XED_ICLASS_FLDCW";
		case XED_ICLASS_FLDENV:
			return "XED_ICLASS_FLDENV";
		case XED_ICLASS_FLDL2E:
			return "XED_ICLASS_FLDL2E";
		case XED_ICLASS_FLDL2T:
			return "XED_ICLASS_FLDL2T";
		case XED_ICLASS_FLDLG2:
			return "XED_ICLASS_FLDLG2";
		case XED_ICLASS_FLDLN2:
			return "XED_ICLASS_FLDLN2";
		case XED_ICLASS_FLDPI:
			return "XED_ICLASS_FLDPI";
		case XED_ICLASS_FLDZ:
			return "XED_ICLASS_FLDZ";
		case XED_ICLASS_FMUL:
			return "XED_ICLASS_FMUL";
		case XED_ICLASS_FMULP:
			return "XED_ICLASS_FMULP";
		case XED_ICLASS_FNCLEX:
			return "XED_ICLASS_FNCLEX";
		case XED_ICLASS_FNINIT:
			return "XED_ICLASS_FNINIT";
		case XED_ICLASS_FNOP:
			return "XED_ICLASS_FNOP";
		case XED_ICLASS_FNSAVE:
			return "XED_ICLASS_FNSAVE";
		case XED_ICLASS_FNSTCW:
			return "XED_ICLASS_FNSTCW";
		case XED_ICLASS_FNSTENV:
			return "XED_ICLASS_FNSTENV";
		case XED_ICLASS_FNSTSW:
			return "XED_ICLASS_FNSTSW";
		case XED_ICLASS_FPATAN:
			return "XED_ICLASS_FPATAN";
		case XED_ICLASS_FPREM:
			return "XED_ICLASS_FPREM";
		case XED_ICLASS_FPREM1:
			return "XED_ICLASS_FPREM1";
		case XED_ICLASS_FPTAN:
			return "XED_ICLASS_FPTAN";
		case XED_ICLASS_FRNDINT:
			return "XED_ICLASS_FRNDINT";
		case XED_ICLASS_FRSTOR:
			return "XED_ICLASS_FRSTOR";
		case XED_ICLASS_FSCALE:
			return "XED_ICLASS_FSCALE";
		case XED_ICLASS_FSETPM287_NOP:
			return "XED_ICLASS_FSETPM287_NOP";
		case XED_ICLASS_FSIN:
			return "XED_ICLASS_FSIN";
		case XED_ICLASS_FSINCOS:
			return "XED_ICLASS_FSINCOS";
		case XED_ICLASS_FSQRT:
			return "XED_ICLASS_FSQRT";
		case XED_ICLASS_FST:
			return "XED_ICLASS_FST";
		case XED_ICLASS_FSTP:
			return "XED_ICLASS_FSTP";
		case XED_ICLASS_FSUB:
			return "XED_ICLASS_FSUB";
		case XED_ICLASS_FSUBP:
			return "XED_ICLASS_FSUBP";
		case XED_ICLASS_FSUBR:
			return "XED_ICLASS_FSUBR";
		case XED_ICLASS_FSUBRP:
			return "XED_ICLASS_FSUBRP";
		case XED_ICLASS_FTST:
			return "XED_ICLASS_FTST";
		case XED_ICLASS_FUCOM:
			return "XED_ICLASS_FUCOM";
		case XED_ICLASS_FUCOMI:
			return "XED_ICLASS_FUCOMI";
		case XED_ICLASS_FUCOMIP:
			return "XED_ICLASS_FUCOMIP";
		case XED_ICLASS_FUCOMP:
			return "XED_ICLASS_FUCOMP";
		case XED_ICLASS_FUCOMPP:
			return "XED_ICLASS_FUCOMPP";
		case XED_ICLASS_FWAIT:
			return "XED_ICLASS_FWAIT";
		case XED_ICLASS_FXAM:
			return "XED_ICLASS_FXAM";
		case XED_ICLASS_FXCH:
			return "XED_ICLASS_FXCH";
		case XED_ICLASS_FXRSTOR:
			return "XED_ICLASS_FXRSTOR";
		case XED_ICLASS_FXSAVE:
			return "XED_ICLASS_FXSAVE";
		case XED_ICLASS_FXTRACT:
			return "XED_ICLASS_FXTRACT";
		case XED_ICLASS_FYL2X:
			return "XED_ICLASS_FYL2X";
		case XED_ICLASS_FYL2XP1:
			return "XED_ICLASS_FYL2XP1";
		case XED_ICLASS_HADDPD:
			return "XED_ICLASS_HADDPD";
		case XED_ICLASS_HADDPS:
			return "XED_ICLASS_HADDPS";
		case XED_ICLASS_HLT:
			return "XED_ICLASS_HLT";
		case XED_ICLASS_HSUBPD:
			return "XED_ICLASS_HSUBPD";
		case XED_ICLASS_HSUBPS:
			return "XED_ICLASS_HSUBPS";
		case XED_ICLASS_IDIV:
			return "XED_ICLASS_IDIV";
		case XED_ICLASS_IMUL:
			return "XED_ICLASS_IMUL";
		case XED_ICLASS_IN:
			return "XED_ICLASS_IN";
		case XED_ICLASS_INC:
			return "XED_ICLASS_INC";
		case XED_ICLASS_INSB:
			return "XED_ICLASS_INSB";
		case XED_ICLASS_INSD:
			return "XED_ICLASS_INSD";
		case XED_ICLASS_INSERTPS:
			return "XED_ICLASS_INSERTPS";
		case XED_ICLASS_INSERTQ:
			return "XED_ICLASS_INSERTQ";
		case XED_ICLASS_INSW:
			return "XED_ICLASS_INSW";
		case XED_ICLASS_INT:
			return "XED_ICLASS_INT";
		case XED_ICLASS_INT1:
			return "XED_ICLASS_INT1";
		case XED_ICLASS_INT3:
			return "XED_ICLASS_INT3";
		case XED_ICLASS_INTO:
			return "XED_ICLASS_INTO";
		case XED_ICLASS_INVD:
			return "XED_ICLASS_INVD";
		case XED_ICLASS_INVEPT:
			return "XED_ICLASS_INVEPT";
		case XED_ICLASS_INVLPG:
			return "XED_ICLASS_INVLPG";
		case XED_ICLASS_INVLPGA:
			return "XED_ICLASS_INVLPGA";
		case XED_ICLASS_INVVPID:
			return "XED_ICLASS_INVVPID";
		case XED_ICLASS_IRET:
			return "XED_ICLASS_IRET";
		case XED_ICLASS_IRETD:
			return "XED_ICLASS_IRETD";
		case XED_ICLASS_IRETQ:
			return "XED_ICLASS_IRETQ";
		case XED_ICLASS_JB:
			return "XED_ICLASS_JB";
		case XED_ICLASS_JBE:
			return "XED_ICLASS_JBE";
		case XED_ICLASS_JL:
			return "XED_ICLASS_JL";
		case XED_ICLASS_JLE:
			return "XED_ICLASS_JLE";
		case XED_ICLASS_JMP:
			return "XED_ICLASS_JMP";
		case XED_ICLASS_JMP_FAR:
			return "XED_ICLASS_JMP_FAR";
		case XED_ICLASS_JNB:
			return "XED_ICLASS_JNB";
		case XED_ICLASS_JNBE:
			return "XED_ICLASS_JNBE";
		case XED_ICLASS_JNL:
			return "XED_ICLASS_JNL";
		case XED_ICLASS_JNLE:
			return "XED_ICLASS_JNLE";
		case XED_ICLASS_JNO:
			return "XED_ICLASS_JNO";
		case XED_ICLASS_JNP:
			return "XED_ICLASS_JNP";
		case XED_ICLASS_JNS:
			return "XED_ICLASS_JNS";
		case XED_ICLASS_JNZ:
			return "XED_ICLASS_JNZ";
		case XED_ICLASS_JO:
			return "XED_ICLASS_JO";
		case XED_ICLASS_JP:
			return "XED_ICLASS_JP";
		case XED_ICLASS_JRCXZ:
			return "XED_ICLASS_JRCXZ";
		case XED_ICLASS_JS:
			return "XED_ICLASS_JS";
		case XED_ICLASS_JZ:
			return "XED_ICLASS_JZ";
		case XED_ICLASS_LAHF:
			return "XED_ICLASS_LAHF";
		case XED_ICLASS_LAR:
			return "XED_ICLASS_LAR";
		case XED_ICLASS_LDDQU:
			return "XED_ICLASS_LDDQU";
		case XED_ICLASS_LDMXCSR:
			return "XED_ICLASS_LDMXCSR";
		case XED_ICLASS_LDS:
			return "XED_ICLASS_LDS";
		case XED_ICLASS_LEA:
			return "XED_ICLASS_LEA";
		case XED_ICLASS_LEAVE:
			return "XED_ICLASS_LEAVE";
		case XED_ICLASS_LES:
			return "XED_ICLASS_LES";
		case XED_ICLASS_LFENCE:
			return "XED_ICLASS_LFENCE";
		case XED_ICLASS_LFS:
			return "XED_ICLASS_LFS";
		case XED_ICLASS_LGDT:
			return "XED_ICLASS_LGDT";
		case XED_ICLASS_LGS:
			return "XED_ICLASS_LGS";
		case XED_ICLASS_LIDT:
			return "XED_ICLASS_LIDT";
		case XED_ICLASS_LLDT:
			return "XED_ICLASS_LLDT";
		case XED_ICLASS_LMSW:
			return "XED_ICLASS_LMSW";
		case XED_ICLASS_LODSB:
			return "XED_ICLASS_LODSB";
		case XED_ICLASS_LODSD:
			return "XED_ICLASS_LODSD";
		case XED_ICLASS_LODSQ:
			return "XED_ICLASS_LODSQ";
		case XED_ICLASS_LODSW:
			return "XED_ICLASS_LODSW";
		case XED_ICLASS_LOOP:
			return "XED_ICLASS_LOOP";
		case XED_ICLASS_LOOPE:
			return "XED_ICLASS_LOOPE";
		case XED_ICLASS_LOOPNE:
			return "XED_ICLASS_LOOPNE";
		case XED_ICLASS_LSL:
			return "XED_ICLASS_LSL";
		case XED_ICLASS_LSS:
			return "XED_ICLASS_LSS";
		case XED_ICLASS_LTR:
			return "XED_ICLASS_LTR";
		case XED_ICLASS_LZCNT:
			return "XED_ICLASS_LZCNT";
		case XED_ICLASS_MASKMOVDQU:
			return "XED_ICLASS_MASKMOVDQU";
		case XED_ICLASS_MASKMOVQ:
			return "XED_ICLASS_MASKMOVQ";
		case XED_ICLASS_MAXPD:
			return "XED_ICLASS_MAXPD";
		case XED_ICLASS_MAXPS:
			return "XED_ICLASS_MAXPS";
		case XED_ICLASS_MAXSD:
			return "XED_ICLASS_MAXSD";
		case XED_ICLASS_MAXSS:
			return "XED_ICLASS_MAXSS";
		case XED_ICLASS_MFENCE:
			return "XED_ICLASS_MFENCE";
		case XED_ICLASS_MINPD:
			return "XED_ICLASS_MINPD";
		case XED_ICLASS_MINPS:
			return "XED_ICLASS_MINPS";
		case XED_ICLASS_MINSD:
			return "XED_ICLASS_MINSD";
		case XED_ICLASS_MINSS:
			return "XED_ICLASS_MINSS";
		case XED_ICLASS_MONITOR:
			return "XED_ICLASS_MONITOR";
		case XED_ICLASS_MOV:
			return "XED_ICLASS_MOV";
		case XED_ICLASS_MOVAPD:
			return "XED_ICLASS_MOVAPD";
		case XED_ICLASS_MOVAPS:
			return "XED_ICLASS_MOVAPS";
		case XED_ICLASS_MOVBE:
			return "XED_ICLASS_MOVBE";
		case XED_ICLASS_MOVD:
			return "XED_ICLASS_MOVD";
		case XED_ICLASS_MOVDDUP:
			return "XED_ICLASS_MOVDDUP";
		case XED_ICLASS_MOVDQ2Q:
			return "XED_ICLASS_MOVDQ2Q";
		case XED_ICLASS_MOVDQA:
			return "XED_ICLASS_MOVDQA";
		case XED_ICLASS_MOVDQU:
			return "XED_ICLASS_MOVDQU";
		case XED_ICLASS_MOVHLPS:
			return "XED_ICLASS_MOVHLPS";
		case XED_ICLASS_MOVHPD:
			return "XED_ICLASS_MOVHPD";
		case XED_ICLASS_MOVHPS:
			return "XED_ICLASS_MOVHPS";
		case XED_ICLASS_MOVLHPS:
			return "XED_ICLASS_MOVLHPS";
		case XED_ICLASS_MOVLPD:
			return "XED_ICLASS_MOVLPD";
		case XED_ICLASS_MOVLPS:
			return "XED_ICLASS_MOVLPS";
		case XED_ICLASS_MOVMSKPD:
			return "XED_ICLASS_MOVMSKPD";
		case XED_ICLASS_MOVMSKPS:
			return "XED_ICLASS_MOVMSKPS";
		case XED_ICLASS_MOVNTDQ:
			return "XED_ICLASS_MOVNTDQ";
		case XED_ICLASS_MOVNTDQA:
			return "XED_ICLASS_MOVNTDQA";
		case XED_ICLASS_MOVNTI:
			return "XED_ICLASS_MOVNTI";
		case XED_ICLASS_MOVNTPD:
			return "XED_ICLASS_MOVNTPD";
		case XED_ICLASS_MOVNTPS:
			return "XED_ICLASS_MOVNTPS";
		case XED_ICLASS_MOVNTQ:
			return "XED_ICLASS_MOVNTQ";
		case XED_ICLASS_MOVNTSD:
			return "XED_ICLASS_MOVNTSD";
		case XED_ICLASS_MOVNTSS:
			return "XED_ICLASS_MOVNTSS";
		case XED_ICLASS_MOVQ:
			return "XED_ICLASS_MOVQ";
		case XED_ICLASS_MOVQ2DQ:
			return "XED_ICLASS_MOVQ2DQ";
		case XED_ICLASS_MOVSB:
			return "XED_ICLASS_MOVSB";
		case XED_ICLASS_MOVSD:
			return "XED_ICLASS_MOVSD";
		case XED_ICLASS_MOVSD_XMM:
			return "XED_ICLASS_MOVSD_XMM";
		case XED_ICLASS_MOVSHDUP:
			return "XED_ICLASS_MOVSHDUP";
		case XED_ICLASS_MOVSLDUP:
			return "XED_ICLASS_MOVSLDUP";
		case XED_ICLASS_MOVSQ:
			return "XED_ICLASS_MOVSQ";
		case XED_ICLASS_MOVSS:
			return "XED_ICLASS_MOVSS";
		case XED_ICLASS_MOVSW:
			return "XED_ICLASS_MOVSW";
		case XED_ICLASS_MOVSX:
			return "XED_ICLASS_MOVSX";
		case XED_ICLASS_MOVSXD:
			return "XED_ICLASS_MOVSXD";
		case XED_ICLASS_MOVUPD:
			return "XED_ICLASS_MOVUPD";
		case XED_ICLASS_MOVUPS:
			return "XED_ICLASS_MOVUPS";
		case XED_ICLASS_MOVZX:
			return "XED_ICLASS_MOVZX";
		case XED_ICLASS_MOV_CR:
			return "XED_ICLASS_MOV_CR";
		case XED_ICLASS_MOV_DR:
			return "XED_ICLASS_MOV_DR";
		case XED_ICLASS_MPSADBW:
			return "XED_ICLASS_MPSADBW";
		case XED_ICLASS_MUL:
			return "XED_ICLASS_MUL";
		case XED_ICLASS_MULPD:
			return "XED_ICLASS_MULPD";
		case XED_ICLASS_MULPS:
			return "XED_ICLASS_MULPS";
		case XED_ICLASS_MULSD:
			return "XED_ICLASS_MULSD";
		case XED_ICLASS_MULSS:
			return "XED_ICLASS_MULSS";
		case XED_ICLASS_MWAIT:
			return "XED_ICLASS_MWAIT";
		case XED_ICLASS_NEG:
			return "XED_ICLASS_NEG";
		case XED_ICLASS_NOP:
			return "XED_ICLASS_NOP";
		case XED_ICLASS_NOP2:
			return "XED_ICLASS_NOP2";
		case XED_ICLASS_NOP3:
			return "XED_ICLASS_NOP3";
		case XED_ICLASS_NOP4:
			return "XED_ICLASS_NOP4";
		case XED_ICLASS_NOP5:
			return "XED_ICLASS_NOP5";
		case XED_ICLASS_NOP6:
			return "XED_ICLASS_NOP6";
		case XED_ICLASS_NOP7:
			return "XED_ICLASS_NOP7";
		case XED_ICLASS_NOP8:
			return "XED_ICLASS_NOP8";
		case XED_ICLASS_NOP9:
			return "XED_ICLASS_NOP9";
		case XED_ICLASS_NOT:
			return "XED_ICLASS_NOT";
		case XED_ICLASS_OR:
			return "XED_ICLASS_OR";
		case XED_ICLASS_ORPD:
			return "XED_ICLASS_ORPD";
		case XED_ICLASS_ORPS:
			return "XED_ICLASS_ORPS";
		case XED_ICLASS_OUT:
			return "XED_ICLASS_OUT";
		case XED_ICLASS_OUTSB:
			return "XED_ICLASS_OUTSB";
		case XED_ICLASS_OUTSD:
			return "XED_ICLASS_OUTSD";
		case XED_ICLASS_OUTSW:
			return "XED_ICLASS_OUTSW";
		case XED_ICLASS_PABSB:
			return "XED_ICLASS_PABSB";
		case XED_ICLASS_PABSD:
			return "XED_ICLASS_PABSD";
		case XED_ICLASS_PABSW:
			return "XED_ICLASS_PABSW";
		case XED_ICLASS_PACKSSDW:
			return "XED_ICLASS_PACKSSDW";
		case XED_ICLASS_PACKSSWB:
			return "XED_ICLASS_PACKSSWB";
		case XED_ICLASS_PACKUSDW:
			return "XED_ICLASS_PACKUSDW";
		case XED_ICLASS_PACKUSWB:
			return "XED_ICLASS_PACKUSWB";
		case XED_ICLASS_PADDB:
			return "XED_ICLASS_PADDB";
		case XED_ICLASS_PADDD:
			return "XED_ICLASS_PADDD";
		case XED_ICLASS_PADDQ:
			return "XED_ICLASS_PADDQ";
		case XED_ICLASS_PADDSB:
			return "XED_ICLASS_PADDSB";
		case XED_ICLASS_PADDSW:
			return "XED_ICLASS_PADDSW";
		case XED_ICLASS_PADDUSB:
			return "XED_ICLASS_PADDUSB";
		case XED_ICLASS_PADDUSW:
			return "XED_ICLASS_PADDUSW";
		case XED_ICLASS_PADDW:
			return "XED_ICLASS_PADDW";
		case XED_ICLASS_PALIGNR:
			return "XED_ICLASS_PALIGNR";
		case XED_ICLASS_PAND:
			return "XED_ICLASS_PAND";
		case XED_ICLASS_PANDN:
			return "XED_ICLASS_PANDN";
		case XED_ICLASS_PAUSE:
			return "XED_ICLASS_PAUSE";
		case XED_ICLASS_PAVGB:
			return "XED_ICLASS_PAVGB";
		case XED_ICLASS_PAVGUSB:
			return "XED_ICLASS_PAVGUSB";
		case XED_ICLASS_PAVGW:
			return "XED_ICLASS_PAVGW";
		case XED_ICLASS_PBLENDVB:
			return "XED_ICLASS_PBLENDVB";
		case XED_ICLASS_PBLENDW:
			return "XED_ICLASS_PBLENDW";
		case XED_ICLASS_PCLMULQDQ:
			return "XED_ICLASS_PCLMULQDQ";
		case XED_ICLASS_PCMPEQB:
			return "XED_ICLASS_PCMPEQB";
		case XED_ICLASS_PCMPEQD:
			return "XED_ICLASS_PCMPEQD";
		case XED_ICLASS_PCMPEQQ:
			return "XED_ICLASS_PCMPEQQ";
		case XED_ICLASS_PCMPEQW:
			return "XED_ICLASS_PCMPEQW";
		case XED_ICLASS_PCMPESTRI:
			return "XED_ICLASS_PCMPESTRI";
		case XED_ICLASS_PCMPESTRM:
			return "XED_ICLASS_PCMPESTRM";
		case XED_ICLASS_PCMPGTB:
			return "XED_ICLASS_PCMPGTB";
		case XED_ICLASS_PCMPGTD:
			return "XED_ICLASS_PCMPGTD";
		case XED_ICLASS_PCMPGTQ:
			return "XED_ICLASS_PCMPGTQ";
		case XED_ICLASS_PCMPGTW:
			return "XED_ICLASS_PCMPGTW";
		case XED_ICLASS_PCMPISTRI:
			return "XED_ICLASS_PCMPISTRI";
		case XED_ICLASS_PCMPISTRM:
			return "XED_ICLASS_PCMPISTRM";
		case XED_ICLASS_PEXTRB:
			return "XED_ICLASS_PEXTRB";
		case XED_ICLASS_PEXTRD:
			return "XED_ICLASS_PEXTRD";
		case XED_ICLASS_PEXTRQ:
			return "XED_ICLASS_PEXTRQ";
		case XED_ICLASS_PEXTRW:
			return "XED_ICLASS_PEXTRW";
		case XED_ICLASS_PF2ID:
			return "XED_ICLASS_PF2ID";
		case XED_ICLASS_PF2IW:
			return "XED_ICLASS_PF2IW";
		case XED_ICLASS_PFACC:
			return "XED_ICLASS_PFACC";
		case XED_ICLASS_PFADD:
			return "XED_ICLASS_PFADD";
		case XED_ICLASS_PFCMPEQ:
			return "XED_ICLASS_PFCMPEQ";
		case XED_ICLASS_PFCMPGE:
			return "XED_ICLASS_PFCMPGE";
		case XED_ICLASS_PFCMPGT:
			return "XED_ICLASS_PFCMPGT";
		case XED_ICLASS_PFCPIT1:
			return "XED_ICLASS_PFCPIT1";
		case XED_ICLASS_PFMAX:
			return "XED_ICLASS_PFMAX";
		case XED_ICLASS_PFMIN:
			return "XED_ICLASS_PFMIN";
		case XED_ICLASS_PFMUL:
			return "XED_ICLASS_PFMUL";
		case XED_ICLASS_PFNACC:
			return "XED_ICLASS_PFNACC";
		case XED_ICLASS_PFPNACC:
			return "XED_ICLASS_PFPNACC";
		case XED_ICLASS_PFRCP:
			return "XED_ICLASS_PFRCP";
		case XED_ICLASS_PFRCPIT2:
			return "XED_ICLASS_PFRCPIT2";
		case XED_ICLASS_PFRSQIT1:
			return "XED_ICLASS_PFRSQIT1";
		case XED_ICLASS_PFSQRT:
			return "XED_ICLASS_PFSQRT";
		case XED_ICLASS_PFSUB:
			return "XED_ICLASS_PFSUB";
		case XED_ICLASS_PFSUBR:
			return "XED_ICLASS_PFSUBR";
		case XED_ICLASS_PHADDD:
			return "XED_ICLASS_PHADDD";
		case XED_ICLASS_PHADDSW:
			return "XED_ICLASS_PHADDSW";
		case XED_ICLASS_PHADDW:
			return "XED_ICLASS_PHADDW";
		case XED_ICLASS_PHMINPOSUW:
			return "XED_ICLASS_PHMINPOSUW";
		case XED_ICLASS_PHSUBD:
			return "XED_ICLASS_PHSUBD";
		case XED_ICLASS_PHSUBSW:
			return "XED_ICLASS_PHSUBSW";
		case XED_ICLASS_PHSUBW:
			return "XED_ICLASS_PHSUBW";
		case XED_ICLASS_PI2FD:
			return "XED_ICLASS_PI2FD";
		case XED_ICLASS_PI2FW:
			return "XED_ICLASS_PI2FW";
		case XED_ICLASS_PINSRB:
			return "XED_ICLASS_PINSRB";
		case XED_ICLASS_PINSRD:
			return "XED_ICLASS_PINSRD";
		case XED_ICLASS_PINSRQ:
			return "XED_ICLASS_PINSRQ";
		case XED_ICLASS_PINSRW:
			return "XED_ICLASS_PINSRW";
		case XED_ICLASS_PMADDUBSW:
			return "XED_ICLASS_PMADDUBSW";
		case XED_ICLASS_PMADDWD:
			return "XED_ICLASS_PMADDWD";
		case XED_ICLASS_PMAXSB:
			return "XED_ICLASS_PMAXSB";
		case XED_ICLASS_PMAXSD:
			return "XED_ICLASS_PMAXSD";
		case XED_ICLASS_PMAXSW:
			return "XED_ICLASS_PMAXSW";
		case XED_ICLASS_PMAXUB:
			return "XED_ICLASS_PMAXUB";
		case XED_ICLASS_PMAXUD:
			return "XED_ICLASS_PMAXUD";
		case XED_ICLASS_PMAXUW:
			return "XED_ICLASS_PMAXUW";
		case XED_ICLASS_PMINSB:
			return "XED_ICLASS_PMINSB";
		case XED_ICLASS_PMINSD:
			return "XED_ICLASS_PMINSD";
		case XED_ICLASS_PMINSW:
			return "XED_ICLASS_PMINSW";
		case XED_ICLASS_PMINUB:
			return "XED_ICLASS_PMINUB";
		case XED_ICLASS_PMINUD:
			return "XED_ICLASS_PMINUD";
		case XED_ICLASS_PMINUW:
			return "XED_ICLASS_PMINUW";
		case XED_ICLASS_PMOVMSKB:
			return "XED_ICLASS_PMOVMSKB";
		case XED_ICLASS_PMOVSXBD:
			return "XED_ICLASS_PMOVSXBD";
		case XED_ICLASS_PMOVSXBQ:
			return "XED_ICLASS_PMOVSXBQ";
		case XED_ICLASS_PMOVSXBW:
			return "XED_ICLASS_PMOVSXBW";
		case XED_ICLASS_PMOVSXDQ:
			return "XED_ICLASS_PMOVSXDQ";
		case XED_ICLASS_PMOVSXWD:
			return "XED_ICLASS_PMOVSXWD";
		case XED_ICLASS_PMOVSXWQ:
			return "XED_ICLASS_PMOVSXWQ";
		case XED_ICLASS_PMOVZXBD:
			return "XED_ICLASS_PMOVZXBD";
		case XED_ICLASS_PMOVZXBQ:
			return "XED_ICLASS_PMOVZXBQ";
		case XED_ICLASS_PMOVZXBW:
			return "XED_ICLASS_PMOVZXBW";
		case XED_ICLASS_PMOVZXDQ:
			return "XED_ICLASS_PMOVZXDQ";
		case XED_ICLASS_PMOVZXWD:
			return "XED_ICLASS_PMOVZXWD";
		case XED_ICLASS_PMOVZXWQ:
			return "XED_ICLASS_PMOVZXWQ";
		case XED_ICLASS_PMULDQ:
			return "XED_ICLASS_PMULDQ";
		case XED_ICLASS_PMULHRSW:
			return "XED_ICLASS_PMULHRSW";
		case XED_ICLASS_PMULHRW:
			return "XED_ICLASS_PMULHRW";
		case XED_ICLASS_PMULHUW:
			return "XED_ICLASS_PMULHUW";
		case XED_ICLASS_PMULHW:
			return "XED_ICLASS_PMULHW";
		case XED_ICLASS_PMULLD:
			return "XED_ICLASS_PMULLD";
		case XED_ICLASS_PMULLW:
			return "XED_ICLASS_PMULLW";
		case XED_ICLASS_PMULUDQ:
			return "XED_ICLASS_PMULUDQ";
		case XED_ICLASS_POP:
			return "XED_ICLASS_POP";
		case XED_ICLASS_POPA:
			return "XED_ICLASS_POPA";
		case XED_ICLASS_POPAD:
			return "XED_ICLASS_POPAD";
		case XED_ICLASS_POPCNT:
			return "XED_ICLASS_POPCNT";
		case XED_ICLASS_POPF:
			return "XED_ICLASS_POPF";
		case XED_ICLASS_POPFD:
			return "XED_ICLASS_POPFD";
		case XED_ICLASS_POPFQ:
			return "XED_ICLASS_POPFQ";
		case XED_ICLASS_POR:
			return "XED_ICLASS_POR";
		case XED_ICLASS_PREFETCHNTA:
			return "XED_ICLASS_PREFETCHNTA";
		case XED_ICLASS_PREFETCHT0:
			return "XED_ICLASS_PREFETCHT0";
		case XED_ICLASS_PREFETCHT1:
			return "XED_ICLASS_PREFETCHT1";
		case XED_ICLASS_PREFETCHT2:
			return "XED_ICLASS_PREFETCHT2";
		case XED_ICLASS_PREFETCH_EXCLUSIVE:
			return "XED_ICLASS_PREFETCH_EXCLUSIVE";
		//case XED_ICLASS_PREFETCH_MODIFIED:
		//	return "XED_ICLASS_PREFETCH_MODIFIED";
		case XED_ICLASS_PREFETCH_RESERVED:
			return "XED_ICLASS_PREFETCH_RESERVED";
		case XED_ICLASS_PSADBW:
			return "XED_ICLASS_PSADBW";
		case XED_ICLASS_PSHUFB:
			return "XED_ICLASS_PSHUFB";
		case XED_ICLASS_PSHUFD:
			return "XED_ICLASS_PSHUFD";
		case XED_ICLASS_PSHUFHW:
			return "XED_ICLASS_PSHUFHW";
		case XED_ICLASS_PSHUFLW:
			return "XED_ICLASS_PSHUFLW";
		case XED_ICLASS_PSHUFW:
			return "XED_ICLASS_PSHUFW";
		case XED_ICLASS_PSIGNB:
			return "XED_ICLASS_PSIGNB";
		case XED_ICLASS_PSIGND:
			return "XED_ICLASS_PSIGND";
		case XED_ICLASS_PSIGNW:
			return "XED_ICLASS_PSIGNW";
		case XED_ICLASS_PSLLD:
			return "XED_ICLASS_PSLLD";
		case XED_ICLASS_PSLLDQ:
			return "XED_ICLASS_PSLLDQ";
		case XED_ICLASS_PSLLQ:
			return "XED_ICLASS_PSLLQ";
		case XED_ICLASS_PSLLW:
			return "XED_ICLASS_PSLLW";
		case XED_ICLASS_PSRAD:
			return "XED_ICLASS_PSRAD";
		case XED_ICLASS_PSRAW:
			return "XED_ICLASS_PSRAW";
		case XED_ICLASS_PSRLD:
			return "XED_ICLASS_PSRLD";
		case XED_ICLASS_PSRLDQ:
			return "XED_ICLASS_PSRLDQ";
		case XED_ICLASS_PSRLQ:
			return "XED_ICLASS_PSRLQ";
		case XED_ICLASS_PSRLW:
			return "XED_ICLASS_PSRLW";
		case XED_ICLASS_PSUBB:
			return "XED_ICLASS_PSUBB";
		case XED_ICLASS_PSUBD:
			return "XED_ICLASS_PSUBD";
		case XED_ICLASS_PSUBQ:
			return "XED_ICLASS_PSUBQ";
		case XED_ICLASS_PSUBSB:
			return "XED_ICLASS_PSUBSB";
		case XED_ICLASS_PSUBSW:
			return "XED_ICLASS_PSUBSW";
		case XED_ICLASS_PSUBUSB:
			return "XED_ICLASS_PSUBUSB";
		case XED_ICLASS_PSUBUSW:
			return "XED_ICLASS_PSUBUSW";
		case XED_ICLASS_PSUBW:
			return "XED_ICLASS_PSUBW";
		case XED_ICLASS_PSWAPD:
			return "XED_ICLASS_PSWAPD";
		case XED_ICLASS_PTEST:
			return "XED_ICLASS_PTEST";
		case XED_ICLASS_PUNPCKHBW:
			return "XED_ICLASS_PUNPCKHBW";
		case XED_ICLASS_PUNPCKHDQ:
			return "XED_ICLASS_PUNPCKHDQ";
		case XED_ICLASS_PUNPCKHQDQ:
			return "XED_ICLASS_PUNPCKHQDQ";
		case XED_ICLASS_PUNPCKHWD:
			return "XED_ICLASS_PUNPCKHWD";
		case XED_ICLASS_PUNPCKLBW:
			return "XED_ICLASS_PUNPCKLBW";
		case XED_ICLASS_PUNPCKLDQ:
			return "XED_ICLASS_PUNPCKLDQ";
		case XED_ICLASS_PUNPCKLQDQ:
			return "XED_ICLASS_PUNPCKLQDQ";
		case XED_ICLASS_PUNPCKLWD:
			return "XED_ICLASS_PUNPCKLWD";
		case XED_ICLASS_PUSH:
			return "XED_ICLASS_PUSH";
		case XED_ICLASS_PUSHA:
			return "XED_ICLASS_PUSHA";
		case XED_ICLASS_PUSHAD:
			return "XED_ICLASS_PUSHAD";
		case XED_ICLASS_PUSHF:
			return "XED_ICLASS_PUSHF";
		case XED_ICLASS_PUSHFD:
			return "XED_ICLASS_PUSHFD";
		case XED_ICLASS_PUSHFQ:
			return "XED_ICLASS_PUSHFQ";
		case XED_ICLASS_PXOR:
			return "XED_ICLASS_PXOR";
		case XED_ICLASS_RCL:
			return "XED_ICLASS_RCL";
		case XED_ICLASS_RCPPS:
			return "XED_ICLASS_RCPPS";
		case XED_ICLASS_RCPSS:
			return "XED_ICLASS_RCPSS";
		case XED_ICLASS_RCR:
			return "XED_ICLASS_RCR";
		case XED_ICLASS_RDMSR:
			return "XED_ICLASS_RDMSR";
		case XED_ICLASS_RDPMC:
			return "XED_ICLASS_RDPMC";
		case XED_ICLASS_RDTSC:
			return "XED_ICLASS_RDTSC";
		case XED_ICLASS_RDTSCP:
			return "XED_ICLASS_RDTSCP";
		case XED_ICLASS_RET_FAR:
			return "XED_ICLASS_RET_FAR";
		case XED_ICLASS_RET_NEAR:
			return "XED_ICLASS_RET_NEAR";
		case XED_ICLASS_ROL:
			return "XED_ICLASS_ROL";
		case XED_ICLASS_ROR:
			return "XED_ICLASS_ROR";
		case XED_ICLASS_ROUNDPD:
			return "XED_ICLASS_ROUNDPD";
		case XED_ICLASS_ROUNDPS:
			return "XED_ICLASS_ROUNDPS";
		case XED_ICLASS_ROUNDSD:
			return "XED_ICLASS_ROUNDSD";
		case XED_ICLASS_ROUNDSS:
			return "XED_ICLASS_ROUNDSS";
		case XED_ICLASS_RSM:
			return "XED_ICLASS_RSM";
		case XED_ICLASS_RSQRTPS:
			return "XED_ICLASS_RSQRTPS";
		case XED_ICLASS_RSQRTSS:
			return "XED_ICLASS_RSQRTSS";
		case XED_ICLASS_SAHF:
			return "XED_ICLASS_SAHF";
		case XED_ICLASS_SALC:
			return "XED_ICLASS_SALC";
		case XED_ICLASS_SAR:
			return "XED_ICLASS_SAR";
		case XED_ICLASS_SBB:
			return "XED_ICLASS_SBB";
		case XED_ICLASS_SCASB:
			return "XED_ICLASS_SCASB";
		case XED_ICLASS_SCASD:
			return "XED_ICLASS_SCASD";
		case XED_ICLASS_SCASQ:
			return "XED_ICLASS_SCASQ";
		case XED_ICLASS_SCASW:
			return "XED_ICLASS_SCASW";
		case XED_ICLASS_SETB:
			return "XED_ICLASS_SETB";
		case XED_ICLASS_SETBE:
			return "XED_ICLASS_SETBE";
		case XED_ICLASS_SETL:
			return "XED_ICLASS_SETL";
		case XED_ICLASS_SETLE:
			return "XED_ICLASS_SETLE";
		case XED_ICLASS_SETNB:
			return "XED_ICLASS_SETNB";
		case XED_ICLASS_SETNBE:
			return "XED_ICLASS_SETNBE";
		case XED_ICLASS_SETNL:
			return "XED_ICLASS_SETNL";
		case XED_ICLASS_SETNLE:
			return "XED_ICLASS_SETNLE";
		case XED_ICLASS_SETNO:
			return "XED_ICLASS_SETNO";
		case XED_ICLASS_SETNP:
			return "XED_ICLASS_SETNP";
		case XED_ICLASS_SETNS:
			return "XED_ICLASS_SETNS";
		case XED_ICLASS_SETNZ:
			return "XED_ICLASS_SETNZ";
		case XED_ICLASS_SETO:
			return "XED_ICLASS_SETO";
		case XED_ICLASS_SETP:
			return "XED_ICLASS_SETP";
		case XED_ICLASS_SETS:
			return "XED_ICLASS_SETS";
		case XED_ICLASS_SETZ:
			return "XED_ICLASS_SETZ";
		case XED_ICLASS_SFENCE:
			return "XED_ICLASS_SFENCE";
		case XED_ICLASS_SGDT:
			return "XED_ICLASS_SGDT";
		case XED_ICLASS_SHL:
			return "XED_ICLASS_SHL";
		case XED_ICLASS_SHLD:
			return "XED_ICLASS_SHLD";
		case XED_ICLASS_SHR:
			return "XED_ICLASS_SHR";
		case XED_ICLASS_SHRD:
			return "XED_ICLASS_SHRD";
		case XED_ICLASS_SHUFPD:
			return "XED_ICLASS_SHUFPD";
		case XED_ICLASS_SHUFPS:
			return "XED_ICLASS_SHUFPS";
		case XED_ICLASS_SIDT:
			return "XED_ICLASS_SIDT";
		case XED_ICLASS_SKINIT:
			return "XED_ICLASS_SKINIT";
		case XED_ICLASS_SLDT:
			return "XED_ICLASS_SLDT";
		case XED_ICLASS_SMSW:
			return "XED_ICLASS_SMSW";
		case XED_ICLASS_SQRTPD:
			return "XED_ICLASS_SQRTPD";
		case XED_ICLASS_SQRTPS:
			return "XED_ICLASS_SQRTPS";
		case XED_ICLASS_SQRTSD:
			return "XED_ICLASS_SQRTSD";
		case XED_ICLASS_SQRTSS:
			return "XED_ICLASS_SQRTSS";
		case XED_ICLASS_STC:
			return "XED_ICLASS_STC";
		case XED_ICLASS_STD:
			return "XED_ICLASS_STD";
		case XED_ICLASS_STGI:
			return "XED_ICLASS_STGI";
		case XED_ICLASS_STI:
			return "XED_ICLASS_STI";
		case XED_ICLASS_STMXCSR:
			return "XED_ICLASS_STMXCSR";
		case XED_ICLASS_STOSB:
			return "XED_ICLASS_STOSB";
		case XED_ICLASS_STOSD:
			return "XED_ICLASS_STOSD";
		case XED_ICLASS_STOSQ:
			return "XED_ICLASS_STOSQ";
		case XED_ICLASS_STOSW:
			return "XED_ICLASS_STOSW";
		case XED_ICLASS_STR:
			return "XED_ICLASS_STR";
		case XED_ICLASS_SUB:
			return "XED_ICLASS_SUB";
		case XED_ICLASS_SUBPD:
			return "XED_ICLASS_SUBPD";
		case XED_ICLASS_SUBPS:
			return "XED_ICLASS_SUBPS";
		case XED_ICLASS_SUBSD:
			return "XED_ICLASS_SUBSD";
		case XED_ICLASS_SUBSS:
			return "XED_ICLASS_SUBSS";
		case XED_ICLASS_SWAPGS:
			return "XED_ICLASS_SWAPGS";
		case XED_ICLASS_SYSCALL:
			return "XED_ICLASS_SYSCALL";
		case XED_ICLASS_SYSENTER:
			return "XED_ICLASS_SYSENTER";
		case XED_ICLASS_SYSEXIT:
			return "XED_ICLASS_SYSEXIT";
		case XED_ICLASS_SYSRET:
			return "XED_ICLASS_SYSRET";
		case XED_ICLASS_TEST:
			return "XED_ICLASS_TEST";
		case XED_ICLASS_UCOMISD:
			return "XED_ICLASS_UCOMISD";
		case XED_ICLASS_UCOMISS:
			return "XED_ICLASS_UCOMISS";
		case XED_ICLASS_UD2:
			return "XED_ICLASS_UD2";
		case XED_ICLASS_UNPCKHPD:
			return "XED_ICLASS_UNPCKHPD";
		case XED_ICLASS_UNPCKHPS:
			return "XED_ICLASS_UNPCKHPS";
		case XED_ICLASS_UNPCKLPD:
			return "XED_ICLASS_UNPCKLPD";
		case XED_ICLASS_UNPCKLPS:
			return "XED_ICLASS_UNPCKLPS";
		case XED_ICLASS_VERR:
			return "XED_ICLASS_VERR";
		case XED_ICLASS_VERW:
			return "XED_ICLASS_VERW";
		case XED_ICLASS_VMCALL:
			return "XED_ICLASS_VMCALL";
		case XED_ICLASS_VMCLEAR:
			return "XED_ICLASS_VMCLEAR";
		case XED_ICLASS_VMLAUNCH:
			return "XED_ICLASS_VMLAUNCH";
		case XED_ICLASS_VMLOAD:
			return "XED_ICLASS_VMLOAD";
		case XED_ICLASS_VMMCALL:
			return "XED_ICLASS_VMMCALL";
		case XED_ICLASS_VMPTRLD:
			return "XED_ICLASS_VMPTRLD";
		case XED_ICLASS_VMPTRST:
			return "XED_ICLASS_VMPTRST";
		case XED_ICLASS_VMREAD:
			return "XED_ICLASS_VMREAD";
		case XED_ICLASS_VMRESUME:
			return "XED_ICLASS_VMRESUME";
		case XED_ICLASS_VMRUN:
			return "XED_ICLASS_VMRUN";
		case XED_ICLASS_VMSAVE:
			return "XED_ICLASS_VMSAVE";
		case XED_ICLASS_VMWRITE:
			return "XED_ICLASS_VMWRITE";
		case XED_ICLASS_VMXOFF:
			return "XED_ICLASS_VMXOFF";
		case XED_ICLASS_VMXON:
			return "XED_ICLASS_VMXON";
		case XED_ICLASS_WBINVD:
			return "XED_ICLASS_WBINVD";
		case XED_ICLASS_WRMSR:
			return "XED_ICLASS_WRMSR";
		case XED_ICLASS_XADD:
			return "XED_ICLASS_XADD";
		case XED_ICLASS_XCHG:
			return "XED_ICLASS_XCHG";
		case XED_ICLASS_XGETBV:
			return "XED_ICLASS_XGETBV";
		case XED_ICLASS_XLAT:
			return "XED_ICLASS_XLAT";
		case XED_ICLASS_XOR:
			return "XED_ICLASS_XOR";
		case XED_ICLASS_XORPD:
			return "XED_ICLASS_XORPD";
		case XED_ICLASS_XORPS:
			return "XED_ICLASS_XORPS";
		case XED_ICLASS_XRSTOR:
			return "XED_ICLASS_XRSTOR";
		case XED_ICLASS_XSAVE:
			return "XED_ICLASS_XSAVE";
		case XED_ICLASS_XSETBV:
			return "XED_ICLASS_XSETBV";
		case XED_ICLASS_LAST:
			return "XED_ICLASS_LAST";
		default:
			return "ILLEGAL";
	}
}
