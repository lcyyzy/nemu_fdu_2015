#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovnp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovnp-template.h"
#undef DATA_BYTE

make_helper_v(cmovnp_rm2r)
