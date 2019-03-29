[bits 64]

%define PAGE_SIZE 4096

segment .text

extern kmain
extern _init
extern _fini
global _start64
_start64:
    lea rsp, [rel kernelStack.end]
    xor rbp, rbp
    mov rdi, rbx
    push rdi
    call _init
    pop rdi
    call kmain
    push rax
    call _fini
    pop rax
    cli
    hlt

segment .bss
align PAGE_SIZE
kernelStack:
    resb 64 << 10
.end:
