/*#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;

	cpu.SF = MSB(result);
	cpu.ZF = result ? 0 : 1;
	if ((MSB(op_dest->val) != MSB(op_src->val)) && (MSB(op_src->val) == cpu.SF))
		cpu.OF = 1;
	else
		cpu.OF = 0;
	cpu.CF = (op_dest->val >= op_src->val) ? 0 : 1;*/


	/*result &= 0xff;
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);*/
	/*cpu.PF = 1;
	int i;
	for (i = 0; i < 8; ++ i) 
		cpu.PF ^= (result & (1 << i)) >> i;
	print_asm_template2();*/





	/*DATA_TYPE src = 0;
	  if (ops_decoded.opcode == 0x83) {
	  src = (int8_t)op_src->val;
	  }
	  else
	  src = op_src->val;
	//printf("%x\n", src);
	//CF:
	DATA_TYPE nsrc = ~src + 1;
	DATA_TYPE result = op_dest->val + nsrc;
	//printf("%x  %x\n", op_src->val, op_src->val >> 7);
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
	//cpu.CF = op_dest->val < op_src->val ? 1 : 0;
	//int k = (result >> 31) & 1;
	//int j = len + ~1 + 1;
	//cpu.OF = (!(k ^ (result >> j))) | (k & !(~result >> j));
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.PF = !(result & 1);*/

	/*DATA_TYPE result = op_dest->val - op_src->val;

	  int len = (DATA_BYTE << 3) - 1;
	  int s1, s2;
	  cpu.CF = (op_src->val > op_dest->val);
	  cpu.SF = (result >> len) & 1;
	  s1 = op_dest->val >> len;
	  s2 = op_src->val >> len;
	  cpu.OF = ((s1 & 1) != (s2 & 1) && ((s1 & 1) != cpu.SF));
	  cpu.ZF = !(result);
	  result &= 0xff;
	  result ^= result >> 4;
	  result ^= result >> 2;
	  result ^= result >> 1;
	  cpu.PF = !(result & 1);

	  print_asm_template2();*/
/*}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"*/

/*#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	if(op_dest->val < op_src->val) cpu.CF = 1; else cpu.CF = 0;
	if(MSB(op_dest->val) != MSB(op_src->val) && MSB(result) != MSB(op_dest->val))
		cpu.OF = 1; else cpu.OF = 0;
	//Updata_EFLAGS(result);
	int i;
	cpu.PF = 1;
	for(i = 0; i < 8; ++ i) cpu.PF ^= (result & (1 << i)) >> i;
	cpu.ZF = (result == 0);
	cpu.SF = MSB(result);
	print_asm_template2();
}


#if DATA_BYTE == 2 || DATA_BYTE == 4

make_instr_helper(si2rm)

#endif

	make_instr_helper(i2rm)
	make_instr_helper(r2rm)
	make_instr_helper(rm2r)
make_instr_helper(i2a)
#include "cpu/exec/template-end.h"
*/
#include "cpu/exec/template-start.h"
//#include "cpu/exec/eflags.h"

#define instr cmp

static void do_execute() {
	//setEFLAGS_ALU((uint32_t)op_dest->val,(uint32_t)op_src->val,1,DATA_BYTE);
	uint32_t x = (uint32_t)op_dest->val;
	uint32_t y = (uint32_t)op_src->val;
	bool cin = 1;
	uint32_t data_len = DATA_BYTE;
	if(cin) y=~y;

	uint32_t bit_len=((data_len)<<3);
	uint32_t mask=((uint32_t)0xffffffff)>>(32-bit_len);
	uint32_t result=(x+y+cin)&mask;
	bool cMSB = ( (x&(mask>>1)) + (y&(mask>>1)) + cin ) >> (bit_len-1);
	bool cout = ( ((x>>(bit_len-1))&1) + ((y>>(bit_len-1))&1) + cMSB ) >> 1;

	cpu.CF= cin^cout;
	//setPF(result);
	uint32_t temp = result;
	temp=x&0xff;
	temp=temp^(temp>>1);
	temp=temp^(temp>>2);
	temp=temp^(temp>>4);
	cpu.PF= ~temp&1;
	cpu.AF= cin^(( (x&0xf)+(y&0xf)+cin )>>4);
	cpu.ZF= result==0;
	cpu.SF= result>>(bit_len-1);
	cpu.OF= cout^cMSB;
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"

