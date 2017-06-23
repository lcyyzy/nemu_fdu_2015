#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	print_asm_template1wo();
	DATA_TYPE_S displacement = op_src->val;
	if (cpu.ZF == 1) {
		//printf("fuck\n");
		//printf("%x\n", cpu.eip);
		//printf("%x\n", displacement);
		cpu.eip += displacement;
		if (DATA_BYTE == 2)
			cpu.eip &= 0xffff;
	}
	//printf("%x\n", cpu.eip);
}

make_instr_helper(i2r)

#include "cpu/exec/template-end.h"
