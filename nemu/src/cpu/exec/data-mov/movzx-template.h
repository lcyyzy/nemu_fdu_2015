/*#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr movzb

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movzx_rm2r_, SUFFIX)) {
	uint32_t decode_eip = cpu.eip + 2;
	op_src->size = 1;
	int len = read_ModR_M(decode_eip, op_src, op_dest);
	op_dest->val = REG(op_dest->reg);

	if (DATA_BYTE == 2) {
		uint16_t result = op_src->val;
		OPERAND_W(op_dest, result);
		print_asm("movzx" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
		return len + 1;
	}
	else {
		uint32_t result = op_src->val;
		OPERAND_W(op_dest, result);
		print_asm("movzx" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
		return len + 1;
		}	
		}
#endif

//#if DATA_BYTE == 4
make_helper(concat(movzx_, SUFFIX)) {
uint32_t decode_eip = cpu.eip + 2;
op_src->size = 2;
int len = read_ModR_M(decode_eip, op_src, op_dest);
op_dest->val = reg_l(op_dest->reg);

DATA_TYPE result = op_src->val;
OPERAND_W(op_dest, result);
print_asm("movzx" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
return len + 1;
}*/
//#endif
/*static void do_execute() {
  OPERAND_W(op_dest, (DATA_TYPE)op_src->val);
//printf("%x\n", )
print_asm_template1();
}

make_instr_helper(rm2r)

make helper(concat(movzx, SUFFIX)) {

}*/
//#undef instr

//#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	if ((ops_decoded.opcode & 0xff) == 0xb6)
	{
		if (op_src->reg >= 0 && op_src->reg < 8)
			op_src->val = reg_b(op_src->reg);
		op_src->val &= 0xff;
	} else
		op_src->val &= 0xffff;
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

