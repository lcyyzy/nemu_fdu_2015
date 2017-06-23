#include "cpu/exec/template-start.h"

#define instr stc

static void do_execute() {
	cpu.CF = 1;
	print_asm("stc");
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
