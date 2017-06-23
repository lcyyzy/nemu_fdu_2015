#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr cmps

static void do_execute() {
	//printf("fuck cmps\n");
	int incDec = 0;
	uint32_t src = 0, dest = 0, result = 0;

	src = swaddr_read(cpu.esi, DATA_BYTE);
	dest = swaddr_read(cpu.edi, DATA_BYTE);
	result = src - dest;
	cpu.SF = result >>  ((DATA_BYTE << 3) - 1);
	//printf("src%x dest%x SF%d\n", src, dest, cpu.SF);
	if (((src >> ((DATA_BYTE << 3) - 1)) != (dest >> ((DATA_BYTE << 3) - 1))) && ((result >> ((DATA_BYTE << 3) - 1)) == (dest >> ((DATA_BYTE << 3) - 1))))
		cpu.OF = 1;
	else
		cpu.OF = 0;

	if (cpu.DF == 0)
		incDec = DATA_BYTE;
	else
		incDec = -DATA_BYTE;

	cpu.esi += incDec;
	cpu.edi += incDec;

	cpu.ZF = !result;
	//printf("%d\n", cpu.ZF);
	cpu.CF = (src >= dest) ? 0 : 1;

	result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	if (DATA_BYTE == 1)
		print_asm("cmps" str(SUFFIX) "$di, $es:($si)");
		else
		print_asm("cmps" str(SUFFIX) "$edi, $es:($esi)");

	/*uint32_t ans=cpu.esi-(sreg(R_ES).base+cpu.edi);
	uint32_t len=(DATA_BYTE<<3)-1;
	if (op_dest->val<op_src->val)
		cpu.CF=1; else
			cpu.CF=0;
	cpu.ZF=!ans;
	cpu.SF=(ans>>len)&1;
	cpu.OF=(((op_dest->val>>len)&1)!=((op_src->val>>len)&1))&&(((op_dest->val>>len)&1)!=cpu.SF);
	ans&=0xff;
	ans^=ans>>4;
	ans^=ans>>2;
	ans^=ans>>1;
	cpu.PF=!(ans&1);
	print_asm(str(instr));
	if (!cpu.DF)
	{
		cpu.esi+=DATA_BYTE;
		cpu.edi+=DATA_BYTE;
	} else
	{
		cpu.esi-=DATA_BYTE;
		cpu.edi-=DATA_BYTE;
 	}*/

}


make_helper(concat(cmps_, SUFFIX)) {
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"

