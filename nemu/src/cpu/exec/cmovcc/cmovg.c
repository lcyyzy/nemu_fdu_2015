#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovg-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovg-template.h"
#undef DATA_BYTE

make_helper_v(cmovg_rm2r)
