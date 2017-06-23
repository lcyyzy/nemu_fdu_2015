#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cmovne-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovne-template.h"
#undef DATA_BYTE

make_helper_v(cmovne_rm2r)
