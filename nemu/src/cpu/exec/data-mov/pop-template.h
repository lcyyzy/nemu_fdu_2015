/*#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {*/
	/*if (DATA_BYTE == 2 || DATA_BYTE == 4) {
		OPERAND_W(op_src, MEM_R(cpu.esp));
		MEM_W(cpu.esp, 0);
		cpu.esp += DATA_BYTE;
	}
	else {
		swaddr_write(op_src->addr, 4, MEM_R(reg_l(R_ESP)));
		MEM_W(reg_l(R_ESP), 0);
		reg_l(R_ESP) += 4;
		}*/
/*	if (DATA_BYTE == 2){
		REG(op_src->reg) = swaddr_read(cpu.esp, 2);
		cpu.esp += 2;
	} 
	else {
		REG(op_src->reg) = swaddr_read(cpu.esp, 4);
		cpu.esp += 4;
	}
	print_asm_template1();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"*/

#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	int val = MEM_R(reg_l(R_ESP));
	OPERAND_W(op_src, val);
	reg_l(R_ESP) += DATA_BYTE;

	print_asm_template1();

}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

