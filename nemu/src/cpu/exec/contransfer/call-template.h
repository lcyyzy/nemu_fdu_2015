/*#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	//printf("%x\n", ops_decoded.opcode);
	//printf("%x\n", op_src->val);
	if (ops_decoded.opcode == 0xe8) {
		cpu.esp -= DATA_BYTE;
		MEM_W(cpu.esp, cpu.eip + DATA_BYTE + 1);
		cpu.eip += op_src->val;
		//printf("%x\n", cpu.eip);
	}
	else if (ops_decoded.opcode == 0xff) {
	 	if (DATA_BYTE == 2) {
			cpu.esp -= DATA_BYTE;
			MEM_W(cpu.esp, cpu.eip + DATA_BYTE + 1);
			printf("val%x\n", op_src->val);
			cpu.eip = (op_src->val & 0x0000ffff) - 2;
		}
	 	else {
			cpu.esp -= DATA_BYTE;
			MEM_W(cpu.esp, cpu.eip + DATA_BYTE + 1);
			printf("val%x\n", op_src->val);
			cpu.eip = op_src->val - 2;
		}
	}
	print_asm_template1call();
}

make_instr_helper(i2r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"*/

/*#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {

	if (DATA_BYTE == 2) {
		cpu.esp -= 2;
		MEM_W(cpu.esp, (uint16_t)(cpu.eip & 0x0000ffff) + 4);
		cpu.eip = (cpu.eip + op_src->val) & 0x0000ffff;
	} 
	else if (DATA_BYTE == 4) {
		cpu.esp -= 4;
		MEM_W(cpu.esp, cpu.eip + 4);
		cpu.eip = cpu.eip + op_src->val;
	}

	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(i)
#endif

	make_helper(concat(call_rm_, SUFFIX)) {
		int len = concat(decode_rm_, SUFFIX)(cpu.eip + 1);

		if (DATA_BYTE == 2) {
			cpu.esp -= 2;
			MEM_W(cpu.esp, (uint16_t)((cpu.eip + len) & 0x0000ffff));
			cpu.eip = op_src->val & 0x0000ffff;
		} 
		else if (DATA_BYTE == 4) {
			cpu.esp -= 4;
			MEM_W(cpu.esp, cpu.eip + len);
			cpu.eip = op_src->val;
		}

		print_asm_template2();
		return 0;
	}

#include "cpu/exec/template-end.h"*/


#include "cpu/exec/template-start.h"

make_helper(concat(call_i_, SUFFIX)){
	reg_l(R_ESP) -= DATA_BYTE;
	int len = concat(decode_i_, SUFFIX)(eip + 1);
	MEM_W(reg_l(R_ESP), eip + len + 1);
	int val = op_src -> val;
	cpu.eip += val;

#define instr call
	print_asm_template1();
#undef instr

	return len + 1;
}

make_helper(concat(call_rm_, SUFFIX)){
	reg_l(R_ESP) -= DATA_BYTE;
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
	MEM_W(reg_l(R_ESP), eip + len + 1);
	DATA_TYPE val = (DATA_TYPE)op_src -> val;
	cpu.eip = val;

#define instr call
	print_asm_template1();
#undef instr

	return 0;
}


#include "cpu/exec/template-end.h"

