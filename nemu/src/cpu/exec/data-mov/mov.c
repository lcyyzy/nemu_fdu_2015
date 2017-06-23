#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
//#include "cpu/reg.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

void sreg_load(uint8_t sreg) {
	Assert(cpu.cr0.PE, "Not in protection mode");
	uint16_t segsel = cpu.sreg[sreg].val;
	//printf("%x\n", segsel);
	uint16_t index = segsel >> 3;
	//printf("%x\n", index);
	uint8_t ti = (segsel >> 2) & 1;

	Assert(ti == 0, "LDT is not supported");
	Assert(index * 8 < cpu.gdtr.limit, "segment selector %x out of limit %x", index, cpu.gdtr.limit);

	uint64_t descriptor = ((uint64_t) lnaddr_read(cpu.gdtr.base + index * 8 + 4, 4) << 32) | lnaddr_read(cpu.gdtr.base + index * 8, 4);

	uint32_t limit = (descriptor & 0xFFFF) | ((descriptor >> 32) & 0xF0000);
	uint32_t base = ((descriptor >> 16) & 0xFFFFFF) | ((descriptor >> 32) & 0xFF000000);
	uint8_t granularity = (descriptor >> 55);
 	if (granularity) {
		limit <<= 12;
	} 
	//printf("%x %x\n", base, limit);
	cpu.sreg[sreg].base = base;
	cpu.sreg[sreg].limit = limit;
}


/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)

make_helper(mov_cr2r) {
	//int i;
	uint8_t opcode = instr_fetch(eip + 1, 1);
	switch (opcode) {
		case 0xc0:
			cpu.eax = cpu.cr0.val;
			print_asm("movcr2r %%%s, %%cr0", str(eax));
			break;
		case 0xd8:
			cpu.eax = cpu.cr3.val;
			//for (i = 0; i < TLB_NUM; ++i) {
			//tlb[i].valid = false;
			//}
			print_asm("movcr2r %%%s, %%cr3", str(eax));
			break;
		default:
			cpu.eax = cpu.cr0.val;
			print_asm("movcr2r %%%s, %%cr0", str(eax));
			break;
  	}
	return 2;
}

make_helper(mov_r2cr) {
	//int temp = cpu.cr3.val;
	uint8_t opcode = instr_fetch(eip + 1, 1);
 	switch (opcode) {
		case 0xc0:
			cpu.cr0.val = cpu.eax;
			print_asm("movr2cr %%cr0, %%%s", str(eax));
			break;
		case 0xd8:
			cpu.cr3.val = cpu.eax;

			print_asm("movr2cr %%cr3, %%%s", str(eax));
			break;
		default:
			cpu.cr0.val = cpu.eax;
			print_asm("movr2cr %%cr0, %%%s", str(eax));
			break;
 	}
 	//if (cpu.cr3.val != temp) {
		//printf("0x%x\n", cpu.eip);
		//printf("qian %x  hou %x\n", temp, cpu.cr3.val);
		//assert(0);
  	//}
	return 2;
}


/*make_helper(mov_s2rm) {
  ModR_M m;
  m.val = instr_fetch(eip + 1, 1);
  if(m.mod == 3) {
  reg_w(m.R_M) = sreg(m.reg).val;
  print_asm("mov" str(SUFFIX) " %%%s,%%%s", sregs[m.reg], regsw[m.R_M]);
  return 2;
  }
  else {
  swaddr_t addr = 0;
  int len = read_ModR_M(eip + 1, op_src, op_dest);
  swaddr_write(addr, 2, sreg(m.reg).val);

  print_asm("mov" str(SUFFIX) " %%%s,%s", sregs[m.reg], str(sth));
  return len + 1;
  } 
  }*/

make_helper(mov_rm2s) {
	uint8_t opcode = instr_fetch(eip + 1, 1);
	switch(opcode) {
		case 0xd8:
			cpu.ds.val = reg_w(R_EAX);
			current_sreg = R_DS;
			sreg_load(current_sreg);
			print_asm("movseg %%%s, ds", str(eax));
			break;
		case 0xc0:
			cpu.es.val = reg_w(R_EAX);
			current_sreg = R_ES;
			sreg_load(current_sreg);
			print_asm("movseg %%%s, es", str(eax));
			break;
		case 0xd0:
			cpu.ss.val = reg_w(R_EAX);
			current_sreg = R_SS;
			sreg_load(current_sreg);
			print_asm("movseg %%%s, ss", str(eax));
			break;
		default:
			break;
 	}
	return 2;

	/*ModR_M m;
	  m.val = instr_fetch(eip + 1, 1);
	  if(m.mod == 3) {
	  sreg(m.reg).val = reg_w(m.R_M);
	  cpu.sreg[m.reg].base = 0;
	  cpu.sreg[m.reg].limit = 0xffffffff;
	//sreg_dirty[m.reg] = 1;
		//print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), sreg_str[m.reg]);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", regsw[m.R_M], sregs[m.reg]);
		return 2;
	} 
	else {
		swaddr_t addr = 0;
		int len = read_ModR_M(eip + 1, op_src, op_dest);
		sreg(m.reg).val = swaddr_read(addr, 2);
		cpu.sreg[m.reg].base = 0;
		cpu.sreg[m.reg].limit = 0xffffffff;
		//sreg_dirty[m.reg] = 1;
		//print_asm("mov" str(SUFFIX) " %s,%%%s", ModR_M_asm, s_reg_str[m.reg]);
		print_asm("mov" str(SUFFIX) "%%%s", sregs[m.reg]);

		return len + 1;
	}*/ 
}
