.code32

.set BOOT_STACK_SIZE,   0x400

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
	# setup stack
	movl (stack + BOOT_STACK_SIZE), %esp

	# push multiboot struct
	pushl %eax
	# push multiboot magic
	pushl %ebx

	call multiboot_loader
cycle:
	cli
	hlt
	jmp cycle

.section .bss, "aw", @nobits
.align 16
stack:
.skip BOOT_STACK_SIZE
