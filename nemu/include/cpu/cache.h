#ifndef __CPU_CACHE_H__
#define __CPU_CACHE_H__

#include "common.h"

typedef struct { 
	bool valid;
	uint32_t tag;// 16 bits
	uint8_t block[64];
} CPU_cacheL1;

typedef struct {
	bool valid,dirty;
	uint32_t tag;
	uint8_t block[64];
} CPU_cacheL2;

typedef union {
	struct {
		uint32_t offset : 6;
		uint32_t set    : 7;
		uint32_t tag    : 14;
	};
	uint32_t addr;
} CacheL1_addr;

typedef union {
	struct {
		uint32_t offset : 6;
		uint32_t set    : 12;
		uint32_t tag    : 9;
	};
	uint32_t addr;
} CacheL2_addr;


#endif

