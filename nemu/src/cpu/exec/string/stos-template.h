#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr stos

static void do_execute() {
	MEM_W(cpu.edi, cpu.eax);
	if (cpu.DF == 0) {
		cpu.edi += DATA_BYTE;
	}
	else {
		cpu.edi -= DATA_BYTE;
	}
	if (DATA_BYTE == 1)
		print_asm("stos $al, $es:($edi)");
	else
		print_asm("stos $eax, $es:($edi)");
}	

make_helper(concat(stos_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"
