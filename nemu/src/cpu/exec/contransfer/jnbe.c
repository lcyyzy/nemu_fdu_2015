#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnbe-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnbe-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnbe-template.h"
#undef DATA_BYTE

make_helper_v(jnbe_i2r)
