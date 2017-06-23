#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr ret

/*static void do_execute() {
	//printf("%x\n", DATA_BYTE);
	if (ops_decoded.opcode == 0xc2) {
		cpu.eip = swaddr_read(cpu.esp, 2) & 0xffff;
		cpu.esp += 2;
		cpu.eip -= 3;
		cpu.esp += (DATA_TYPE_S)op_src->val;
		//printf("%x\n", op_src->val);
		print_asm_template1();
	} 
	else {
		if (DATA_BYTE == 2) {
			current_sreg = R_SS;
			cpu.eip = (MEM_R(REG(R_ESP)) & 0xffff) - 1;
			REG(R_ESP) += DATA_BYTE;
		}
		else {
			current_sreg = R_SS;
			cpu.eip = MEM_R(REG(R_ESP)) - 1;
			REG(R_ESP) += DATA_BYTE;
		}
		print_asm("ret");
		//return 2;
	}
}

make_instr_helper(r)
make_instr_helper(i)
*/

make_helper(concat(ret_, SUFFIX)) {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp = cpu.esp + DATA_BYTE;
	if (DATA_BYTE == 2) 
		cpu.eip &= 0x0000ffff;
	print_asm("ret");
	cpu.eip -= 1;
	//printf("%x\n", cpu.eip);
	return 1;
}

make_helper(concat(ret_i_, SUFFIX)) {
	uint16_t imm = instr_fetch(eip + 1, 1 + 1);
	if (DATA_BYTE == 4) {
		cpu.eip = swaddr_read(reg_l(R_ESP), 4) - 1;
		reg_l(R_ESP) += 4;
	}
	else { 
		cpu.eip = swaddr_read(reg_l(R_ESP), 2) & 0xffff;
		cpu.eip -= 1;
		reg_l(R_ESP) += 2;
	}
	reg_l(R_ESP) += imm;

	print_asm("ret" str(SUFFIX) " $0x%x", imm);
	return 1;
}

#include "cpu/exec/template-end.h"
