#include "cpu/exec/template-start.h"

#define instr jnle

static void do_execute() {
	DATA_TYPE_S displacement = op_src->val;
	if (cpu.SF == cpu.OF && cpu.ZF == 0) {
		cpu.eip += displacement;
		if (DATA_BYTE == 2)
			cpu.eip &= 0xffff;
	}
	print_asm_template1call();
}

make_instr_helper(i2r)

#include "cpu/exec/template-end.h"
