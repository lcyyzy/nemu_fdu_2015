#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnae-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnae-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnae-template.h"
#undef DATA_BYTE

make_helper_v(jnae_i2r)
