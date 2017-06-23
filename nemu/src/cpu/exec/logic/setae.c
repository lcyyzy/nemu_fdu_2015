#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setae-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setae-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setae-template.h"
#undef DATA_BYTE

make_helper_v(setae_rm)
