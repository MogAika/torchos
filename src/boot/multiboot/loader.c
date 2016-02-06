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
}
