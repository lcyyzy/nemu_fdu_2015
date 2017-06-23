#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr lgdt

make_helper(concat(lgdt_rm_, SUFFIX)) {
	/*swaddr_t addr = instr_fetch(eip + 2, 4);
	if (DATA_BYTE == 2) {
		cpu.gdtr.limit = instr_fetch(addr, 2);
		cpu.gdtr.base = instr_fetch(addr + 2, 3);
	}
	else {
		cpu.gdtr.limit = instr_fetch(addr, 2);
		cpu.gdtr.base = instr_fetch(addr + 2, 4);
	}
	print_asm("lgdt" " 0x%x", addr);
	return 6;*/

	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	uint16_t limit;
	uint32_t base;
	limit = swaddr_read(op_src->addr, 2);
	base = swaddr_read(op_src->addr + 2, 4);

	if (DATA_BYTE == 2) {
		cpu.gdtr.limit = limit;
		cpu.gdtr.base = base & 0xffffff;
	}
	else {
		cpu.gdtr.limit = limit;
		cpu.gdtr.base = base;
	}
	//print_asm("lgdt")
	print_asm("lgdt seg_limit:%2x, base_addr:%x", cpu.gdtr.limit, cpu.gdtr.base);
	return len + 1;
}

#include "cpu/exec/template-end.h"
