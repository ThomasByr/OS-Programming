	.file	"main.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	""
	.text
	.p2align 4
	.globl	panic
	.type	panic, @function
panic:
.LFB64:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rsi, %r10
	movl	%edi, %ebx
	subq	$208, %rsp
	.cfi_def_cfa_offset 224
	movq	%rdx, 48(%rsp)
	movq	%rcx, 56(%rsp)
	movq	%r8, 64(%rsp)
	movq	%r9, 72(%rsp)
	testb	%al, %al
	je	.L2
	vmovaps	%xmm0, 80(%rsp)
	vmovaps	%xmm1, 96(%rsp)
	vmovaps	%xmm2, 112(%rsp)
	vmovaps	%xmm3, 128(%rsp)
	vmovaps	%xmm4, 144(%rsp)
	vmovaps	%xmm5, 160(%rsp)
	vmovaps	%xmm6, 176(%rsp)
	vmovaps	%xmm7, 192(%rsp)
.L2:
	movl	$1, %esi
	movq	%rsp, %rcx
	movq	%r10, %rdx
	movq	stderr(%rip), %rdi
	movq	%fs:40, %rax
	movq	%rax, 24(%rsp)
	xorl	%eax, %eax
	leaq	224(%rsp), %rax
	movl	$16, (%rsp)
	movl	$48, 4(%rsp)
	movq	%rax, 8(%rsp)
	leaq	32(%rsp), %rax
	movq	%rax, 16(%rsp)
	call	__vfprintf_chk@PLT
	movl	$10, %edi
	movq	stderr(%rip), %rsi
	call	fputc@PLT
	testl	%ebx, %ebx
	jne	.L10
.L3:
	movl	$1, %edi
	call	exit@PLT
.L10:
	leaq	.LC0(%rip), %rdi
	call	perror@PLT
	jmp	.L3
	.cfi_endproc
.LFE64:
	.size	panic, .-panic
	.section	.rodata.str1.1
.LC1:
	.string	"r"
.LC2:
	.string	"fp = fopen(a->fichier, \"r\")"
.LC3:
	.string	"sem_wait(a->sem_r)"
.LC4:
	.string	"sem_post(a->sem_w)"
.LC5:
	.string	"fclose %s"
	.text
	.p2align 4
	.globl	f
	.type	f, @function
f:
.LFB65:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	leaq	.LC1(%rip), %rsi
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rdi, %rbp
	subq	$1008, %rsp
	.cfi_def_cfa_offset 1056
	movq	(%rdi), %rdi
	movq	%fs:40, %rax
	movq	%rax, 1000(%rsp)
	xorl	%eax, %eax
	call	fopen@PLT
	testq	%rax, %rax
	je	.L21
	movq	%rax, %r13
	movq	%rsp, %r12
	leaq	8(%rbp), %r14
	jmp	.L12
	.p2align 4
	.p2align 3
.L14:
	call	__errno_location@PLT
	movq	1016(%rbp), %rdi
	movq	%rax, %rbx
	call	sem_wait@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L16
	movl	$1000, %edx
	movq	%r12, %rsi
	movq	%r14, %rdi
	call	__strcpy_chk@PLT
	movq	1024(%rbp), %rdi
	call	sem_post@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L17
.L12:
	movq	%r13, %rdx
	movl	$1000, %esi
	movq	%r12, %rdi
	call	fgets@PLT
	testq	%rax, %rax
	jne	.L14
	movq	%r13, %rdi
	call	fclose@PLT
	cmpl	$-1, %eax
	je	.L23
	call	__errno_location@PLT
	movq	1016(%rbp), %rdi
	movq	%rax, %rbx
	call	sem_wait@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L16
	movq	1008(%rbp), %rax
	movb	$0, 8(%rbp)
	movq	1024(%rbp), %rdi
	movl	$1, (%rax)
	call	sem_post@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L17
	movq	1000(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L24
	addq	$1008, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L16:
	.cfi_restore_state
	leaq	.LC3(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L17:
	leaq	.LC4(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L23:
	movq	0(%rbp), %rdx
	leaq	.LC5(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L21:
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L24:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE65:
	.size	f, .-f
	.section	.rodata.str1.1
.LC6:
	.string	"sem_init(&sem_r, 0, 1)"
.LC7:
	.string	"sem_init(&sem_w, 0, 0)"
.LC8:
	.string	"usage: %s <file>"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC9:
	.string	"pthread_create(&id, NULL, f, &arg)"
	.section	.rodata.str1.1
.LC10:
	.string	"sem_wait(&sem_w)"
.LC11:
	.string	"%s"
.LC12:
	.string	"sem_post(&sem_r)"
.LC13:
	.string	"pthread_join(id, NULL)"
.LC14:
	.string	"sem_destroy(&sem_r)"
.LC15:
	.string	"sem_destroy(&sem_w)"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB66:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movl	%edi, %r13d
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	movq	%rsi, %r12
	subq	$1136, %rsp
	.cfi_def_cfa_offset 1184
	movq	%fs:40, %rax
	movq	%rax, 1128(%rsp)
	xorl	%eax, %eax
	leaq	20(%rsp), %rax
	leaq	32(%rsp), %rbp
	movl	$0, 20(%rsp)
	vmovq	%rax, %xmm1
	vpinsrq	$1, %rbp, %xmm1, %xmm0
	vmovdqa	%xmm0, (%rsp)
	call	__errno_location@PLT
	movl	$1, %edx
	xorl	%esi, %esi
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	sem_init@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jle	.L26
	leaq	.LC6(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L26:
	leaq	64(%rsp), %r14
	xorl	%edx, %edx
	xorl	%esi, %esi
	movq	%r14, %rdi
	call	sem_init@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L39
	cmpl	$2, %r13d
	je	.L28
	movq	(%r12), %rdx
	leaq	.LC8(%rip), %rsi
	xorl	%edi, %edi
	xorl	%eax, %eax
	call	panic
.L28:
	vmovdqa	(%rsp), %xmm2
	movq	8(%r12), %rax
	leaq	96(%rsp), %rcx
	leaq	24(%rsp), %rdi
	leaq	f(%rip), %rdx
	xorl	%esi, %esi
	movq	%r14, 1120(%rsp)
	leaq	.LC11(%rip), %r12
	movq	%rax, 96(%rsp)
	vmovdqa	%xmm2, 1104(%rsp)
	call	pthread_create@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jle	.L29
	leaq	.LC9(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L31:
	movq	%r14, %rdi
	call	sem_wait@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L40
	leaq	104(%rsp), %rdx
	movq	%r12, %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk@PLT
	movq	stdout(%rip), %rdi
	call	fflush@PLT
	movq	%rbp, %rdi
	call	sem_post@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jg	.L41
.L29:
	cmpl	$0, 20(%rsp)
	je	.L31
	movq	24(%rsp), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jle	.L32
	leaq	.LC13(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L39:
	leaq	.LC7(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L40:
	leaq	.LC10(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L41:
	leaq	.LC12(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L32:
	movq	%rbp, %rdi
	call	sem_destroy@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jle	.L33
	leaq	.LC14(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L33:
	movq	%r14, %rdi
	call	sem_destroy@PLT
	movl	%eax, (%rbx)
	testl	%eax, %eax
	jle	.L34
	leaq	.LC15(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L34:
	xorl	%edi, %edi
	call	exit@PLT
	.cfi_endproc
.LFE66:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
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
