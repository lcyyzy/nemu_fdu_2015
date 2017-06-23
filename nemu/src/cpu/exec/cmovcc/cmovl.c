#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovl-template.h"
#undef DATA_BYTE

make_helper_v(cmovl_rm2r)
