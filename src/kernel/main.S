.code64

.section .text
.align 8
_kernel_64start:
.global _kernel_64start
.type _kernel_64start , @function
	shl $32, %rax
	add %rbx, %rax
	mov %rax, %rsp
	call kernel_main
cycle:
	cli
	hlt
	jmp cycle
