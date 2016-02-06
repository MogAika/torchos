#include <stdint.h>

#include <boot/multiboot/multiboot.h>
#include <boot/multiboot/terminal.h>

void multiboot_loader(uint32_t uMagic, multiboot_info_t *pMultiboot) {
	terminal_initialize();

	terminal_setcolor(make_color(COLOR_WHITE, COLOR_BROWN));
	terminal_clear();

	terminal_printf("Multiboot struct place: 0x%p\n", pMultiboot);
	terminal_printf("Multiboot magic: 0x%p\n", uMagic);
}
