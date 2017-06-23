#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setb-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setb-template.h"
#undef DATA_BYTE

make_helper_v(setb_rm)

