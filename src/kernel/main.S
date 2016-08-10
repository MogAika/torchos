.code64

.set BOOT_STACK_SIZE,   0x4000

.section .text
.align 8
_kernel_64start:
.global _kernel_64start
.type _kernel_64start , @function
	mov $__kernel_stack, %rsp
	call kernel_main
cycle:
	cli
	hlt
	jmp cycle

.section .kernelstack, "aw", @nobits
.align 16
.skip BOOT_STACK_SIZE
__kernel_stack:
