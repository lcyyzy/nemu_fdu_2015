#include "common.h"
#include "memory/memory.h"
#include "cpu/reg.h"

extern uint8_t current_sreg;
lnaddr_t seg_translate(swaddr_t, size_t, uint8_t);

hwaddr_t page_translate(lnaddr_t lnaddr); 

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */

uint32_t cacheL1_read(hwaddr_t, size_t);
void cacheL1_write(hwaddr_t, size_t, uint32_t);

uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

uint32_t is_mmio(hwaddr_t);

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//assert(len == 1 || len == 2 || len == 4);
	//assert((cacheL1_read(addr, len)) == (dram_read(addr, len) & (~0u >> ((4 - len) << 3))));
	//printf("hwaddr: addr: %x\n", addr);
	int map_NO = is_mmio(addr);
	//return cacheL1_read(addr, len);
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	//return map_NO == -1 ? cacheL1_read(addr, len) & (~0u >> ((4 - len) << 3)) : mmio_read(addr, len, map_NO) & (~0u >> ((4 - len) << 3));
	return map_NO == -1 ? dram_read(addr, len) & (~0u >> ((4 - len) << 3)) : mmio_read(addr, len, map_NO) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//assert(len == 1 || len == 2 || len == 4);
	//cacheL1_write(addr, len, data);
	//dram_write(addr, len, data);
	int32_t map_NO = is_mmio(addr);
	//map_NO == -1 ? cacheL1_write(addr, len, data) : mmio_write(addr, len, data, map_NO);
	map_NO == -1 ? dram_write(addr, len, data) : mmio_write(addr, len, data, map_NO);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//assert(len == 1 || len == 2 || len == 4);
	size_t max_len = ((~addr) & 0xFFF) + 1;
	hwaddr_t hwaddr = addr;
 	if (len > max_len) {
		//assert(0);
		uint32_t low = lnaddr_read(addr, max_len);
		uint32_t high = lnaddr_read(addr + max_len, len - max_len);
		return (high << (max_len * 8)) | low;
	}
	else {
		//if (cpu.cr0.PG & cpu.cr0.PE) {
			//printf("page: %x\n", addr);
			hwaddr = page_translate(addr);	
 		//}
		//else {
			//hwaddr = addr;
 		//}
 	}
	return hwaddr_read(hwaddr, len);
	//hwaddr_t hwaddr = page_translate(addr);
		//return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	size_t max_len = ((~addr) & 0xFFF) + 1;
	hwaddr_t hwaddr = addr;
	if (len > max_len) {
		//assert(0);
		lnaddr_write(addr, max_len, data & ((1 << (max_len * 8)) - 1));
		lnaddr_write(addr + max_len, len - max_len, data >> (max_len * 8));
		return;
    	}
 	else {
 		if (cpu.cr0.PG & cpu.cr0.PE) {
			hwaddr = page_translate(addr);	
  	 	}
 		else {
			hwaddr = addr;
  	 	}
  	}
	hwaddr_write(hwaddr, len, data);
	//hwaddr_t hwaddr = page_translate(addr);
		//hwaddr_write(hwaddr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
	//printf("%d\n", len);
	//assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = addr;//seg_translate(addr, len, current_sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
	//assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr = addr;//seg_translate(addr, len, current_sreg);
	lnaddr_write(lnaddr, len, data);
}


