#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setnl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setnl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setnl-template.h"
#undef DATA_BYTE

make_helper_v(setnl_rm)
