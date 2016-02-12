ENTRY(_kernel_64start)

SECTIONS {
	. = 0xffff800000000000;
	
	.text BLOCK(4K) : ALIGN(4K) {
		*(.text .text.*)
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	.data BLOCK(4K) : ALIGN(4K) {
		*(.data .data.*)
	}

	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON .bss .bss.*)
	}
}
