#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

char test[] = "KOKOKO PO ZERNISHKU";

char nil[256];

void kernel_main() {
	for (size_t i = 0; i < 32; i++) {
		((uint64_t*)(0xB8000))[i] = 0x1F201F201F201F20;
	}
}
