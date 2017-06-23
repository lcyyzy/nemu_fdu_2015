#include "cpu/exec/template-start.h"

#define instr clc

static void do_execute() {
	cpu.CF = 0;
	print_asm("clc");
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
