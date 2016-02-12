#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <cpuid.h>

#include <boot/multiboot/minilib.h>
#include <boot/multiboot/multiboot.h>
#include <boot/multiboot/terminal.h>
#include <boot/kernel_input.h>
#include <elf64.h>

extern uint8_t __PACKED_KERNEL_START[];
extern uint8_t __PACKED_KERNEL_END[];

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

bool init() {
	terminal_initialize();

	terminal_setcolor(make_color(FGCOLOR, BGCOLOR));
	terminal_clear();

	terminal_printf("Welcome to ");
	terminal_setcolor(make_color(COLOR_MAGENTA, BGCOLOR));
	terminal_printf("%s", __KERNEL_NAME);

	terminal_setcolor(make_color(FGCOLOR, BGCOLOR));
	terminal_printf(" kernel loader\n");
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

bool process_multiboot(uint32_t uMagic, multiboot_info_t *pMultiboot) {
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
		terminal_printf("mem_lower = %uKB, mem_upper = %uKB (%uMb), total: %uMb(%ukB)\n",
			pMultiboot->mem_lower, pMultiboot->mem_upper, pMultiboot->mem_upper / 1024,
			memtotal / 1024, memtotal);
	} else
		return false;

	if (have_memmap) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)pMultiboot->mmap_addr;
		uint32_t mmapEnd = pMultiboot->mmap_addr + pMultiboot->mmap_length;

		terminal_printf ("Multiboot mmap_addr: 0x%p, mmap_length: 0x%x\nMemmap:\n",
			pMultiboot->mmap_addr, pMultiboot->mmap_length);

		while((uint32_t)mmap < mmapEnd) {
			terminal_printf(" (0x%p[%u]) addr: 0x%p:%p, len: 0x%x:%x, type: 0x%x\n", mmap, mmap->size,
				(uint32_t)(mmap->addr >> 32), (uint32_t)(mmap->addr & 0xffffffff),
				(uint32_t)(mmap->len >> 32), (uint32_t)(mmap->len & 0xffffffff), mmap->type);

			mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
		}
	}

	return true;
}

bool load_kernel(uint8_t *pack_start, uint8_t *pack_end) {
	terminal_printf("Packed kernel start: 0x%p; size: %u\n",
		pack_start, pack_end - pack_start);
	return true;
}

void multiboot_loader(uint32_t uMagic, multiboot_info_t *pMultiboot) {
	if (!init())
		return;

	if (!process_cpu_features())
		return;

	if (!process_multiboot(uMagic, pMultiboot))
		return;

	load_kernel(__PACKED_KERNEL_START, __PACKED_KERNEL_END);
}
