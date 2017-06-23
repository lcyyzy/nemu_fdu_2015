#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {

	DATA_TYPE result = op_dest->val - op_src->val;

	cpu.SF = MSB(result);
	cpu.ZF = result ? 0 : 1;
	if ((MSB(op_dest->val) != MSB(op_src->val)) && (MSB(op_src->val) == cpu.SF))
		cpu.OF = 1;
	else
		cpu.OF = 0;
	cpu.CF = (op_dest->val >= op_src->val) ? 0 : 1;

	OPERAND_W(op_dest, result);

	result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);
	print_asm_template2();

	//DATA_TYPE result = op_dest->val - op_src->val;
	//uint32_t len = (DATA_BYTE << 3) - 1;
	/*cpu.SF = ((DATA_TYPE_S)(result) < 0);
	  cpu.ZF = !(result);
	  cpu.CF = op_dest->val < op_src->val;
	  cpu.OF = (MSB(op_dest->val) != MSB(op_src->val)) && (MSB(result) == MSB(op_src->val));
	  cpu.PF = 0;
	  while (result) {
	  cpu.PF = !cpu.PF;
	  result = result & (result - 1);
	  }*/

	/*OPERAND_W(op_dest, op_dest->val - op_src->val);

	  int len = (DATA_BYTE << 3) - 1;
	  int s1, s2;
	  cpu.CF = (result < op_dest->val);
	  cpu.SF = (result >> len) & 1;
	  s1 = op_dest->val >> len;
	  s2 = op_src->val >> len;
	  cpu.OF = ((s1 & 1) != (s2 & 1) && ((s1 & 1) != cpu.SF));
	  cpu.ZF = !(result);
	  result &= 0xff;
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);*/

	/*int s1, s2;
	  s1 = op_dest->val >> len;
	  s2 = op_src->val >> len;
	  cpu.CF = (result > op_dest->val);
	  cpu.OF = (s1 == s2 && s1 != cpu.SF);
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);*/
	//print_asm_template2();

	/*DATA_TYPE src = 0;
	  if (ops_decoded.opcode == 0x83) {
	  src = (int8_t)op_src->val;
	  }
	  else
	  src = op_src->val;
	//CF:
	DATA_TYPE nsrc = ~src + 1;
	DATA_TYPE result = op_dest->val + nsrc;
	uint32_t cout = 0;
	if ((result < op_dest->val) && (result < nsrc)) {
	cout = 1;
	}
	else 
	cout = 0;
	cpu.CF = cout ^ 0x1;
	if (src == nsrc) {
	cpu.CF = 0;
	}
	//OF:
	DATA_TYPE rdest = op_dest->val & (~(0x1 << ((DATA_BYTE << 3) - 1)));
	DATA_TYPE rsrc = nsrc & (~(0x1 << ((DATA_BYTE << 3) - 1)));
	uint32_t cout2 = (rdest + rsrc) >> ((DATA_BYTE << 3) - 1);
	cpu.OF = cout ^ cout2;
	if (src == nsrc && (src != 0x0)) {
	if (op_dest->val >> ((DATA_BYTE << 3) - 1) == 1)
	cpu.OF = 0;
	else
	cpu.OF = 1;
	}
	uint32_t len = (DATA_BYTE << 3) - 1;
	cpu.SF = result >> len;
	cpu.ZF = !(result);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);	
	//printf("%x\n", op_dest->val - op_src->val);
	print_asm_template 2();*/

	/*DATA_TYPE result = op_dest->val - op_src->val;

	  uint8_t b = result & 0x000000ff;
	  cpu.SF = MSB(result);
	  cpu.ZF = result ? 0 : 1;
	  cpu.OF = MSB(op_dest->val) ? (MSB(op_src->val) ? 0 : !cpu.SF) : (MSB(op_src->val) ? cpu.SF : 0);
	  cpu.PF = ((b & 1) ^ (b >> 1 & 1) ^ (b >> 2 & 1) ^ (b >> 3 & 1) ^ (b >> 4 & 1) ^ (b >> 5 & 1) ^ (b >> 6 & 1) ^ (b >> 7 & 1)) ? 1 : 0;
	cpu.CF = (op_dest->val >= op_src->val) ? 0 : 1;

	OPERAND_W(op_dest, result);

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

