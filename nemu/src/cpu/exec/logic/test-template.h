#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	DATA_TYPE result = op_dest->val & op_src->val;

	uint8_t b = result & 0x000000ff;
	cpu.SF = MSB(result);
	cpu.ZF = result ? 0 : 1;
	cpu.OF = 0;
	cpu.PF = ((b&1)^(b>>1&1)^(b>>2&1)^(b>>3&1)^(b>>4&1)^(b>>5&1)^(b>>6&1)^(b>>7&1)) ? 1 : 0;
	cpu.CF = 0;


	print_asm_template2();

	//DATA_TYPE result = (op_dest->val & op_src->val);
	/*DATA_TYPE res = op_dest->val & op_src->val;
	  DATA_TYPE sf = 1 << (8 * DATA_BYTE - 1);
	  cpu.SF = MSB(res);
	  if (res == 0)
	  cpu.ZF = 1;
	  else
	  cpu.ZF = 0;
	  int i,num=0;
	  for (i = 0,sf = 0x1,res = res & 0xff;i < 8; i++)
	  { 
	  if ((sf & res) == sf)num++;
	  sf <<= 1;
	  }
	  if (num % 2 == 1)
	  cpu.PF = 1;
	  else cpu.PF = 0;
	  cpu.CF = cpu.OF = 0;*/

	/*uint32_t len=(DATA_BYTE<<3)-1;
	  cpu.OF = 0;
	  cpu.CF = 0;
	  cpu.SF = (result >> len) & 1;
	  cpu.ZF = !result;
	  result &= 0xff;
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);

*/
	/*uint8_t b = result & 0x000000ff;
	  cpu.SF = MSB(result);
	  cpu.ZF = result ? 0 : 1;
	  cpu.OF = 0;
	  cpu.PF = ((b&1)^(b>>1&1)^(b>>2&1)^(b>>3&1)^(b>>4&1)^(b>>5&1)^(b>>6&1)^(b>>7&1)) ? 1 : 0;
	  cpu.CF = 0;*/
	/*uint32_t len = (DATA_BYTE << 3) - 1;
	  cpu.SF = result >> len;
	  cpu.ZF = !(result);
	  cpu.CF = 0; 
	  cpu.OF = 0;
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);*/
	//print_asm_template2();
}

	make_instr_helper(i2a)
	make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
