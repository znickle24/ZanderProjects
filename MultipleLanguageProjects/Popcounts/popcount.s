	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_popcount_1_data        ## -- Begin function popcount_1_data
	.p2align	4, 0x90
_popcount_1_data:                       ## @popcount_1_data
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi0:
	.cfi_def_cfa_offset 16
Lcfi1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi2:
	.cfi_def_cfa_register %rbp
	movl	$64, %ecx
	xorl	%eax, %eax
	.p2align	4, 0x90
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	%edi, %edx
	andl	$1, %edx
	movl	%eax, %eax
	addq	%rdx, %rax
	shrq	%rdi
	decl	%ecx
	jne	LBB0_1
## BB#2:
                                        ## kill: %EAX<def> %EAX<kill> %RAX<kill>
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount_1_control     ## -- Begin function popcount_1_control
	.p2align	4, 0x90
_popcount_1_control:                    ## @popcount_1_control
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi3:
	.cfi_def_cfa_offset 16
Lcfi4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi5:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$64, %ecx
	.p2align	4, 0x90
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	%edi, %edx
	andl	$1, %edx
	addl	%edx, %eax
	shrq	%rdi
	decl	%ecx
	jne	LBB1_1
## BB#2:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount_4_data        ## -- Begin function popcount_4_data
	.p2align	4, 0x90
_popcount_4_data:                       ## @popcount_4_data
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi6:
	.cfi_def_cfa_offset 16
Lcfi7:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi8:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$16, %ecx
	leaq	_pop4(%rip), %rdx
	.p2align	4, 0x90
LBB2_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	%edi, %esi
	andl	$15, %esi
	addl	(%rdx,%rsi,4), %eax
	shrq	$4, %rdi
	decl	%ecx
	jne	LBB2_1
## BB#2:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount_4_control     ## -- Begin function popcount_4_control
	.p2align	4, 0x90
_popcount_4_control:                    ## @popcount_4_control
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi9:
	.cfi_def_cfa_offset 16
Lcfi10:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi11:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$-1, %ecx
	leaq	LJTI3_0(%rip), %rsi
	.p2align	4, 0x90
LBB3_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	%edi, %edx
	andb	$15, %dl
	decb	%dl
	cmpb	$14, %dl
	ja	LBB3_14
## BB#2:                                ##   in Loop: Header=BB3_1 Depth=1
	movzbl	%dl, %edx
	movslq	(%rsi,%rdx,4), %rdx
	addq	%rsi, %rdx
	jmpq	*%rdx
LBB3_7:                                 ##   in Loop: Header=BB3_1 Depth=1
	addl	$2, %eax
	jmp	LBB3_14
	.p2align	4, 0x90
LBB3_3:                                 ##   in Loop: Header=BB3_1 Depth=1
	incl	%eax
	jmp	LBB3_14
	.p2align	4, 0x90
LBB3_13:                                ##   in Loop: Header=BB3_1 Depth=1
	addl	$3, %eax
	jmp	LBB3_14
LBB3_9:                                 ##   in Loop: Header=BB3_1 Depth=1
	addl	$4, %eax
	.p2align	4, 0x90
LBB3_14:                                ##   in Loop: Header=BB3_1 Depth=1
	shrq	$4, %rdi
	incl	%ecx
	cmpl	$15, %ecx
	jb	LBB3_1
## BB#15:
	popq	%rbp
	retq
	.cfi_endproc
	.p2align	2, 0x90
	.data_region jt32
L3_0_set_3 = LBB3_3-LJTI3_0
L3_0_set_7 = LBB3_7-LJTI3_0
L3_0_set_13 = LBB3_13-LJTI3_0
L3_0_set_9 = LBB3_9-LJTI3_0
LJTI3_0:
	.long	L3_0_set_3
	.long	L3_0_set_3
	.long	L3_0_set_7
	.long	L3_0_set_3
	.long	L3_0_set_7
	.long	L3_0_set_7
	.long	L3_0_set_13
	.long	L3_0_set_3
	.long	L3_0_set_7
	.long	L3_0_set_7
	.long	L3_0_set_13
	.long	L3_0_set_7
	.long	L3_0_set_13
	.long	L3_0_set_13
	.long	L3_0_set_9
	.end_data_region
                                        ## -- End function
	.globl	_popcount_8_data        ## -- Begin function popcount_8_data
	.p2align	4, 0x90
_popcount_8_data:                       ## @popcount_8_data
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi12:
	.cfi_def_cfa_offset 16
Lcfi13:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi14:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$8, %ecx
	leaq	_pop8(%rip), %rdx
	.p2align	4, 0x90
LBB4_1:                                 ## =>This Inner Loop Header: Depth=1
	movzbl	%dil, %esi
	addl	(%rdx,%rsi,4), %eax
	shrq	$8, %rdi
	decl	%ecx
	jne	LBB4_1
## BB#2:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount_16_data       ## -- Begin function popcount_16_data
	.p2align	4, 0x90
_popcount_16_data:                      ## @popcount_16_data
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi15:
	.cfi_def_cfa_offset 16
Lcfi16:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi17:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$4, %ecx
	leaq	_pop16(%rip), %rdx
	.p2align	4, 0x90
LBB5_1:                                 ## =>This Inner Loop Header: Depth=1
	movzwl	%di, %esi
	addl	(%rdx,%rsi,4), %eax
	shrq	$16, %rdi
	decl	%ecx
	jne	LBB5_1
## BB#2:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount_kernighan     ## -- Begin function popcount_kernighan
	.p2align	4, 0x90
_popcount_kernighan:                    ## @popcount_kernighan
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi18:
	.cfi_def_cfa_offset 16
Lcfi19:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi20:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	testq	%rdi, %rdi
	je	LBB6_2
	.p2align	4, 0x90
LBB6_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	%edi, %ecx
	andl	$1, %ecx
	movl	%eax, %eax
	addq	%rcx, %rax
	shrq	%rdi
	jne	LBB6_1
LBB6_2:
                                        ## kill: %EAX<def> %EAX<kill> %RAX<kill>
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount64a            ## -- Begin function popcount64a
	.p2align	4, 0x90
_popcount64a:                           ## @popcount64a
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi21:
	.cfi_def_cfa_offset 16
Lcfi22:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi23:
	.cfi_def_cfa_register %rbp
	movabsq	$6148914691236517205, %rax ## imm = 0x5555555555555555
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	%rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movabsq	$3689348814741910323, %rax ## imm = 0x3333333333333333
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$2, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movabsq	$506381209866536711, %rax ## imm = 0x707070707070707
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$4, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movabsq	$4222189076152335, %rax ## imm = 0xF000F000F000F
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$8, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movabsq	$133143986207, %rax     ## imm = 0x1F0000001F
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$16, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movq	%rdi, %rax
	shrq	$32, %rax
	addl	%eax, %edi
	movl	%edi, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount64b            ## -- Begin function popcount64b
	.p2align	4, 0x90
_popcount64b:                           ## @popcount64b
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi24:
	.cfi_def_cfa_offset 16
Lcfi25:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi26:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rax
	shrq	%rax
	movabsq	$6148914691236517205, %rcx ## imm = 0x5555555555555555
	andq	%rax, %rcx
	subq	%rcx, %rdi
	movabsq	$3689348814741910323, %rax ## imm = 0x3333333333333333
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$2, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movq	%rdi, %rax
	shrq	$4, %rax
	addq	%rdi, %rax
	movabsq	$1085102592571150095, %rcx ## imm = 0xF0F0F0F0F0F0F0F
	andq	%rax, %rcx
	movq	%rcx, %rax
	shrq	$8, %rax
	addq	%rcx, %rax
	movq	%rax, %rcx
	shrq	$16, %rcx
	addq	%rax, %rcx
	movq	%rcx, %rax
	shrq	$32, %rax
	addl	%ecx, %eax
	andl	$127, %eax
                                        ## kill: %EAX<def> %EAX<kill> %RAX<kill>
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount64c            ## -- Begin function popcount64c
	.p2align	4, 0x90
_popcount64c:                           ## @popcount64c
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi27:
	.cfi_def_cfa_offset 16
Lcfi28:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi29:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rax
	shrq	%rax
	movabsq	$6148914691236517205, %rcx ## imm = 0x5555555555555555
	andq	%rax, %rcx
	subq	%rcx, %rdi
	movabsq	$3689348814741910323, %rax ## imm = 0x3333333333333333
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$2, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movq	%rdi, %rax
	shrq	$4, %rax
	leaq	(%rax,%rdi), %rax
	movabsq	$1085102592571150095, %rcx ## imm = 0xF0F0F0F0F0F0F0F
	andq	%rax, %rcx
	movabsq	$72340172838076673, %rax ## imm = 0x101010101010101
	imulq	%rcx, %rax
	shrq	$56, %rax
                                        ## kill: %EAX<def> %EAX<kill> %RAX<kill>
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_popcount64_fast        ## -- Begin function popcount64_fast
	.p2align	4, 0x90
_popcount64_fast:                       ## @popcount64_fast
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi30:
	.cfi_def_cfa_offset 16
Lcfi31:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi32:
	.cfi_def_cfa_register %rbp
	movq	%rdi, %rax
	shrq	%rax
	movabsq	$6148914691236517205, %rcx ## imm = 0x5555555555555555
	andq	%rax, %rcx
	subq	%rcx, %rdi
	movabsq	$3689348814741910323, %rax ## imm = 0x3333333333333333
	movq	%rdi, %rcx
	andq	%rax, %rcx
	shrq	$2, %rdi
	andq	%rax, %rdi
	addq	%rcx, %rdi
	movq	%rdi, %rax
	shrq	$4, %rax
	leaq	(%rax,%rdi), %rax
	movabsq	$1085102592571150095, %rcx ## imm = 0xF0F0F0F0F0F0F0F
	andq	%rax, %rcx
	movabsq	$72340172838076673, %rax ## imm = 0x101010101010101
	imulq	%rcx, %rax
	shrq	$56, %rax
                                        ## kill: %EAX<def> %EAX<kill> %RAX<kill>
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi33:
	.cfi_def_cfa_offset 16
Lcfi34:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi35:
	.cfi_def_cfa_register %rbp
	callq	_init
	callq	_test_popcounts
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90         ## -- Begin function init
_init:                                  ## @init
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi36:
	.cfi_def_cfa_offset 16
Lcfi37:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi38:
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	leaq	_pop8(%rip), %r8
	leaq	_pop16(%rip), %rdx
	xorl	%esi, %esi
	.p2align	4, 0x90
LBB12_1:                                ## =>This Inner Loop Header: Depth=1
	cmpq	$255, %rsi
	ja	LBB12_3
## BB#2:                                ##   in Loop: Header=BB12_1 Depth=1
	movl	%esi, %edi
	shrl	%edi
	andl	$1431655765, %edi       ## imm = 0x55555555
	movl	%esi, %ecx
	subl	%edi, %ecx
	movl	%ecx, %edi
	andl	$858993459, %edi        ## imm = 0x33333333
	shrl	$2, %ecx
	andl	$858993459, %ecx        ## imm = 0x33333333
	addl	%edi, %ecx
	movl	%ecx, %edi
	shrl	$4, %edi
	addl	%ecx, %edi
	andl	$252645135, %edi        ## imm = 0xF0F0F0F
	imull	$16843009, %edi, %ecx   ## imm = 0x1010101
	shrl	$24, %ecx
	movl	%ecx, (%rax,%r8)
LBB12_3:                                ##   in Loop: Header=BB12_1 Depth=1
	movl	%esi, %ecx
	shrl	%ecx
	andl	$1431655765, %ecx       ## imm = 0x55555555
	movl	%esi, %edi
	subl	%ecx, %edi
	movl	%edi, %ecx
	andl	$858993459, %ecx        ## imm = 0x33333333
	shrl	$2, %edi
	andl	$858993459, %edi        ## imm = 0x33333333
	addl	%ecx, %edi
	movl	%edi, %ecx
	shrl	$4, %ecx
	addl	%edi, %ecx
	andl	$252645135, %ecx        ## imm = 0xF0F0F0F
	imull	$16843009, %ecx, %ecx   ## imm = 0x1010101
	shrl	$24, %ecx
	movl	%ecx, (%rax,%rdx)
	incq	%rsi
	addq	$4, %rax
	cmpq	$65536, %rsi            ## imm = 0x10000
	jne	LBB12_1
## BB#4:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ## -- Begin function test_popcounts
LCPI13_0:
	.quad	4666723172467343360     ## double 1.0E+4
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4, 0x90
_test_popcounts:                        ## @test_popcounts
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi39:
	.cfi_def_cfa_offset 16
Lcfi40:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi41:
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$80056, %rsp            ## imm = 0x138B8
Lcfi42:
	.cfi_offset %rbx, -56
Lcfi43:
	.cfi_offset %r12, -48
Lcfi44:
	.cfi_offset %r13, -40
Lcfi45:
	.cfi_offset %r14, -32
Lcfi46:
	.cfi_offset %r15, -24
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -48(%rbp)
	movq	$0, -80048(%rbp)
	movq	$-1, -80040(%rbp)
	movl	$2, %ebx
	.p2align	4, 0x90
LBB13_1:                                ## =>This Inner Loop Header: Depth=1
	callq	_rand64
	movq	%rax, -80048(%rbp,%rbx,8)
	incq	%rbx
	cmpq	$10000, %rbx            ## imm = 0x2710
	jne	LBB13_1
## BB#2:
	leaq	_popcounts(%rip), %r14
	xorl	%ebx, %ebx
	leaq	-80088(%rbp), %r13
                                        ## implicit-def: %R12D
	.p2align	4, 0x90
LBB13_3:                                ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB13_4 Depth 2
	movl	%r12d, -80052(%rbp)     ## 4-byte Spill
	movl	$6, %edi
	movq	%r13, %r15
	movq	%r13, %rsi
	callq	_clock_gettime
	xorl	%r13d, %r13d
	xorl	%r12d, %r12d
	.p2align	4, 0x90
LBB13_4:                                ##   Parent Loop BB13_3 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	movq	-80048(%rbp,%r13,8), %rdi
	callq	*(%r14)
	addl	%eax, %r12d
	incq	%r13
	cmpq	$10000, %r13            ## imm = 0x2710
	jne	LBB13_4
## BB#5:                                ##   in Loop: Header=BB13_3 Depth=1
	movl	$6, %edi
	leaq	-80072(%rbp), %r14
	movq	%r14, %rsi
	callq	_clock_gettime
	movq	%r14, %rdi
	movq	%r15, %r13
	movq	%r13, %rsi
	callq	_timespecDiff
	divsd	LCPI13_0(%rip), %xmm0
	movb	$1, %al
	leaq	L_.str(%rip), %rdi
	callq	_printf
	testq	%rbx, %rbx
	je	LBB13_7
## BB#6:                                ##   in Loop: Header=BB13_3 Depth=1
	cmpl	-80052(%rbp), %r12d     ## 4-byte Folded Reload
	jne	LBB13_10
LBB13_7:                                ##   in Loop: Header=BB13_3 Depth=1
	leaq	_popcounts(%rip), %rax
	leaq	8(%rax,%rbx,8), %r14
	incq	%rbx
	cmpq	$11, %rbx
	jne	LBB13_3
## BB#8:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-48(%rbp), %rax
	jne	LBB13_9
## BB#11:
	addq	$80056, %rsp            ## imm = 0x138B8
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
LBB13_9:
	callq	___stack_chk_fail
LBB13_10:
	leaq	L___func__.test_popcounts(%rip), %rdi
	leaq	L_.str.1(%rip), %rsi
	leaq	L_.str.2(%rip), %rcx
	movl	$236, %edx
	callq	___assert_rtn
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90         ## -- Begin function rand64
_rand64:                                ## @rand64
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi47:
	.cfi_def_cfa_offset 16
Lcfi48:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi49:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
Lcfi50:
	.cfi_offset %rbx, -24
	leaq	L_.str.3(%rip), %rdi
	xorl	%esi, %esi
	xorl	%eax, %eax
	callq	_open
	movl	%eax, %ebx
	testl	%ebx, %ebx
	js	LBB14_4
## BB#1:
	leaq	-16(%rbp), %rsi
	movl	$8, %edx
	movl	%ebx, %edi
	callq	_read
	cmpq	$8, %rax
	jne	LBB14_4
## BB#2:
	movl	%ebx, %edi
	callq	_close
	testl	%eax, %eax
	jne	LBB14_4
## BB#3:
	movq	-16(%rbp), %rax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
LBB14_4:
	callq	_abort
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ## -- Begin function timespecDiff
LCPI15_0:
	.quad	4741671816366391296     ## double 1.0E+9
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4, 0x90
_timespecDiff:                          ## @timespecDiff
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi51:
	.cfi_def_cfa_offset 16
Lcfi52:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi53:
	.cfi_def_cfa_register %rbp
	cvtsi2sdq	(%rdi), %xmm1
	movsd	LCPI15_0(%rip), %xmm2   ## xmm2 = mem[0],zero
	mulsd	%xmm2, %xmm1
	cvtsi2sdq	8(%rdi), %xmm0
	addsd	%xmm1, %xmm0
	xorps	%xmm1, %xmm1
	cvtsi2sdq	(%rsi), %xmm1
	mulsd	%xmm2, %xmm1
	xorps	%xmm2, %xmm2
	cvtsi2sdq	8(%rsi), %xmm2
	addsd	%xmm1, %xmm2
	subsd	%xmm2, %xmm0
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__const
	.p2align	4               ## @pop4
_pop4:
	.long	0                       ## 0x0
	.long	1                       ## 0x1
	.long	1                       ## 0x1
	.long	2                       ## 0x2
	.long	1                       ## 0x1
	.long	2                       ## 0x2
	.long	2                       ## 0x2
	.long	3                       ## 0x3
	.long	1                       ## 0x1
	.long	2                       ## 0x2
	.long	2                       ## 0x2
	.long	3                       ## 0x3
	.long	2                       ## 0x2
	.long	3                       ## 0x3
	.long	3                       ## 0x3
	.long	4                       ## 0x4

.zerofill __DATA,__bss,_pop8,1024,4     ## @pop8
.zerofill __DATA,__bss,_pop16,262144,4  ## @pop16
	.section	__DATA,__const
	.p2align	4               ## @popcounts
_popcounts:
	.quad	_popcount_1_data
	.quad	_popcount_1_control
	.quad	_popcount_4_data
	.quad	_popcount_4_control
	.quad	_popcount_8_data
	.quad	_popcount_16_data
	.quad	_popcount_kernighan
	.quad	_popcount64a
	.quad	_popcount64b
	.quad	_popcount64c
	.quad	_popcount64_fast
	.quad	0

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%lf ns / op\n"

L___func__.test_popcounts:              ## @__func__.test_popcounts
	.asciz	"test_popcounts"

L_.str.1:                               ## @.str.1
	.asciz	"popcount.c"

L_.str.2:                               ## @.str.2
	.asciz	"sum == last_sum"

L_.str.3:                               ## @.str.3
	.asciz	"/dev/urandom"


.subsections_via_symbols
