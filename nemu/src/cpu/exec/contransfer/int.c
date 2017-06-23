#include "cpu/exec/helper.h"
#include "cpu/reg.h"
#include "setjmp.h"

void raise_intr(uint8_t/*, uint32_t*/);

make_helper(int_r) {
	//printf("int\n");	
	uint16_t imm = instr_fetch (eip + 1, 1);
	cpu.eip += 2;
	
	//print_asm("int" " $0x%x", imm);
	raise_intr(imm);
	return 0;
}


