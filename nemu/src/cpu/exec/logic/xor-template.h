#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE result = op_dest->val ^ op_src->val;

	uint8_t b = result & 0x000000ff;
	cpu.SF = MSB(result);
	cpu.ZF = result ? 0 : 1;
	cpu.OF = 0;
	cpu.PF = ((b&1)^(b>>1&1)^(b>>2&1)^(b>>3&1)^(b>>4&1)^(b>>5&1)^(b>>6&1)^(b>>7&1)) ? 1 : 0;
	cpu.CF = 0;

	OPERAND_W(op_dest, result);

	print_asm_template2();

	/*DATA_TYPE result = op_dest->val ^ op_src->val;
	  OPERAND_W(op_dest, result);

	  int len = (DATA_BYTE << 3) - 1;
	//int s1, s2;
	cpu.CF = 0;
	cpu.SF = (result >> len) & 1;
	//s1 = op_dest->val >> len;
	//s2 = op_src->val >> len;
	cpu.OF = 0;
	cpu.ZF = !(result);
	result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);*/

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	/*cpu.OF = 0;
	  cpu.CF = 0;
	  uint32_t len = (DATA_BYTE << 3) - 1;
	  cpu.SF = result >> len;
	  cpu.ZF = !(result);
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1); */
	//print_asm_template2();
}

	make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
	make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
