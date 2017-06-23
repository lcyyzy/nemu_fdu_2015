#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setp-template.h"
#undef DATA_BYTE

make_helper_v(setp_rm)
