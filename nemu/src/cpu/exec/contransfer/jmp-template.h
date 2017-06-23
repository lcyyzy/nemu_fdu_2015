#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	if (ops_decoded.opcode == 0xFF) {
		//printf("%x\n", op_dest->val);
		if (DATA_BYTE == 2) {
			cpu.eip = op_src->val - concat(decode_rm_,SUFFIX)(cpu.eip + 1) -1;
			/*cpu.eip = (op_src->val & 0xFFFF) - 2;*/
		}
		else {
			cpu.eip = op_src->val - concat(decode_rm_,SUFFIX)(cpu.eip + 1) -1;
			//cpu.eip = op_src->val - 2;
		}
		//printf("%x\n", cpu.eip);
	}
	else {
		DATA_TYPE_S displacement = op_src->val;
		cpu.eip = cpu.eip + displacement;
		if (DATA_BYTE == 2)
			cpu.eip &= 0xffff;
		//printf("%x\n", cpu.eip);
	}
	print_asm_template1call();
}

make_instr_helper(i2r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
