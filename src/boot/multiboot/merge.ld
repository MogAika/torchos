ENTRY(_start)

SECTIONS {
	. = 1M;
	__BOOT_IMAGE_START = .;

	.text : {
		*(.multiboot)
		*(.text .text.*)
	}

	.rodata : {
		__PACKED_KERNEL_START = .;
		kernel.o(.data)
		__PACKED_KERNEL_END = .;
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	.data : {
		*(.data .data.*)
	}

	.bss : {
		*(COMMON .bss .bss.*)
	}

	__BOOT_IMAGE_END = .;
}
