ENTRY(_kernel_64start)

SECTIONS {
	. = 0xffff800000000000;
	
	.text : {
		*(.text .text.*)
	}

	.rodata : {
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	.data : {
		*(.data .data.*)
	}

	.bss : {
		*(COMMON .bss .bss.*)
	}
	
	. = 0xffffffffffffc000;
	.stack : {
		*(.kernelstack)
	}
}
