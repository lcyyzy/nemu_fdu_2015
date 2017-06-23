/*#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "iret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "iret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "iret-template.h"
#undef DATA_BYTE*/

/* for instruction encoding overloading */
#include "cpu/exec/helper.h"

int iret(swaddr_t eip){

	cpu.eip = swaddr_read(cpu.esp, 4);
	//printf("%x", cpu.eip);

	cpu.esp += 4;

	cpu.cs.val = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	//load_sreg(R_CS);

	/*uint8_t tmp[8];  
	  int i = 0;
	  for(i = 0; i < 8; ++ i) 
	  tmp[i] = lnaddr_read(cpu.gdtr.base + sreg(R_CS).INDEX * 8 + i, 1);
	  SegDesc *segdesc = (SegDesc*)tmp;
	  cpu.sreg[R_CS].invi = *segdesc;*/


	//uint64_t base_limit = decode_gdt(cpu.Sreg[SR_CS].index);
	uint32_t base = cpu.gdtr.base;
	uint32_t addr = base + cpu.cs.INDEX * 8;

	uint32_t val1 = lnaddr_read(addr, 4);
	uint32_t base1 = val1 >> 16;
	uint32_t limit1 = val1 & 0xffff;

	uint32_t val2 = lnaddr_read(addr + 4, 4);
	uint32_t base2 = val2 & 0xff;
	base2 = base2 << 16;
	uint32_t base3 = val2 & 0xff000000;
	uint32_t limit2 = val2 & 0xf0000;

	uint32_t seg_base = (base1 | base2 | base3);
	uint32_t seg_limit = limit1 | limit2;

	uint64_t base_limit = seg_base;
	base_limit = (base_limit << 32) | seg_limit;
	cpu.cs.base = base_limit >> 32;
	cpu.cs.limit = base_limit & 0xffffffff;


	cpu.EFLAGS = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	//printf("%x", cpu.eip);
	print_asm("iret");

	return 0;
}

