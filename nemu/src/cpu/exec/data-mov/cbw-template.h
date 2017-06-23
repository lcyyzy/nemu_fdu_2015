#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr cbw

make_helper(concat(cbw_, SUFFIX)) {
	if (DATA_BYTE == 2) {
		reg_w(R_AX) = (int8_t)reg_b(R_AL);
		print_asm("cbw");
	}
	else {
		cpu.eax = (int16_t)reg_w(R_AX);
		print_asm("cwde");
	}
	return 1;
}

#include "cpu/exec/template-end.h"
