#include "cpu/exec/template-start.h"

#define instr cltd

static void do_execute() {
	DATA_TYPE len = DATA_BYTE << 3;
	if (REG(R_EAX) >> (len - 1) == 1) {
		REG(R_EDX) = (1 << len) - 1;
	}
	else
		REG(R_EDX) = 0;
	print_asm("cltd");
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"

