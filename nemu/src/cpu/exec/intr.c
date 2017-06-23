#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"
#include <setjmp.h>
extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t, size_t);
void swaddr_write(swaddr_t, size_t, uint32_t);

//void load_sreg(uint32_t);

void raise_intr(uint8_t NO/*, uint32_t len*/) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *	 *	 * That is, use ``NO'' to index the IDT.
	 *		 *		 */
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.EFLAGS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.cs.val);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip);

	uint8_t idt[8];
	int i;
	for (i = 0; i < 8; i++)
		idt[i] = lnaddr_read(cpu.idtr.base + 8 * NO + i, 1);
	GateDesc *gde = (GateDesc*) idt;
	cpu.cs.val = gde->segment;
	//erode_sreg(R_CS);

	uint8_t tmp[8]; 
	for(i = 0; i < 8; ++ i) 
		tmp[i] = lnaddr_read(cpu.gdtr.base + sreg(R_CS).INDEX * 8 + i, 1);
	SegDesc *segdesc = (SegDesc*)tmp;
	cpu.sreg[R_CS].invi = *segdesc;

	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);

}

