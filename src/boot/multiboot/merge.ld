ENTRY(_start)

SECTIONS
{
	. = 1M;

	/* Kernel code */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text .text.*)
	}

	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data .data.*)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON .bss .bss.*)
	}

	/* Packed kernel */
	.kernel BLOCK(4K) : ALIGN(4K)
	{
		*(.kernel)	
	}
}
