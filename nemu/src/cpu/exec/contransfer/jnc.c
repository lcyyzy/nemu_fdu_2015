#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnc-template.h"
#undef DATA_BYTE

make_helper_v(jnc_i2r)
