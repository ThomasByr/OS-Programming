	.file	"ex1.c"
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
	cmpl	$1, %ebx
	je	.L7
.L3:
	movl	$1, %edi
	call	exit@PLT
.L7:
	leaq	.LC0(%rip), %rdi
	call	perror@PLT
	jmp	.L3
	.cfi_endproc
.LFE64:
	.size	panic, .-panic
	.section	.rodata.str1.1
.LC1:
	.string	"pthread_mutex_lock(mutex)"
.LC2:
	.string	"pthread_mutex_unlock(mutex)"
	.text
	.p2align 4
	.globl	thread_f
	.type	thread_f, @function
thread_f:
.LFB65:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movq	%rdi, %r12
	call	__errno_location@PLT
	movl	$1000000, %ebx
	movq	%rax, %rbp
	.p2align 4
	.p2align 3
.L11:
	movq	%r12, %rdi
	call	pthread_mutex_lock@PLT
	movl	%eax, 0(%rbp)
	testl	%eax, %eax
	jg	.L14
	movq	%r12, %rdi
	incq	count(%rip)
	call	pthread_mutex_unlock@PLT
	movl	%eax, 0(%rbp)
	testl	%eax, %eax
	jg	.L15
	decl	%ebx
	jne	.L11
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
.L14:
	.cfi_restore_state
	leaq	.LC1(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L15:
	leaq	.LC2(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
	.cfi_endproc
.LFE65:
	.size	thread_f, .-thread_f
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"pthread_mutex_init(&mutex, NULL)"
	.align 8
.LC4:
	.string	"pthread_create(&t[i], NULL, thread_f, &mutex)"
	.section	.rodata.str1.1
.LC5:
	.string	"pthread_join(t[i], NULL)"
.LC6:
	.string	"pthread_mutex_destroy(&mutex)"
.LC7:
	.string	"count = %ld\n"
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
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	subq	$80, %rsp
	.cfi_def_cfa_offset 128
	leaq	32(%rsp), %rbx
	movq	%fs:40, %rax
	movq	%rax, 72(%rsp)
	xorl	%eax, %eax
	movq	$0, count(%rip)
	call	__errno_location@PLT
	xorl	%esi, %esi
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	pthread_mutex_init@PLT
	movl	%eax, 0(%rbp)
	testl	%eax, %eax
	jg	.L27
	movq	%rsp, %r12
	leaq	thread_f(%rip), %r13
	movq	%r12, %r14
.L19:
	movq	%rbx, %rcx
	movq	%r13, %rdx
	xorl	%esi, %esi
	movq	%r14, %rdi
	call	pthread_create@PLT
	movl	%eax, 0(%rbp)
	testl	%eax, %eax
	jg	.L28
	addq	$8, %r14
	cmpq	%rbx, %r14
	jne	.L19
.L21:
	movq	(%r12), %rdi
	xorl	%esi, %esi
	call	pthread_join@PLT
	movl	%eax, 0(%rbp)
	testl	%eax, %eax
	jg	.L29
	addq	$8, %r12
	cmpq	%rbx, %r12
	jne	.L21
	movq	%r12, %rdi
	call	pthread_mutex_destroy@PLT
	movl	%eax, 0(%rbp)
	testl	%eax, %eax
	jg	.L30
	movq	count(%rip), %rdx
	xorl	%eax, %eax
	leaq	.LC7(%rip), %rsi
	movl	$1, %edi
	call	__printf_chk@PLT
	movq	72(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L31
	addq	$80, %rsp
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
.L28:
	.cfi_restore_state
	leaq	.LC4(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L29:
	leaq	.LC5(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L30:
	leaq	.LC6(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L27:
	leaq	.LC3(%rip), %rsi
	movl	$1, %edi
	xorl	%eax, %eax
	call	panic
.L31:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE66:
	.size	main, .-main
	.globl	count
	.bss
	.align 8
	.type	count, @object
	.size	count, 8
count:
	.zero	8
	.ident	"GCC: (Ubuntu 11.2.0-7ubuntu2) 11.2.0"
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
