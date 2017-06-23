#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnl-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnl-template.h"
#undef DATA_BYTE

make_helper_v(jnl_i2r)
