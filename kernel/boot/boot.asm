[bits 32]

%define KERNEL_BASE 0xFFFF800000000000
%define PAGE_SIZE 4096

%define ISR_STUB_SIZE       16  ; this MUST match the size of ISR_ERRCODE and
                                ; ISR_NOERRCODE macro expansions in isrs.asm

%define MULTIBOOT_MAGIC 0x1BADB002
%define MULTIBOOT_FLAGS 0x00000007
%define VIDEO_USE_TEXT  1
%define VIDEO_WIDTH     80
%define VIDEO_HEIGHT    25
%define VIDEO_BPP       0

segment .text.boot
align 4
_multiboot_header:
    dd MULTIBOOT_MAGIC
    dd MULTIBOOT_FLAGS
    dd -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)
    dd 0, 0, 0, 0, 0
    dd VIDEO_USE_TEXT, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_BPP

    global _start
    extern _start64
_start:
    ; save multiboot info pointer
    mov [multiboot_info_ptr - KERNEL_BASE], ebx

    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; load CR3
    mov eax, pml4 - KERNEL_BASE
    mov cr3, eax

    ; enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; load new gdt
    lgdt [gdt_descr - KERNEL_BASE]

    jmp 0x0028:.tramp - KERNEL_BASE
[bits 64]
.tramp:
    ; load new GDT with 64 bit address
    lgdt [gdt_descr - KERNEL_BASE]

    ; setup dsta/stack segment registers
    mov ax, 0x0030
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

; set up TSS
.setup_tss:
    cld
    mov rax, KERNEL_BASE
    add rax, tss - KERNEL_BASE
    mov rdi, KERNEL_BASE
    add rdi, gdt - KERNEL_BASE + 0x48 + 2
    stosw
    shr rax, 16
    stosb
    inc rdi
    inc rdi
    mov [rdi], ah
    shr rax, 16
    inc rdi
    stosd
    mov ax, 0x0048
    ltr ax

; set up IDT
extern isr0
.setup_idt:
    cld
    mov rcx, 256
    mov rdi, KERNEL_BASE
    add rdi, idt - KERNEL_BASE
    mov rdx, KERNEL_BASE
    add rdx, isr0 - KERNEL_BASE
.next_idt_entry
    mov rax, rdx
    stosw           ; offset 15..0
    mov ax, 0x0028
    stosw           ; 32 bit kernel code selector
    mov al, 0
    stosb           ; zero
    mov al, 0x8E
    stosb           ; type and attributes
    shr rax, 16
    stosw           ; offset 31..16
    shr rax, 16
    stosd           ; offset 63..32
    shr rax, 32
    stosd           ; reserverd fields
    add rdx, ISR_STUB_SIZE
    loop .next_idt_entry
    lidt [idt_descr - KERNEL_BASE]

    ; jump to 64 bit entry point
    mov rbx, KERNEL_BASE
    add rbx, [multiboot_info_ptr - KERNEL_BASE]
    jmp [.jmpaddr - KERNEL_BASE]
.jmpaddr:
    dq _start64

segment .data

gdt_descr:
    dw gdt.end - gdt - 1
    dq gdt

    align 4096
pml4:
    dq pml4e - KERNEL_BASE + 3
    times 255 dq 0
    dq pml4e - KERNEL_BASE + 3
    times 255 dq 0

    align 4096
pml4e:
    dq pdpe - KERNEL_BASE + 3
    times 4096 - ($ - pml4e) db 0

    align 4096
pdpe:
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    dq (($ - pdpe) << 21) | 0x83
    times 4096 - ($ - pdpe) db 0

gdt:
    dq 0x0000000000000000   ; null 0x0000

    dq 0x00CF9A000000FFFF   ; kernel32 code 0x0008
    dq 0x00CF92000000FFFF   ; kernel32 data 0x0010
    dq 0x00CFFA000000FFFF   ; user32 code 0x0018
    dq 0x00CFF2000000FFFF   ; user32 data 0x0020

    dq 0x00AF9A000000FFFF   ; kernel64 code 0x0028
    dq 0x00AF92000000FFFF   ; kernel64 data 0x0030
    dq 0x00AFFA000000FFFF   ; user64 code 0x0038
    dq 0x00AFF2000000FFFF   ; user64 data 0x0040

    dq 0x0040890000000067   ; tss 0x0048
    dq 0x0000000000000000   ; tss high half
.end:

idt_descr:
    dw idt.end - idt;
    dq idt

segment .bss
multiboot_info_ptr:
    resd 1

align PAGE_SIZE
tss:
    resb PAGE_SIZE

align PAGE_SIZE
idt:
    resq 256 * 2
.end:
