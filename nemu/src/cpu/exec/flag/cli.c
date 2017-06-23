#define DATA_BYTE 2
#include "cli-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cli-template.h"
#undef DATA_BYTE

make_helper_v(cli_r)
