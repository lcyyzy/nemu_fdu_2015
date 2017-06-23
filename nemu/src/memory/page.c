#include <nemu.h>

#define DIR(addr) ((addr)>>22)
#define PAGE(addr) (((addr)>>12) & 0x3ff)
#define OFFSET(addr) ((addr) & 0xfff)


uint32_t readTLB(uint32_t va);
void writeTLB(uint32_t va, uint32_t pa);
void inittable();

typedef union {
	struct {
		uint32_t P	:1;
		uint32_t W	:1;
		uint32_t U	:1;
		uint32_t PWT	:1;
		uint32_t PCD	:1;
		uint32_t A	:1;
		uint32_t D	:1;
		uint32_t zero	:2;
		uint32_t AVL	:3;
		uint32_t page_base	:20;
	};
	uint32_t val;
}PAGE_DES;

hwaddr_t page_translate(lnaddr_t addr) {
	PAGE_DES dir;
	PAGE_DES page;
	hwaddr_t hwaddr;
	//inittable();
	if (cpu.cr0.PG == 0)
		return addr;
 	if ((hwaddr = readTLB(addr)) != -1) {
		//printf("tlb\n");
		return hwaddr + OFFSET(addr);
	}
	dir.val = hwaddr_read((cpu.cr3.page_directory_base << 12) + (DIR(addr) << 2), 4);
	Assert(dir.P, "dir do not exist at %x", addr);
	//if (dir.P == 0)
		//return addr;
	page.val = hwaddr_read((dir.page_base << 12) + (PAGE(addr) << 2), 4);
	Assert(page.P, "page do not exist at %x", addr);
	//if (page.P == 0)
		//return addr;
	hwaddr = (page.page_base << 12) + OFFSET(addr);
	writeTLB(addr, hwaddr);
	return hwaddr;
}

/*#include "common.h"
#include "memory/memory.h"
#include "cpu/reg.h"

hwaddr_t page_translate(lnaddr_t lnaddr) {
	//if (cpu.cr0.PG == 0)
		//return lnaddr;
	//printf("lnaddr: %x\n", lnaddr);
	//printf("cpu.cr3: %x\n", cpu.cr3.val);
	hwaddr_t pde_addr = (cpu.cr3.val & 0xfffff000) + ((lnaddr >> 20) & 0xffc);
	//printf("pde_addr: %x\n", pde_addr);
	uint32_t pde = hwaddr_read(pde_addr, 4);
	//printf("pde %x\n", pde);
	Assert((pde & 0x1) == 1, "PDE not present, address = %#10x", lnaddr);
	//if ((pde & 0x1) == 0)
		//return lnaddr;
	Assert(((pde >> 7) & 1) == 0, "4MB page not supported");
	hwaddr_t pte_addr = (pde & 0xFFFFF000) + ((lnaddr >> 10) & 0xffc);
	uint32_t pte = hwaddr_read(pte_addr, 4);
	//printf("pte %x\n", pte);
	//if ((pte & 0x1) == 0)
		//return lnaddr;
	//printf("excuse me?\n");
	Assert((pte & 0x1) == 1, "PTE not present, address = %#10x", lnaddr);
	hwaddr_t hwaddr = (pte & 0xFFFFF000) | (lnaddr & 0xFFF);
	return hwaddr;
}*/

/*void page_show(lnaddr_t lnaddr) {
	if (!cpu.cr0.PG) {
		printf("Paging disabled\n");
	} 
	else {
		hwaddr_t pde_addr = (cpu.cr3 & 0xFFFFF000) + ((lnaddr >> 20) & 0xFFC);
		uint32_t pde = hwaddr_read(pde_addr, 4);
		printf("PDE:\t%#10x\n", pde);
 		if ((pde & 1) == 0) {
			printf("PDE invalid, address = %#10x\n", lnaddr);
			return;
		}
		Assert(((pde >> 7) & 1) == 0, "4MB page not supported");
		hwaddr_t pte_addr = (pde & 0xFFFFF000) + ((lnaddr >> 10) & 0xFFC);
		uint32_t pte = hwaddr_read(pte_addr, 4);
		printf("PTE:\t%#10x\n", pte);
		if ((pte & 1) == 0) {
			printf("PTE invalid, address = %#10x\n", lnaddr);
			return;
		}
		hwaddr_t hwaddr = (pte & 0xFFFFF000) + (lnaddr & 0xFFF);
		printf("hwaddr:\t%#10x\n", hwaddr);
	}
}*/
