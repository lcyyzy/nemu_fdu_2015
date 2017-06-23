#include "cpu/exec/template-start.h"

#define instr shld

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in,out;
	uint8_t count;
	/*if ((ops_decoded.opcode&0xff)==0xa5)
	{
		op_src->val=cpu.ecx&0xff;
		in = op_src2->val;
		out = op_dest->val;
		count = op_src->val;
		count &= 0x1f;
		while(count != 0) {
			out <<= 1;
			out |= ((in >> ((DATA_BYTE << 3) - 1)) & 1);
			in <<= 1;
			count --;
		}
		OPERAND_W(op_dest, out);
		print_asm("shld" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
		return;
	}*/
	in = op_dest->val;
	out = op_src2->val;


	count = op_src->val;
	count &= 0x1f;
	while (count != 0) {
		out <<= 1;
		out |= ((in >> ((DATA_BYTE << 3) - 1)) & 1);
		in <<= 1;
		count --;
	}

	OPERAND_W(op_src2, out);
	print_asm("shld" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

make_helper(concat(shldi_, SUFFIX)) {
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}

make_helper(concat(shld_r2rm_, SUFFIX)) {
	int len = concat(decode_rm2r_, SUFFIX) (eip + 1);
	op_dest->val = REG(op_dest->reg);
	DATA_TYPE in = op_dest->val;
	DATA_TYPE out = op_src->val;
	uint8_t count;
	count = reg_b(R_CL);
	count &= 0x1f;
	while (count != 0) {
		out <<= 1;
		out |= ((in >> ((DATA_BYTE << 3) - 1)) & 1);
		in <<= 1;
		count --;
	}
	OPERAND_W(op_src, out);
	print_asm("shld" str(SUFFIX) " cl,%s,%s", op_dest->str, op_src->str);
	//make_instr_helper(r2rm)
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
