#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovo-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovo-template.h"
#undef DATA_BYTE

make_helper_v(cmovo_rm2r)
