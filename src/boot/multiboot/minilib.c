#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <boot/multiboot/minilib.h>

size_t strlen(const char* str) {
	size_t size = 0;
	while (str[size])
		size++;
	return size;
}

void *memset(void *ptr, int value, size_t num) {
	uint8_t v1 = value & 0xff;
	uint8_t *p = (uint8_t*) ptr;
	if (num > 15) {
		uint32_t v4 = ((uint32_t)v1 << 24) | ((uint32_t)v1 << 16) | ((uint32_t)v1 << 8) | (uint32_t)v1;
		for (; num > 3; num -= 4, p += 4)
			*((uint32_t*)p) = v4;
	}
	for (; num; num--, p++)
		*p = v1;

	return ptr;
}

void *memcpy(void *dest, const void *src, size_t num) {
	uint8_t *s = (uint8_t*) src;
	uint8_t *d = (uint8_t*) dest;
	if (num > 15) {
		for (; num > 3; num -= 4, d += 4, s += 4)
			*((uint32_t*)d) = *((uint32_t*)s);
	}
	for (; num; num--, s++, d++)
		*d = *s;

	return dest;
}

char *reverse(char *s) {
	int i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		swap(s[i], s[j]);
	}
	return s;
}

char *itoa_s(int32_t value, char* buffer, size_t bufsize, int radix) {
	bool minus = value < 0;
	size_t i = 0;
	if (minus)
		bufsize--;
	do {
		int val = value % radix;
		if(minus)
			val = -val;
		buffer[i++] = (char) val + ((val < 10) ? '0' : 'A' - 10);
		value /= radix;           
	} while (i < bufsize && value);
	if (minus)
		buffer[i++] = '-';
	buffer[i] = 0;
	return reverse(buffer);
}

char *itoa(int32_t value, char* buffer, int radix) {
	return itoa_s(value, buffer, 64, radix);
}

char *utoa_s(uint32_t value, char* buffer, size_t bufsize, unsigned radix) {
	size_t i = 0;
	do {
		unsigned val = value % radix;
		buffer[i++] = (char) val + ((val < 10) ? '0' : 'A' - 10);
		value /= radix;
	} while( i < bufsize && value != 0);
	buffer[i] = 0;
	return reverse(buffer);
}

char *utoa(uint32_t value, char* buffer, int radix) {
	return utoa_s(value, buffer, 64, radix);
}
