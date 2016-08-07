.code32

.set BOOT_STACK_SIZE,   0x200

.set MULTIBOOT_FLAGS,   0x00000003
.set MULTIBOOT_MAGIC,   0x1BADB002
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.align 4
.section .multiboot
.long MULTIBOOT_MAGIC
.long MULTIBOOT_FLAGS
.long MULTIBOOT_CHECKSUM

.align 4
.section .text
.global _start
.type _start , @function
_start:
	cli

	# setup stack
	movl $stack, %esp

	# push multiboot magic
	pushl %ebx

	# push multiboot struct
	pushl %eax

	call multiboot_loader
cycle:
	cli
	hlt
	jmp cycle

.global _goto64
.type _goto64, @function
_goto64:
	# enable PAE
	mov %cr4, %eax
	bts $5, %eax
	mov %eax, %cr4
	
	# pml4e table set
	movl pml4e, %eax
	movl %eax, %cr3
	
	# enable long mode (efer.lme=1)
	mov $0xC0000080, %ecx
	rdmsr
	bts $8, %eax
	wrmsr
	
	# enable paging
	mov %cr0, %eax
	bts $31, %eax
	mov %eax, %cr0
	
	# jmp to 64 bit long mode
	# TODO: IMPORTANT: MAKE OWN GDT IN 32 BIT. $8 only work in GRUB
	ljmp $8, $	_goto64_64
	
.code64
_goto64_64:
	hlt

.section .data
.align 8

.section .bss, "aw", @nobits
.align 16
.skip BOOT_STACK_SIZE
stack:
