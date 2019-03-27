[bits 64]

segment .text

extern kmain
global _start64
_start64:
    lea rsp, [rel kernelStack.end]
    xor rbp, rbp
    mov rdi, rbx
    call kmain
    cli
    hlt

segment .bss
align 4096
kernelStack:
    resb 64 << 10
.end:
