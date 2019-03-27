[bits 64]

%define PAGE_SIZE 4096

segment .text

extern preInitialize
extern kmain
global _start64
_start64:
    lea rsp, [rel kernelStack.end]
    xor rbp, rbp
    mov rdi, rbx
    push rdi
    call preInitialize
    pop rdi
    call kmain
    cli
    hlt

segment .bss
align PAGE_SIZE
kernelStack:
    resb 64 << 10
.end:
