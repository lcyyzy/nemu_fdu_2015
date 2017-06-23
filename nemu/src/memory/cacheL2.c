#include "misc.h"
#include "burst.h"
#include "stdlib.h"

#define BLK_WIDTH 6
#define SET_WIDTH 12
#define WAY_WIDTH 4 
#define TAG_WIDTH (27 - BLK_WIDTH - SET_WIDTH)

typedef union {
	struct { 
		uint32_t offset : BLK_WIDTH;
		uint32_t set : SET_WIDTH;
		uint32_t tag : TAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;

#define CACHE_BLK_SIZE (1 << BLK_WIDTH)
#define OFFSET_MAX (CACHE_BLK_SIZE - 1)
#define CACHE_WAY (1 << WAY_WIDTH)
#define CACHE_SET (1 << SET_WIDTH)
#define CACHE_TAG (1 << TAG_WIDTH)

typedef struct { 
	bool valid;
	bool dirty;
	uint32_t tag;
	uint8_t block[64];
} CACHEL2;

CACHEL2 cacheL2[CACHE_SET][CACHE_WAY];

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_cacheL2() {
	int i,j;
	for (i = 0;i < CACHE_SET; i ++) 
		for (j = 0;j < CACHE_WAY; j ++) { 
			cacheL2[i][j].valid = false;
			cacheL2[i][j].dirty = false;
		}
}

static bool c_read(hwaddr_t addr, void *data, size_t len) {
	cache_addr temp;
	temp.addr = addr;
	uint32_t tag = temp.tag;
	uint32_t set = temp.set;
	uint32_t offset = temp.offset;
	int i, way;
	bool hit = false;
	for (i = 0;i < CACHE_WAY; i ++) {
		if (cacheL2[set][i].valid && cacheL2[set][i].tag == tag) {
			hit = true;
			break;
		}
	}
	if (hit) memcpy(data, cacheL2[set][i].block + offset, len);
	else { 
		bool empty = false;
		for (i = 0;i < CACHE_WAY; i ++) { 
			if (cacheL2[set][i].valid == false) { 
				empty = true;
				break;
			}
		}
		if (empty) 
			way = i;
		else { 
			way = 0;
			if(cacheL2[set][way].dirty == true) {
				/* write back */
				cache_addr replace;
				replace.offset = 0;
				replace.set = set;
				replace.tag = cacheL2[set][way].tag;
				for(i = 0; i < CACHE_BLK_SIZE; i++)
					dram_write(replace.addr + i, 1, cacheL2[set][way].block[i]); 
			}
		}
		for (i = 0;i < CACHE_BLK_SIZE; i ++) { 
			cacheL2[set][way].block[i] = dram_read(temp.addr + i, 1);
		}
		cacheL2[set][way].tag = tag;
		cacheL2[set][way].valid = true;
		cacheL2[set][way].dirty = false;
	}
	return hit;
}

static bool c_write(hwaddr_t addr, void *data, size_t len) {
	cache_addr temp;
	temp.addr = addr;
	uint32_t tag = temp.tag;
	uint32_t set = temp.set;
	uint32_t offset = temp.offset;
	int i;
	bool hit = false;
	for (i = 0;i < CACHE_WAY; i ++) 
		if (cacheL2[set][i].valid && cacheL2[set][i].tag == tag) {
			hit = true;
			break;
		}	
	if (hit) {
		memcpy(cacheL2[set][i].block + offset, data, len);
		cacheL2[set][i].dirty = true;
	}
	else {
		int i,way;
		bool empty = false;
		temp.offset = 0;
		for(i = 0; i < CACHE_WAY; i++) {
			if(cacheL2[set][i].valid == false) {
				empty = true;
				break;
			}
		}
		if(empty)
			way = i;
		else {
			way = (rand() % (CACHE_WAY - 1)) + 1;
			if(cacheL2[set][way].dirty == true) {
				/* write back */
				cache_addr replace;
				replace.offset = 0;
				replace.set = set;
				replace.tag = cacheL2[set][way].tag;
				for(i = 0; i < CACHE_BLK_SIZE; i++)
					dram_write(replace.addr + i, 1, cacheL2[set][way].block[i]);
			}
		}
		for(i = 0; i < CACHE_BLK_SIZE; i++)
			cacheL2[set][way].block[i] = dram_read(temp.addr + i, 1);
		cacheL2[set][way].valid = true;
		cacheL2[set][way].tag = tag;
		memcpy(cacheL2[set][way].block + offset, data, len);
		cacheL2[set][way].dirty = true;
	}			

	return hit;
} 

uint32_t cacheL2_read(hwaddr_t addr, size_t len) {
	uint32_t offset = addr & 0x3f;
	uint8_t temp[4];
	bool hit;
	uint32_t result;
	uint32_t shift = 0;

	if ((offset + len > 0x3f) && (len != 1)) {
		uint32_t len1, len2;
		bool hit1, hit2;
		len1 = 0x40 - offset;
		len2 = len - len1;
		hit1 = c_read(addr, temp, len1);
		hit2 = c_read(addr+len1, temp+len1, len2);
		hit = hit1 & hit2;
	}
	else
		hit = c_read(addr, temp, len);

	if(hit)
		result = *(uint32_t *)(temp + shift) & (~0u >> ((4 - len) << 3));
	else
		result = dram_read(addr, len) & (~0u >> ((4 -len) << 3));

	return result;
} 

void cacheL2_write(hwaddr_t addr,size_t len,uint32_t data) {
	uint8_t temp[4];
	uint32_t shift = 0;
	uint32_t offset = addr & 0x3f;
	*(uint32_t *)(temp + shift) = data;

	if((offset + len > 0x3f) && (len != 1)) {
		int len1,len2;
		len1 = 0x40 - offset;
		len2 = len - len1;
		c_write(addr, temp, len1);
		if(*(uint32_t *)(temp + len1) != 0)
			c_write(addr + len1, temp + len1, len2);
	}
	else
		c_write(addr, temp, len);
}

bool cacheL2_test(hwaddr_t addr) {
	cache_addr temp;
	temp.addr = addr;
	uint32_t offset = temp.offset;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	int i;
	bool hit = false;
	uint8_t data;

	for(i = 0; i < CACHE_WAY; i ++) {
		if(cacheL2[set][i].valid && (cacheL2[set][i].tag == tag)) {
			hit = true;
			printf("cacheL2 hit.\n");
			printf("cache_set = %x, cache_way = %x, cache_offset = %x\n", set, i, offset);
			memcpy((void *)&data, cacheL2[set][i].block + offset, 1);
			printf("data = %x\n", data);
			break;
		}
	}
	return hit;
}



/*#include "nemu.h"
#include "common.h"
#include "misc.h"
#include "cpu/cache.h"
#include "stdlib.h"
#include "time.h"

#define BLOCK_WIDTH 6
#define WAY_WIDTH 4
#define SET_WIDTH 12
#define TAG_WIDTH (27 - SET_WIDTH - BLOCK_WIDTH)

typedef union {
struct {
uint32_t offset : BLOCK_WIDTH;
uint32_t set : SET_WIDTH;
		uint32_t tag : TAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;

#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_WAY (1 << WAY_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_TAG (1 << TAG_WIDTH)


#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

CPU_cacheL2 cacheL2[NR_SET][NR_WAY];

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_cacheL2() {
	int i, j;
	for (i = 0; i < NR_SET; ++i) 
		for (j = 0; j < NR_WAY; ++j) 
			cacheL2[i][j].valid = cacheL2[i][j].dirty = false;
}

static void _cacheL2_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "addr = %x\n", addr);

	cache_addr temp;
	temp.addr = addr & (~BURST_MASK);
	uint32_t tag = temp.tag;
	uint32_t set = temp.set;
	uint32_t offset = temp.offset;
	int i, way;
	bool hit = false;
	for (i = 0; i < NR_WAY; ++i) 
		if (cacheL2[set][i].valid && cacheL2[set][i].tag == tag) {
			hit = true;
			break;
 		}
	if (hit) 
		memcpy(data, cacheL2[set][i].block + offset, BURST_LEN);
 	else {
		temp.addr = 0;
		temp.tag = tag;
		temp.set = set;
		hit = false;
		for (i = 0; i < NR_WAY; ++i) 
 			if (cacheL2[set][i].valid == false) {
				hit = true;
				break;
		 	}
		if (hit) 
			way = i;
 		else {
			way = rand() % NR_WAY;
		 	if (cacheL2[set][way].dirty == true) {
				temp.addr = 0;
				temp.set = set ;
				temp.tag = cacheL2[set][way].tag;
				for (i = 0; i < NR_BLOCK; ++i) 
					dram_write(temp.addr + i, 1, cacheL2[set][way].block[i]);
			}
		}
		for (i = 0; i < NR_BLOCK; ++i) 
			cacheL2[set][way].block[i] = dram_read(temp.addr + i, 1);
		memcpy(data, cacheL2[set][way].block + offset, BURST_LEN);
		cacheL2[set][way].tag = temp.tag;
		cacheL2[set][way].valid = true;
		cacheL2[set][way].dirty = false;
	}
}

static void _cacheL2_write(hwaddr_t addr, void *data, uint8_t *mask) {
	//Assert(addr < HW_MEM_SIZE, "addr = %x\n",addr);

	cache_addr temp;
	temp.addr = addr & (~BURST_MASK);
	uint32_t tag = temp.tag;
	uint32_t set = temp.set;
	uint32_t offset = temp.offset;
	int i, way;
	bool hit = false;
	for (i = 0; i < NR_WAY; ++i) 
		if (cacheL2[set][i].valid && cacheL2[set][i].tag == tag) {
			hit = true;
			break;
  		}
 	if (hit) {
		memcpy_with_mask(cacheL2[set][i].block + offset, data, BURST_LEN, mask);
		cacheL2[set][i].dirty = true;
 	}
 	else {
		temp.addr = 0;
		temp.tag = tag;
		temp.set = set;
		for (i = 0; i < NR_WAY; ++i) 
 			if (cacheL2[set][i].valid == false) {
				hit = true;
				break;
			}
		if (hit) way = i;
 		else {
			srand(time(0));
			way = rand() % NR_WAY;
			if (cacheL2[set][way].dirty == true) {
				temp.addr = 0;
				temp.set = set;
				temp.tag = cacheL2[set][way].tag;
				for (i = 0; i < NR_BLOCK; ++i) 
					dram_write(temp.addr + i, 1, cacheL2[set][way].block[i]);
 			}
		}
		for (i = 0; i < NR_BLOCK; ++i) 
			cacheL2[set][way].block[i] = dram_read(temp.addr + i, 1);
		memcpy_with_mask(cacheL2[set][way].block + offset, data, BURST_LEN, mask);
		cacheL2[set][way].tag = temp.tag;
		cacheL2[set][way].valid = true;
		cacheL2[set][way].dirty = true;
 	}
}

uint32_t cacheL2_read(hwaddr_t addr,size_t len) {
	//assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	_cacheL2_read(addr, temp);

 	if ((addr ^ (addr + len - 1)) & ~(BURST_MASK)) {
		_cacheL2_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	return *(uint32_t *)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void cacheL2_write(hwaddr_t addr,size_t len, uint32_t data) {
	//assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	_cacheL2_write(addr, temp, mask);

	if ((addr ^ (addr + len - 1)) & ~(BURST_MASK)) 
		_cacheL2_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
}*/

