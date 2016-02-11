#ifndef MINILIB_H
#define MINILIB_H 1

#define swap(a,b) {typeof(a) t=a;a=b;b=t;}

size_t strlen(const char* str);

char *reverse(char *s);

char *itoa_s(int value, char* buffer, size_t bufsize, int radix);

char *itoa(int value, char* buffer, int radix);

char *utoa_s(unsigned value, char* buffer, size_t bufsize, unsigned radix);

char *utoa(unsigned value, char* buffer, int radix);

#endif
