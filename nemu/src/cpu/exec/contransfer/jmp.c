#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

make_helper_v(jmp_i2r)
make_helper_v(jmp_rm)

make_helper(jmp_ptr) {
	/*uint16_t cs = instr_fetch(eip + 5, 2);
	uint32_t imm = instr_fetch(eip + 1, 4);
	cpu.cs.val = cs;
	cpu.eip = imm - (1 + 6);
	print_asm("ljmp  $%x,$%x", cs, imm);
	return 7;*/

	uint32_t op1 = instr_fetch(eip + 1, 4) - 7;
	uint16_t op2 = instr_fetch(eip + 5, 2);
	cpu.eip = op1;
	cpu.cs.val = op2;
	Assert(cpu.cr0.PE, "Not in protection mode");
	uint16_t segsel = cpu.sreg[R_CS].val;
	//printf("%x\n", segsel);
	uint16_t index = segsel >> 3;
	//printf("%x", index);
	uint8_t ti = (segsel >> 2) & 1;

	Assert(ti == 0, "LDT is not supported");
	Assert(index * 8 < cpu.gdtr.limit, "segment selector %x out of limit %x", index, cpu.gdtr.limit);

	uint64_t descriptor = ((uint64_t) lnaddr_read(cpu.gdtr.base + index * 8 + 4, 4) << 32) | lnaddr_read(cpu.gdtr.base + index * 8, 4);

	uint32_t limit = (descriptor & 0xFFFF) | ((descriptor >> 32) & 0xF0000);
	uint32_t base = ((descriptor >> 16) & 0xFFFFFF) | ((descriptor >> 32) & 0xFF000000);
	uint8_t granularity = (descriptor >> 55);
	if (granularity) {
		limit <<= 12;
	}

	cpu.sreg[R_CS].base = base;
	cpu.sreg[R_CS].limit = limit;
	print_asm("ljmp %x, %x\n", op2, op1 + 7);
	return 7;
}

