#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"


enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS, R_FS, R_GS};

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */
typedef struct {
	union { 
		struct { 
			struct {
				uint32_t RPL : 2;
				uint32_t TI : 1;
				uint32_t INDEX : 13;
			};
			SegDesc invi;
		};
		uint16_t val;
	};
	//uint16_t selector;
	uint32_t limit;
	uint32_t base;
} SREG;

typedef struct {
	union {
		union {
			uint32_t _32;
			uint16_t _16;
			uint8_t _8[2];
		} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
		struct {
			uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
			swaddr_t eip;
			union {
				uint32_t EFLAGS;
				struct {
					uint32_t CF:1;
					uint32_t :1;
					uint32_t PF:1;
					uint32_t :1;
					uint32_t AF:1;
					uint32_t :1;
					uint32_t ZF:1;
					uint32_t SF:1;
					uint32_t TF:1;
					uint32_t IF:1;
					uint32_t DF:1;
					uint32_t OF:1;
					uint32_t OLIP:2;
					uint32_t NT:1;
					uint32_t :1;
					uint32_t RF:1;
					uint32_t VM:1;
					uint32_t :14;
				};
			};
		};
	};
	struct GDTR {
		uint32_t limit;
	    lnaddr_t base;
	} gdtr;
	struct IDTR {
		uint32_t limit;
		lnaddr_t base;
	} idtr;
	union {
		struct {	
			uint32_t PE:1;	
			uint32_t MP:1;	
			uint32_t EM:1;	
			uint32_t TS:1;	
			uint32_t ET:1;	
			uint32_t NE:1;	
			uint32_t :10;	
			uint32_t WP:1;	
			uint32_t :1;	
			uint32_t AM:1;	
			uint32_t :10;	
			uint32_t NW:1;	
			uint32_t CD:1;	
			uint32_t PG:1;
		};
		uint32_t val;
	} cr0;
	union {
		struct {
			uint32_t pad0 : 3;
			uint32_t page_write_through : 1;
			uint32_t page_cache_disable : 1;
			uint32_t pad1 : 7;
			uint32_t page_directory_base : 20;
		};
		uint32_t val;
	} cr3;
	union {
		SREG sreg[6];
		struct {
			SREG es, cs, ss, ds, fs, gs;
		};
	};	
	volatile bool INTR;

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])
#define sreg(index) (cpu.sreg[index])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* sregs[];

//void sreg_load(uint8_t sreg);

#endif