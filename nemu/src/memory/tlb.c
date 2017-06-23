#include <stdlib.h>
#include <time.h>

#include <common.h>

#include "tlb.h"

/*#define TLB_WIDTH 6
#define TLB_SIZE (1 << TLB_WIDTH)
#define TLB_MASK (TLB_SIZE - 1)
#define RAND_BIT 4*/


TLB tlb[64];
int tlbcnt = 0;
int tlbtable[1024];
int j;
int flag = 0;
void inittable()
{
	flag = 1;
	for (j = 0; j < 1024; ++j) {
		tlbtable[j] = (rand() % (64 - 1)) + 1;
	}
}

uint32_t readTLB(uint32_t va) {
	int i;
	va = va & 0xfffff000;
	for (i = 0; i < 64; ++i)
		if (tlb[i].viaddr == va && (tlb[i].valid))
			return tlb[i].phaddr;
	return -1;
	//bool hit = 0;
	//uint32_t target = (addr >> RAND_BIT) & (TLB_MASK);
	//uint32_t offset = addr & 0xfff;
	/*uint32_t i;
	for (i = 0; i < TLB_SIZE; ++i){
	 	if ((tlb[i].valid) && (tlb[i].viaddr == (addr>>12))) {
			return tlb[i].phaddr;
		}
	}
	return -1;*/
} 

 void writeTLB(uint32_t va, uint32_t pa) {
	if (flag == 0)
		inittable();
	int i;
	va = va & 0xfffff000;
	pa = pa & 0xfffff000;
 	for (i = 0; i < 64; ++i) {
		if (tlb[i].valid == false) {
			tlb[i].viaddr = va;
			tlb[i].phaddr = pa;
			tlb[i].valid = true;
			return;
		}
	}
	//srand(time(0));
	//i = (rand() % (64 - 1)) + 1;
	//i = 5;
	i = tlbtable[tlbcnt];
	tlb[i].viaddr = va;
	tlb[i].phaddr = pa;
	tlb[i].valid = true;
	tlbcnt++;
	if (tlbcnt == 1024)
		tlbcnt = 0;
}

