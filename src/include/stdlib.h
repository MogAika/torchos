#ifndef STDLIB_H
#define STDLIB_H 1

#include <stddef.h>
#include <stdbool.h>

#define swap(a,b) {typeof(a) t=a;a=b;b=t;}

size_t strlen(const char* str) {
	size_t size = 0;
	while(str[size])
		size++;
	return size;
}

char *reverse(char *s) {
	int i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		swap(s[i], s[j]);
	}
	return s;
}

char *itoa_s(int value, char* buffer, size_t bufsize, int radix) {
	bool minus = value < 0;
	size_t i = 0;
	if (minus)
		bufsize--;
	do {
		int val = value % radix;
		if(minus)
			val *= -1;
		buffer[i++] = (char) val + ((val < 10) ? '0' : 'A' - 10);
		value /= radix;           
	} while (i < bufsize && value);
	if (minus)
		buffer[i++] = '-';
	buffer[i] = 0;
	return reverse(buffer);
}

char *itoa(int value, char* buffer, int radix) {
	return itoa_s(value, buffer, 256, radix);
}

char *utoa_s(unsigned value, char* buffer, size_t bufsize, unsigned radix) {
	size_t i = 0;
	do {
		unsigned val = value % radix;
		buffer[i++] = (char) val + ((val < 10) ? '0' : 'A' - 10);
		value /= radix;
	} while( i < bufsize && value != 0);
	buffer[i] = 0;
	return reverse(buffer);
}

char *utoa(unsigned value, char* buffer, int radix) {
	return utoa_s(value, buffer, 256, radix);
}

#endif
