	.file	"test3.c"
	.text
	.p2align 4
	.type	mac_equals_normal, @function
mac_equals_normal:
.LFB15:
	.cfi_startproc
	movl	(%rsi), %eax
	cmpl	%eax, (%rdi)
	je	.L5
.L2:
	movl	$1, %eax
	testl	%eax, %eax
	sete	%al
	ret
	.p2align 4,,10
	.p2align 3
.L5:
	movzwl	4(%rsi), %eax
	cmpw	%ax, 4(%rdi)
	jne	.L2
	xorl	%eax, %eax
	testl	%eax, %eax
	sete	%al
	ret
	.cfi_endproc
.LFE15:
	.size	mac_equals_normal, .-mac_equals_normal
	.p2align 4
	.globl	test_with_inline
	.type	test_with_inline, @function
test_with_inline:
.LFB16:
	.cfi_startproc
	endbr64
	movl	(%rdi), %r8d
	cmpl	(%rsi), %r8d
	je	.L18
.L7:
	movl	$1, %eax
.L8:
	movl	(%rsi), %ecx
	xorl	$1, %eax
	cmpl	(%rdx), %ecx
	je	.L19
.L11:
	cmpl	(%rdx), %r8d
	je	.L20
.L14:
	cmpl	(%rdi), %ecx
	je	.L21
.L6:
	ret
	.p2align 4,,10
	.p2align 3
.L21:
	movzwl	4(%rdi), %ecx
	cmpw	%cx, 4(%rsi)
	jne	.L6
	addl	$8, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L20:
	movzwl	4(%rdx), %edx
	cmpw	%dx, 4(%rdi)
	jne	.L14
	addl	$4, %eax
	jmp	.L14
	.p2align 4,,10
	.p2align 3
.L19:
	movzwl	4(%rdx), %r9d
	cmpw	%r9w, 4(%rsi)
	jne	.L11
	addl	$2, %eax
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L18:
	movzwl	4(%rsi), %eax
	cmpw	%ax, 4(%rdi)
	jne	.L7
	xorl	%eax, %eax
	jmp	.L8
	.cfi_endproc
.LFE16:
	.size	test_with_inline, .-test_with_inline
	.p2align 4
	.globl	test_without_inline
	.type	test_without_inline, @function
test_without_inline:
.LFB17:
	.cfi_startproc
	endbr64
	call	mac_equals_normal
	movq	%rsi, %r8
	movq	%rdi, %r9
	movq	%rdx, %rsi
	movq	%r8, %rdi
	movzbl	%al, %ecx
	call	mac_equals_normal
	movq	%r9, %rdi
	movl	%eax, %esi
	leal	2(%rcx), %eax
	testb	%sil, %sil
	movq	%rdx, %rsi
	cmovne	%eax, %ecx
	call	mac_equals_normal
	movq	%r9, %rsi
	movq	%r8, %rdi
	movl	%eax, %edx
	leal	4(%rcx), %eax
	testb	%dl, %dl
	cmovne	%eax, %ecx
	call	mac_equals_normal
	movl	%eax, %edx
	leal	8(%rcx), %eax
	testb	%dl, %dl
	cmovne	%eax, %ecx
	movl	%ecx, %eax
	ret
	.cfi_endproc
.LFE17:
	.size	test_without_inline, .-test_without_inline
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
