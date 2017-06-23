#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;

	cpu.SF = MSB(result);
	cpu.ZF = result ? 0 : 1;
	if ((MSB(op_src->val) == 1) && (MSB(result) == 0))
		cpu.OF = 1;
	else
		cpu.OF = 0;

	OPERAND_W(op_src, result);

	result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template1();



	/* TODO: Update EFLAGS. */
	//panic("please implement me");

	//print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
