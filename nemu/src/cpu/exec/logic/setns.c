#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setns-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setns-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setns-template.h"
#undef DATA_BYTE

make_helper_v(setns_rm)
