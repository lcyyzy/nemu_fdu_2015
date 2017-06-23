#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmps-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmps-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmps-template.h"
#undef DATA_BYTE

make_helper_v(cmps)

/*#include "cpu/exec/helper.h"

make_helper(cmps) {

	int incDec = 0;
	uint32_t src = 0, dest = 0, result = 0;

	switch (ops_decoded.opcode & 0xff){
		case 0xa6:
			src = swaddr_read(cpu.esi, 1);
			dest = swaddr_read(cpu.edi, 1);
			result = src - dest;
			cpu.SF = result >> 7;
			if (((src >> 7) != (dest >> 7)) && ((result >> 7) == (dest >> 7)))
				cpu.OF = 1;
			else
				cpu.OF = 0;
			if (cpu.DF == 0)
				incDec = 1;
			else
				incDec = -1;
			break;
		case 0xa7:
			src = swaddr_read(cpu.esi, DATA_BYTE);
			dest = swaddr_read(cpu.edi, DATA_BYTE);
			result = src - dest;
			cpu.SF = result >>  (DATA_BYTE << 3 - 1);
			if (((src >> (DATA_BYTE << 3 - 1)) != (dest >> (DATA_BYTE << 3 - 1))) && ((result >> (DATA_BYTE << 3 - 1)) == (dest >> (DATA_BYTE << 3 - 1))))
				cpu.OF = 1;
			else
				cpu.OF = 0;

			if (cpu.DF == 0)
				incDec = DATA_BYTE;
			else
				incDec = -DATA_BYTE;
			break;
		default:
			printf("Invoked cmps default!\n");
			break;
	}

	cpu.esi += incDec;
	cpu.edi += incDec;

	cpu.ZF = result ? 0 : 1;
	cpu.CF = (src >= dest) ? 0 : 1;

	result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	return 1;
}

*/
/*#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmps-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmps-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmps-template.h"
#undef DATA_BYTE

 for instruction encoding overloading 

make_helper_v(cmps)*/

/*#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr cmps

static void do_execute() {
*/
