#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovge-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovge-template.h"
#undef DATA_BYTE

make_helper_v(cmovge_rm2r)
