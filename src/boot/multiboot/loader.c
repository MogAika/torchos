#include <stdint.h>

#include <boot/multiboot/multiboot.h>
#include <boot/multiboot/terminal.h>

extern uint8_t __PACKED_KERNEL_START[];
extern uint8_t __PACKED_KERNEL_END[];

void multiboot_loader(uint32_t uMagic, multiboot_info_t *pMultiboot) {
	terminal_initialize();

	terminal_setcolor(make_color(COLOR_WHITE, COLOR_BROWN));
	terminal_clear();

	terminal_printf("Multiboot struct: 0x%p; magic: 0x%p\n", pMultiboot, uMagic);
	terminal_printf("Packed kernel start: 0x%p; size: %u\n",
		__PACKED_KERNEL_START, __PACKED_KERNEL_END - __PACKED_KERNEL_START);

	if (uMagic != MULTIBOOT_BOOTLOADER_MAGIC) {
		terminal_printf("Multiboot magic not correct 0x%p != 0x%p\n",
			uMagic, MULTIBOOT_BOOTLOADER_MAGIC);
		return;
	}
	
	terminal_printf("Multiboot magic correct. Flags: 0x%p\n", pMultiboot->flags);
	
	if (pMultiboot->flags & MULTIBOOT_INFO_MEMORY) {
		terminal_printf("mem_lower = %uKB, mem_upper = %uKB (%uMb)\n",
			pMultiboot->mem_lower, pMultiboot->mem_upper, pMultiboot->mem_upper / 1024);
	} else {
		terminal_printf("Multiboot config not provide memory info\n");
	}

	if (pMultiboot->flags & MULTIBOOT_INFO_MEM_MAP) {
		multiboot_memory_map_t *mmap = (multiboot_memory_map_t*)pMultiboot->mmap_addr;
		uint32_t mmapEnd = pMultiboot->mmap_addr + pMultiboot->mmap_length;

		terminal_printf ("Multiboot mmap_addr: 0x%p, mmap_length: 0x%x\nMemmap:\n",
			pMultiboot->mmap_addr, pMultiboot->mmap_length);

		while((uint32_t)mmap < mmapEnd) {
			terminal_printf(" (0x%p[%u]) addr: 0x%x%x, len: 0x%x%x, type: 0x%x\n", mmap, mmap->size,
				(uint32_t)(mmap->addr >> 32), (uint32_t)(mmap->addr & 0xffffffff),
				(uint32_t)(mmap->len >> 32), (uint32_t)(mmap->len & 0xffffffff), mmap->type);

			mmap = (multiboot_memory_map_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
		}
	} else {
		terminal_printf("Multiboot config not provide memory map\n");
	}
}
