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
	

.section .bss, "aw", @nobits
.align 16
.skip BOOT_STACK_SIZE
stack:
