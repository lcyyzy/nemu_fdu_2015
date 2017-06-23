#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
	DATA_TYPE result = op_dest->val + op_src->val + cpu.CF;

	cpu.SF = MSB(result);
	cpu.ZF = result ? 0 : 1;
	if ((MSB(op_dest->val) == MSB(op_src->val)) && (MSB(op_src->val) != cpu.SF))
		cpu.OF = 1;
	else
		cpu.OF = 0;
	cpu.CF = (op_dest->val > result) ? 1 : 0;

	OPERAND_W(op_dest, result); 

	result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template2();
	/*DATA_TYPE result = op_dest->val + op_src->val + cpu.CF;	
	  OPERAND_W(op_dest, op_src->val + op_dest->val + cpu. CF);
	  int len = (DATA_BYTE << 3) - 1;
	  int s1, s2;
	  cpu.CF = (result < op_dest->val);
	  cpu.SF = (result >> len) & 1;
	  s1 = op_dest->val >> len;
	  s2 = op_src->val >> len;
	  cpu.OF = ((s1 & 1) == (s2 & 1) && ((s1 & 1) != cpu.SF));
	  cpu.ZF = !(result);
	  result &= 0xff;
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);	
	  print_asm_template2();*/
	/*DATA_TYPE result = op_dest->val + op_src->val + cpu.CF;
	  uint32_t len = (DATA_BYTE << 3) - 1;
	  cpu.SF = result >> len;
	  cpu.ZF = !(result);
	  cpu.CF = op_dest->val < op_src->val ? 1 : 0;
	  int k = (result >> 31) & 1;
	  int j = len + ~1 + 1;
	  cpu.OF = (!(k ^ (result >> j))) | (k & !(~result >> j));
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);
	  OPERAND_W(op_dest, op_dest->val + op_src->val + cpu.CF);
	  print_asm_template2();*/
}

	make_instr_helper(i2rm)
make_instr_helper(i2a)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
	make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
