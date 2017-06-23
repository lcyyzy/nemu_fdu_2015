#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setle-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setle-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setle-template.h"
#undef DATA_BYTE

make_helper_v(setle_rm)
