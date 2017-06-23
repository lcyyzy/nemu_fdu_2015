/*#include "cpu/exec/template-start.h"

#define instr shrd

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in = op_dest->val;
	DATA_TYPE out = op_src2->val;

	uint8_t count = op_src->val;
	count &= 0x1f;
	while(count != 0) {
		out >>= 1;
		out |= (in & 1) << ((DATA_BYTE << 3) - 1);
		in >>= 1;
		count --;
	}

	OPERAND_W(op_src2, out);

	print_asm("shrd" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

make_helper(concat(shrdi_, SUFFIX)) {
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"*/

#include "cpu/exec/template-start.h"

#define instr shrd

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	DATA_TYPE in,out;
	uint8_t count;
	if ((ops_decoded.opcode & 0xff) == 0xad)
	{
		//printf("%s %x %s %x %s %x\n",op_src->str,op_src->val,op_src2->str,op_src2->val,op_dest->str,op_dest->val);
		if (op_src2->reg == 7)
			op_src2->val = cpu.edx;
		op_src->val = cpu.ecx & 0xff;
		in = op_src2->val;
		out = op_dest->val;
		count = op_src->val;
		count &= 0x1f;
		while(count != 0) {
			out >>= 1;
			out |= (in & 1) << ((DATA_BYTE << 3) - 1);
			in >>= 1;
			count--;
		}
		OPERAND_W(op_dest, out);
		print_asm("shrd" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
		return;
	}
	in = op_dest->val;
	out = op_src2->val;
	count = op_src->val;
	count &= 0x1f;
	while(count != 0) {
		out >>= 1;
		out |= (in & 1) << ((DATA_BYTE << 3) - 1);
		in >>= 1;
		count--;
	}

	OPERAND_W(op_src2, out);

	print_asm("shrd" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

make_helper(concat(shrdi_, SUFFIX)) {
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);
	op_dest->val = REG(op_dest->reg);
	do_execute();
	return len + 1;
}

make_instr_helper(r2rm)
#endif

#include "cpu/exec/template-end.h"

