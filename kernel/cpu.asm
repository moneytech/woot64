[bits 64]

global cpuEnableInterrupts
cpuEnableInterrupts:
    sti
    ret

global cpuDisableInterrupts
cpuDisableInterrupts:
    pushfq  ; FIXME: There might be race condition here
    cli
    pop rax
    shr rax, 9
    and rax, 1
    ret

global cpuAreInterruptsEnabled
cpuAreInterruptsEnabled:
    pushfq
    pop rax
    shr rax, 9
    and rax, 1
    ret

global cpuRestoreInterrupts
cpuRestoreInterrupts:
    or rdi, rdi
    jz .disable
    sti
    ret
.disable:
    cli
    ret

global cpuGetCR0
cpuGetCR0:
    mov rax, cr0
    ret

global cpuSetCR0
cpuSetCR0:
    mov cr0, rdi
    ret

global cpuGetCR2
cpuGetCR2:
    mov rax, cr2
    ret

global cpuGetCR3
cpuGetCR3:
    mov rax, cr3
    ret

global cpuSetCR3
cpuSetCR3:
    mov cr3, rdi
    ret

global cpuInvalidatePage
cpuInvalidatePage:
    invlpg [rdi]
    ret

global _inb
_inb:
    mov rdx, rdi
    xor rax, rax
    in al, dx
    ret

global _inw
_inw:
    mov rdx, rdi
    xor rax, rax
    in ax, dx
    ret

global _inl
global _ind
_inl:
_ind:
    mov rdx, rdi
    xor rax, rax
    in eax, dx
    ret

global _outb
_outb:
    mov rdx, rdi
    mov rax, rsi
    out dx, al
    ret

global _outw
_outw:
    mov rdx, rdi
    mov rax, rsi
    out dx, ax
    ret

global _outl
global _outd
_outl:
_outd:
    mov rdx, rdi
    mov rax, rsi
    out dx, eax
    ret

global _insb
_insb:
    cld
    xchg rdx, rsi
    mov rcx, rsi
    rep insb
    ret

global _insw
_insw:
    cld
    xchg rdx, rsi
    mov rcx, rsi
    rep insw
    ret

global _insl
global _insd
_insl:
_insd:
    cld
    xchg rdx, rsi
    mov rcx, rsi
    rep insd
    ret

global _outsb
_outsb:
    cld
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    rep outsb
    ret

global _outsw
_outsw:
    cld
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    rep outsw
    ret

global _outsl
global _outsd
_outsl:
_outsd:
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    rep outsd
    ret

global cpuIOSetBitsB
cpuIOSetBitsB:
    mov rdx, rdi
    in al, dx
    or rax, rsi
    out dx, al
    ret

global cpuIOSetBitsW
cpuIOSetBitsW:
    mov rdx, rdi
    in ax, dx
    or rax, rsi
    out dx, ax
    ret

global cpuIOSetBitsL
global cpuIOSetBitsD
cpuIOSetBitsL:
cpuIOSetBitsD:
    mov rdx, rdi
    in eax, dx
    or rax, rsi
    out dx, eax
    ret

global cpuIOClrBitsB
cpuIOClrBitsB:
    mov rdx, rdi
    in al, dx
    not rsi
    and rax, rsi
    out dx, al
    ret

global cpuIOClrBitsW
cpuIOClrBitsW:
    mov rdx, rdi
    in ax, dx
    not rsi
    and rax, rsi
    out dx, ax
    ret

global cpuIOClrBitsL
global cpuIOClrBitsD
cpuIOClrBitsL:
cpuIOClrBitsD:
    mov rdx, rdi
    in eax, dx
    not rsi
    and rax, rsi
    out dx, eax
    ret
