#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnle-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnle-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnle-template.h"
#undef DATA_BYTE

make_helper_v(jnle_i2r)