#ifndef _BOOT_MULTIBOOT_MINILIB_HEADER
#define _BOOT_MULTIBOOT_MINILIB_HEADER 1

#define swap(a,b) {typeof(a) t=a;a=b;b=t;}

size_t strlen(const char* str);

void *memset(void *ptr, int value, size_t num);

void *memcpy(void *dest, const void *src, size_t num);

char *reverse(char *s);

char *itoa_s(int32_t value, char* buffer, size_t bufsize, int radix);

char *itoa(int32_t  value, char* buffer, int radix);

char *utoa_s(uint32_t  value, char* buffer, size_t bufsize, unsigned radix);

char *utoa(uint32_t  value, char* buffer, int radix);

#endif
