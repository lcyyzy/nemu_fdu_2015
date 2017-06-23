#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute() {
	/*cpu.esp = REG(R_EBP);
	  REG(R_EBP) = MEM_R(cpu.esp);
	  cpu.esp += DATA_BYTE;*/
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	print_asm_templateno();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
