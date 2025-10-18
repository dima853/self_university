	.file	"main.c"
	.text
	.p2align 4
	.globl	function_ifndef
	.type	function_ifndef, @function
function_ifndef:
.LFB0:
	.cfi_startproc
	endbr64
	movl	$42, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	function_ifndef, .-function_ifndef
	.p2align 4
	.globl	function_pragma
	.type	function_pragma, @function
function_pragma:
.LFB1:
	.cfi_startproc
	endbr64
	movl	$24, %eax
	ret
	.cfi_endproc
.LFE1:
	.size	function_pragma, .-function_pragma
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	endbr64
	movl	$66, %eax
	ret
	.cfi_endproc
.LFE2:
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
