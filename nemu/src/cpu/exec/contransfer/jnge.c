#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jnge-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jnge-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jnge-template.h"
#undef DATA_BYTE

make_helper_v(jnge_i2r)
