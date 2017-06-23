#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define DATA_BYTE 2
#include "lgdt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "lgdt-template.h"
#undef DATA_BYTE

/*make_helper(lgdt_m) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	cpu.gdtr.limit = swaddr_read(eip + 4, 2);
	cpu.gdtr.base = swaddr_read(eip, 4);
	print_asm("lgdt seg_limit:%2x, base_addr:%x", cpu.gdtr.limit, cpu.gdtr.base);
	return 1 + len;
}*/

make_helper_v(lgdt_rm)
