#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr movsx

/*if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(movsx_rm2r_, SUFFIX)) {
	uint32_t decode_eip = cpu.eip + 2;
	op_src->size = 1;
	int len = read_ModR_M(decode_eip, op_src, op_dest);
	op_dest->val = REG(op_dest->reg);

	DATA_TYPE result = (int8_t)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm("movsx" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
	return len + 1;

}
#endif

//#if DATA_BYTE == 4
make_helper(concat(movsx_, SUFFIX)) {
	uint32_t decode_eip = cpu.eip + 2;
	op_src->size = 2;
	int len = read_ModR_M(decode_eip, op_src, op_dest);
	op_dest->val = REG(op_dest->reg);

	DATA_TYPE result = (int16_t)op_src->val;
	OPERAND_W(op_dest, result);
	print_asm("movsx" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
	return len + 1;
}
//#endif*/

static void do_execute() {
	if ((ops_decoded.opcode & 0xff) == 0xbe) {
		if (op_src->reg < 8)
			op_src->val = reg_b(op_src->reg);
		op_src->val &= 0xff;
		if (op_src->val & 0x80)	{
			if (ops_decoded.is_data_size_16)
				OPERAND_W(op_dest, op_src->val | 0xff00); 
			else
				OPERAND_W(op_dest, op_src->val | 0xffffff00);
		} 
		else
			OPERAND_W(op_dest, op_src->val);
	} 
	else {
		op_src->val &= 0xffff;
		if (op_src->val & 0x8000)
			OPERAND_W(op_dest, op_src->val | 0xffff0000); 
		else
			OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"
