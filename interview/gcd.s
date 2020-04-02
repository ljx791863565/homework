	.file	"gcd.c"
	.text
	.globl	gcd
	.type	gcd, @function
gcd:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	cmpl	$0, -24(%rbp)
	jne	.L2
	movl	-20(%rbp), %eax
	jmp	.L3
.L2:
	movl	-20(%rbp), %eax
	cltd
	idivl	-24(%rbp)
	movl	%edx, -4(%rbp)
	movl	-4(%rbp), %edx
	movl	-24(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	gcd
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	gcd, .-gcd
	.section	.rodata
.LC0:
	.string	"\350\257\267\350\276\223\345\205\245\344\270\244\344\270\252\346\225\264\346\225\260"
.LC1:
	.string	"%d %d"
	.align 8
.LC2:
	.string	"%d \345\222\214 %d \347\232\204\346\234\200\345\244\247\345\205\254\347\272\246\346\225\260\346\230\257\357\274\232%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$.LC0, %edi
	call	puts
	leaq	-8(%rbp), %rdx
	leaq	-12(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	movl	-8(%rbp), %edx
	movl	-12(%rbp), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	gcd
	movl	%eax, -4(%rbp)
	movl	-8(%rbp), %edx
	movl	-12(%rbp), %eax
	movl	-4(%rbp), %ecx
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.4) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
