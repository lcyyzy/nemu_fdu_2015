#include "cpu/exec/helper.h"

make_helper(hlt) {
	Assert(cpu.IF == 1, "hlt!");
	while (!cpu.INTR);
	print_asm("hlt");
	return 1;
}
