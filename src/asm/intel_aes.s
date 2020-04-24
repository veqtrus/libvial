/*
Copyright (c) 2020 Pavlos Georgiou

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

.globl vial_intel_aes_supported
.globl vial_intel_aes_encrypt
.globl vial_intel_aes_decrypt

vial_intel_aes_supported:
	pushq	%rbx
	movl	$1, %eax
	cpuid
	popq	%rbx
	shrl	$25, %ecx
	andl	$1, %ecx
	movl	%ecx, %eax
	ret

vial_intel_aes_encrypt: # blk (rdi), keys (rsi), rounds (rdx)
	movdqu	(%rdi), %xmm1
	shll	$4, %edx
	addq	%rsi, %rdx
	movdqu	(%rsi), %xmm2
	addq	$16, %rsi
	pxor	%xmm2, %xmm1
encrypt_loop:
	movdqu	(%rsi), %xmm2
	addq	$16, %rsi
	aesenc	%xmm2, %xmm1
	cmpq	%rsi, %rdx
	jne	encrypt_loop
	movdqu	(%rsi), %xmm2
	aesenclast	%xmm2, %xmm1
	movdqu	%xmm1, (%rdi)
	ret

vial_intel_aes_decrypt: # blk (rdi), keys (rsi), rounds (rdx)
	movdqu	(%rdi), %xmm1
	shll	$4, %edx
	addq	%rsi, %rdx
	movdqu	(%rdx), %xmm2
	subq	$16, %rdx
	pxor	%xmm2, %xmm1
decrypt_loop:
	movdqu	(%rdx), %xmm2
	subq	$16, %rdx
	aesimc	%xmm2, %xmm2
	aesdec	%xmm2, %xmm1
	cmpq	%rsi, %rdx
	jne	decrypt_loop
	movdqu	(%rdx), %xmm2
	aesdeclast	%xmm2, %xmm1
	movdqu	%xmm1, (%rdi)
	ret
