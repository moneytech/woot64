[bits 64]

%define PAGE_SIZE 4096

segment .text

extern kmain
extern _init
extern _fini
extern cpuInitFPU
extern cpuEnableSSE
global _start64
_start64:
    lea rsp, [rel mainKernelThreadStack]
    xor rbp, rbp
    mov rdi, rbx

    push rdi
    push rdi

; init FPU
.init_fpu:
    mov rdi, 0x37F
    call cpuInitFPU

; enable SSE
.enable_sse:
    call cpuEnableSSE

; call _init
    pop rdi
    push rdi
    call _init
    pop rdi
    pop rdi

; call kmain
    call kmain
    push rax

; call _fini
    push rax
    call _fini
    pop rax
    pop rax
    cli
    hlt

segment .bss
align PAGE_SIZE
global mainKernelThreadStack
mainKernelThreadStack:
    resb 64 << 10
global mainKernelThreadStackEnd
mainKernelThreadStackEnd:
