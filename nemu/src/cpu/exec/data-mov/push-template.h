#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	if (DATA_BYTE == 2) {
		cpu.esp -= DATA_BYTE;
		MEM_W(cpu.esp, op_src->val);
	}
	else {
		cpu.esp -= 4;
		swaddr_write(cpu.esp, 4, op_src->val);
	}
	//printf("cpu.esp %x\n", cpu.esp);
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"


/*#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	DATA_TYPE val0 = op_src -> val;
	unsigned val = val0;
#if DATA_BYTE == 2 || DATA_BYTE == 4
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W( reg_l(R_ESP), val);
#else
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP), 4, SR_SS, val);
#endif

	print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
	make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"*/

