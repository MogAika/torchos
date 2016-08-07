#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <cpuid.h>

#include <boot/multiboot/minilib.h>
#include <boot/multiboot/multiboot.h>
#include <boot/multiboot/terminal.h>
#include <boot/multiboot/pagealloc.h>
#include <boot/kernel_input.h>
#include <specs/elf64.h>

extern uint8_t __PACKED_KERNEL_START[];
extern uint8_t __PACKED_KERNEL_END[];
extern void _goto64();

#define FGCOLOR COLOR_WHITE
#define BGCOLOR COLOR_BLACK

void print_requirement(const char *text, bool have) {
	terminal_setcolor(make_color(FGCOLOR, BGCOLOR));
	terminal_printf(" - %s ", text);

	size_t textlen = strlen(text);
	for (size_t pneed = textlen; pneed < 32; pneed++) {
		terminal_putchar('.');
	}

	terminal_putchar('[');
	if (have) {
		terminal_setcolor(make_color(COLOR_GREEN, BGCOLOR));
		terminal_writestring("OK");
	} else {
		terminal_setcolor(make_color(COLOR_RED, BGCOLOR));
		terminal_writestring("FAIL");
	}
	terminal_setcolor(make_color(FGCOLOR, BGCOLOR));
	terminal_writestring("]\n");
}

bool init_screen() {
	terminal_initialize();

	terminal_setcolor(make_color(FGCOLOR, BGCOLOR));
	terminal_clear();

	terminal_printf("Welcome to ");
	terminal_setcolor(make_color(COLOR_MAGENTA, BGCOLOR));
	terminal_printf("%s", __KERNEL_NAME);

	terminal_setcolor(make_color(FGCOLOR, BGCOLOR));
	terminal_printf(" multiboot32 => kernel loader\n");
	return true;
}

bool process_cpu_features() {
	uint32_t cpuidmax = __get_cpuid_max(0, 0);
	bool have_cpuid = cpuidmax != 0;

	print_requirement("CPUID instruction", have_cpuid);
	if (!have_cpuid)
		return false;

	uint32_t a, b, c, d;
	uint32_t cpuidext = __get_cpuid(0x80000001, &a, &b, &c, &d);
	bool have_ext_cpuid = cpuidext != 0;
	print_requirement("CPUID 0x80000001", have_ext_cpuid);
	if (!have_ext_cpuid)
		return false;

	bool have_64bit = d & (1 << 29);
	print_requirement("64 bit longmode support", have_64bit);
	terminal_printf("cpuid[0x80000001] edx:%p ecx:%p\n", d, c);

	return have_64bit;
}

bool process_multiboot(uint32_t uMagic, multiboot_info_t *pMultiboot,
	uint32_t *total_memory, multiboot_memory_map_t **multiboot_mmap, uint32_t *mmap_lenght) {
	bool magic_match = uMagic == MULTIBOOT_BOOTLOADER_MAGIC;
	print_requirement("Multiboot magic match", magic_match);
	if (!magic_match) {
		terminal_printf("Multiboot magic not correct 0x%p != 0x%p\n",
			uMagic, MULTIBOOT_BOOTLOADER_MAGIC);
		return false;
	}
	
	bool have_meminfo = pMultiboot->flags & MULTIBOOT_INFO_MEMORY;
	bool have_memmap = pMultiboot->flags & MULTIBOOT_INFO_MEM_MAP;

	print_requirement("Multiboot memory info", have_meminfo);
	print_requirement("Multiboot memory map", have_memmap);

	terminal_printf("Multiboot struct: 0x%p; magic: 0x%p\n", pMultiboot, uMagic);

	if (magic_match) {
		terminal_printf("Multiboot magic correct. Flags: b%b\n", pMultiboot->flags);
	}

	if (have_meminfo) {
		uint32_t memtotal = pMultiboot->mem_upper ? (pMultiboot->mem_upper + 1024) : pMultiboot->mem_lower;
		*total_memory = memtotal;
		terminal_printf("mem_lower = %uKB, mem_upper = %uKB (%uMb), total: %uMb(%ukB)\n",
			pMultiboot->mem_lower, pMultiboot->mem_upper, pMultiboot->mem_upper / 1024,
			memtotal / 1024, memtotal);
	} else
		return false;

	if (have_memmap) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)pMultiboot->mmap_addr;
		uint32_t mmapEnd = pMultiboot->mmap_addr + pMultiboot->mmap_length;

		*multiboot_mmap = mmap;
		*mmap_lenght = pMultiboot->mmap_length;

		terminal_printf ("Multiboot mmap_addr: 0x%p, mmap_length: 0x%x\nMemmap:\n",
			pMultiboot->mmap_addr, pMultiboot->mmap_length);

		while((uint32_t)mmap < mmapEnd) {
			terminal_printf(" (0x%p[%u]) addr: 0x%l (%x:%x), len: 0x%l (%x:%x), type: 0x%x\n",
				mmap, mmap->size, mmap->addr, (uint32_t)(mmap->addr >> 32), (uint32_t) mmap->addr,
				mmap->len, (uint32_t)(mmap->len >> 32), (uint32_t) mmap->len, mmap->type);

			mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
		}
	}

	return true;
}


uint64_t load_kernel(uint8_t *pack_start, uint8_t *pack_end) {
	terminal_printf("Packed kernel start: 0x%p; size: %u\n",
		pack_start, pack_end - pack_start);

	Elf64_Ehdr_t *ehdr = (Elf64_Ehdr_t*) pack_start;
	terminal_printf("entry: 0x%l; phoff: 0x%l\n", ehdr->e_entry, ehdr->e_phoff);
	
	Elf64_Phdr_t *phdr = (Elf64_Phdr_t*) (pack_start + ehdr->e_phoff);
	for (int i = 0; i < ehdr->e_phnum; i++, phdr++) {
		if (phdr->p_type != ELF64_PT_LOAD)
			continue;

		terminal_printf("+ section [%c%c%c]:0x%l[0x%l] va:0x%l[0x%l]\n", (phdr->p_flags & ELF64_PF_X) ? 'X' : '-',
			(phdr->p_flags & ELF64_PF_W) ? 'W' : '-', (phdr->p_flags & ELF64_PF_R) ? 'R' : '-',
			phdr->p_offset, phdr->p_filesz, phdr->p_vaddr, phdr->p_memsz);	
		
		for (uint64_t ppos = 0; ppos < phdr->p_memsz; ppos += PAGE_SIZE) {
			uint64_t vaddr = phdr->p_vaddr + ppos;
			terminal_printf(" trying to alloc and map %l\n", vaddr);
			
			uint32_t pp = phys_alloc_to_virt4k(vaddr, !!(phdr->p_flags & ELF64_PF_W), !!(phdr->p_flags & ELF64_PF_X));
			if (ppos < phdr->p_memsz) {
				uint32_t csize = (phdr->p_memsz - ppos) > PAGE_SIZE ? PAGE_SIZE : (phdr->p_memsz - ppos);
				memcpy((void*) pp, (void*)(pack_start + phdr->p_offset + ppos), csize);
			}
		}
	}

	return true;
}

void multiboot_loader(uint32_t uMagic, multiboot_info_t *pMultiboot) {
	uint32_t uPhysicalMemoryTotal, uMultibootMmapLenght;
	multiboot_memory_map_t *pMultibootMmap;

	if (!init_screen())
		return;

	if (!process_cpu_features())
		return;

	if (!process_multiboot(uMagic, pMultiboot, &uPhysicalMemoryTotal, &pMultibootMmap, &uMultibootMmapLenght))
		return;

	phys_alloc_init(pMultibootMmap, uMultibootMmapLenght);
	
	if (!virt_init())
		return;
	
	load_kernel(__PACKED_KERNEL_START, __PACKED_KERNEL_END);
	
	_goto64();
}
