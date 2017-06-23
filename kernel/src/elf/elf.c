#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
//void ramdisk_write(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();
//PDE* get_updir();
//PDE* get_kpdir();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;
	//PDE *kpdir, *updir;
	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
	//panic("please implement me");
	int i = 0;
	ph = (void *)(buf + elf->e_phoff);
	//uint8_t buf1[32];
	for (i = 0; i < elf->e_phnum; ++i, ++ph) {
		//ph = (void *)(buf + elf->e_phoff + i * elf->e_phentsize);
		/*ph =(void *) (elf + elf->e_ehsize + i * elf->e_phentsize);*/
		//ramdisk_read(buf1, 52 + i * 32, 32);
		//ph = (void *)buf1;
		/* Scan the program header table, load each segment into memory */
	 	if (ph->p_type == PT_LOAD) {
			
			/*mm_malloc(ph->p_vaddr, ph->p_memsz);
			kpdir = get_kpdir();
			updir = get_updir();
			kpdir[32].val = updir[32].val;
			ramdisk_read((uint8_t *)ph->p_vaddr, ph->p_offset, ph->p_filesz);
			memset((void *)(ph->p_vaddr + ph->p_filesz), 0, ph->p_memsz - ph->p_filesz);*/

			uint32_t hwaddr = mm_malloc(ph->p_vaddr, ph->p_memsz);
#ifdef HAS_DEVICE
			ide_read((uint8_t *)hwaddr, ph->p_offset, ph->p_filesz);
#else
			ramdisk_read((uint8_t *)hwaddr, ph->p_offset, ph->p_filesz);
#endif
			memset((void *)hwaddr + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */

			/*ramdisk_read((uint8_t*)ph->p_vaddr, ph->p_offset, ph->p_filesz);

TODO: zero the memory region 
[VirtAddr + FileSiz, VirtAddr + MemSiz)

uint8_t *p = NULL;
for (p = (uint8_t*)(ph->p_vaddr + ph->p_filesz); p < (uint8_t*)(ph->p_vaddr + ph->p_memsz); ++p) {
			 *p = 0;
			 }*/


#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	} 

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
