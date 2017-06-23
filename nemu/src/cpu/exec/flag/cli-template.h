#include "cpu/exec/template-start.h"

#define instr cli

static void do_execute() {
		cpu.IF = 0;
		print_asm("cli");
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
