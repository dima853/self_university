	.file	"main.c"
	.text
	.globl	function_ifndef
	.type	function_ifndef, @function
function_ifndef:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$42, %eax
	popq	%rbp
	ret
	.size	function_ifndef, .-function_ifndef
	.globl	function_pragma
	.type	function_pragma, @function
function_pragma:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$24, %eax
	popq	%rbp
	ret
	.size	function_pragma, .-function_pragma
	.globl	main
	.type	main, @function
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rbx
	movl	$0, %eax
	call	function_ifndef
	movl	%eax, %ebx
	movl	$0, %eax
	call	function_pragma
	addl	%ebx, %eax
	movq	-8(%rbp), %rbx
	leave
	ret
	.size	main, .-main
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
