#include "cpu/exec/template-start.h"

#define instr shl

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;
	uint8_t count = src & 0x1f;
	/*cpu.CF = MSB(dest << (count - 1));
	if (count == 1 && (MSB(dest) != MSB(dest << count)))
		cpu.OF = 1;
	else
		cpu.OF = 0;*/
	dest <<= count;
	cpu.ZF = !dest;	
	OPERAND_W(op_dest, dest);

	/* There is no need to update EFLAGS, since no other instructions 
	 * in PA will test the flags updated by this instruction.
	 */
	

	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
