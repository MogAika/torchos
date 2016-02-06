ENTRY(_start)

SECTIONS {
	. = 1M;

	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text .text.*)
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	.data BLOCK(4K) : ALIGN(4K) {
		__PACKED_KERNEL_START = .;
		kernel.o(.data)
		__PACKED_KERNEL_END = .;
		*(.data .data.*)
	}

	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON .bss .bss.*)
	}
}
