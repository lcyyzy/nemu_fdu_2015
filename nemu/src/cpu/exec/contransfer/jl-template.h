#include "cpu/exec/template-start.h"

#define instr jl

static void do_execute() {
	DATA_TYPE_S displacement = op_src->val;
	if (cpu.SF != cpu.OF) {
		cpu.eip += displacement;
		//printf("%x\n", cpu.eip);
		if (DATA_BYTE == 2) {
			//printf("fuck\n");
			cpu.eip &= 0xffff;
		}
	}
	print_asm_template1call();
}

make_instr_helper(i2r)

#include "cpu/exec/template-end.h"
