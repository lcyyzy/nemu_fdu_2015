#include "nemu.h"

typedef union {
	struct {
		uint32_t limit_15_0 : 16;
		uint32_t base_15_0 : 16;
		uint32_t base_23_16 : 8;
		uint32_t type : 4;
		uint32_t seg_type : 1;
		uint32_t DPL : 2;
		uint32_t P : 1;
		uint32_t limit_19_16 : 4;
		uint32_t AVL : 1;
		uint32_t op_type : 1;
		uint32_t B : 1;
		uint32_t G : 1;
		uint32_t base_31_24	: 8;
	};
	uint8_t val[8];
} SegDescriptor;

uint32_t lnaddr_read(lnaddr_t, size_t);
uint8_t current_sreg;

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t current_sreg) {
	//printf("seg\n");
	lnaddr_t gdt;
	lnaddr_t table_addr;
	SegDescriptor SD;
	if (cpu.cr0.PE == 0)
		return addr;
	gdt = cpu.gdtr.base;
	table_addr = gdt + sreg(current_sreg).INDEX * 8;
	int i;
 	for (i = 0; i < 8; ++i) {
		SD.val[i] = lnaddr_read(table_addr + i, 1);
	}
	uint32_t base = (SD.base_31_24 << 24) | (SD.base_23_16 << 16) | SD.base_15_0;
	return base + addr;
}


/*#include "common.h"
#include "memory.h"
#include "cpu/reg.h"
#include "debug.h"

extern uint32_t hwaddr_read(hwaddr_t addr, size_t len);
int sreg_dirty[4] = {1, 1, 1, 1};
lnaddr_t segment_translate(swaddr_t addr, size_t len, int32_t current_sreg) {
	if(cpu.cr0.PE) {
		if(sreg_dirty[current_sreg] == 0) {
			Assert(cpu.sreg[current_sreg].limit >= addr + len, "out of limit: \naddr:0x%x\nlen: %x\nlimit:0x%x\ncurrent_sreg:%s", addr, len, cpu.sreg[current_sreg].limit, sreg_str[current_sreg]);
		}
		else {
			uint32_t mask_BASE0 = 0x0000ffff;
			uint32_t mask_BASE1 = 0x00ff0000;
			uint32_t mask_BASE2 = 0xff000000;
			uint16_t selector_tmp = cpu.sreg[current_sreg].selector;
			assert((selector_tmp >> 3) <= cpu.GDTR.limit);
			uint32_t gdt_tmp[4];
			int i;
			for (i = 0; i < 4; ++i)
				gdt_tmp[i] = hwaddr_read(cpu.GDTR.base + ((selector_tmp >> 3) << 3) + 2 * i, 2);
			uint32_t base_tmp = (gdt_tmp[1] & mask_BASE0) | ((gdt_tmp[2] << 16) & mask_BASE1) | ((gdt_tmp[3] << 16) & mask_BASE2);
			cpu.sreg[current_sreg].base = base_tmp;
			Assert(cpu.sreg[current_sreg].limit >= addr + len,"out of limit: \naddr:0x%x\nlen: %x\nlimit:0x%x\ncurrent_sreg:%s", addr, len, cpu.sreg[current_sreg].limit, sreg_str[current_sreg]);
			sreg_dirty[current_sreg] = 0;
		}
		return addr + cpu.sreg[current_sreg].base;
	}
	else {
		return addr;
	}
}*/
