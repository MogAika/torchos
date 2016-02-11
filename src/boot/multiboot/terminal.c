#include <stdarg.h>
#include <stdint.h>

#include <boot/multiboot/terminal.h>
#include <boot/multiboot/minilib.h>
 
uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void terminal_putchar(char c) {
	if (c != '\n')
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH || c == '\n') {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT) {
			for (size_t i = 0; i < VGA_HEIGHT - 1; i++) {
				for (size_t j = 0; j < VGA_WIDTH; j++)
					terminal_buffer[i * VGA_WIDTH + j] = terminal_buffer[(i+1) * VGA_WIDTH + j];
			}
			for (size_t j = 0; j < VGA_WIDTH; j++) {
				terminal_buffer[(VGA_HEIGHT-1) * VGA_WIDTH + j] = make_vgaentry(' ', terminal_color);
			}
			terminal_row--;
		}
	}
}
 
void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}

void terminal_clear() {
	uint16_t cls = make_vgaentry(' ', terminal_color);
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		terminal_buffer[i] = cls;
	}
}

void terminal_printf(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	char c;
	while ((c = *format)) {
		if (c == '%') {
			c = *(++format);
			const size_t intbuf_size = 33;
			char intbuf[intbuf_size];
			switch (c) {
				case 'p':
				case 'x':
					terminal_writestring(utoa_s(va_arg(ap, unsigned), intbuf, intbuf_size, 16));
					break;
				case 'u':
					terminal_writestring(utoa_s(va_arg(ap, unsigned), intbuf, intbuf_size, 10));
					break;
				case 'd':
					terminal_writestring(itoa_s(va_arg(ap, int), intbuf, intbuf_size, 10));
					break;			
				case 'b':
					terminal_writestring(utoa_s(va_arg(ap, unsigned), intbuf, intbuf_size, 2));
					break;
				default:
					terminal_putchar('%');
				case '%':
				case 'c':
					terminal_putchar(c);
					break;
				case 's':
					terminal_writestring(va_arg(ap, const char*));
					break;
			}
		} else
			terminal_putchar(c);
		format++;
	}
	va_end(ap);
}
