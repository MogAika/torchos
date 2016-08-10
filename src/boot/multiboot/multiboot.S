.code32

.set BOOT_STACK_SIZE,   0x200

.set MULTIBOOT_FLAGS,   0x00000003
.set MULTIBOOT_MAGIC,   0x1BADB002
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

.set SEGMENT_32CODE, 0x8
.set SEGMENT_32DATA, 0x10
.set SEGMENT_64CODE, 0x18
.set SEGMENT_64DATA, 0x20

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

	# use our GDT
	lgdt (_gdt)
	mov $SEGMENT_32DATA, %edx
	mov %edx, %ds
	mov %edx, %es
	mov %edx, %fs
	mov %edx, %gs
	mov %edx, %ss

	ljmp $SEGMENT_32CODE, $_gdt_apply
_gdt_apply:
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
	
	# prepare arguments
	mov 4(%esp), %edx
	mov 8(%esp), %ecx
	
	# enable paging
	mov %cr0, %eax
	bts $31, %eax
	mov %eax, %cr0
	
	# jmp to 64 bit long mode
	ljmp $SEGMENT_64CODE, $_goto64_64
	
.code64
_goto64_64:
	shl $32, %rcx
	add %rcx, %rdx
	jmp *%rdx

.section .data
.align 8
_gdt_base:
.quad 0x0000000000000000 # zero
.quad 0x00CF9A000000FFFF # code segment
.quad 0x00CF92000000FFFF # data segment
.quad 0x002098000000FFFF # code segment
.quad 0x000090000000FFFF # data segment
_gdt:
.word _gdt - _gdt_base - 1
.long _gdt_base

.section .bss, "aw", @nobits
.align 16
.skip BOOT_STACK_SIZE
stack:
