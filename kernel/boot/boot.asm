[bits 32]

%define KERNEL_BASE         0xFFFF800000000000
%define PAGE_SIZE           4096

%define DEBUG_SERIAL_BASE   0x03F8

%define ISR_STUB_SIZE       16  ; this MUST match the size of ISR_ERRCODE and
                                ; ISR_NOERRCODE macro expansions in isrs.asm

%define MULTIBOOT_MAGIC     0x1BADB002
%define MULTIBOOT_FLAGS     0x00000007
%define VIDEO_USE_TEXT      1
%define VIDEO_WIDTH         80
%define VIDEO_HEIGHT        25
%define VIDEO_BPP           0

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

    ; initialize serial port as early as posible
.init_serial:
    cld
    mov esi, serialInitSequence - KERNEL_BASE
    mov ecx, (serialInitSequence.end - serialInitSequence) / 2
.next_serial:
    mov dx, DEBUG_SERIAL_BASE
    lodsb
    add dl, al
    outsb
    loop .next_serial

    ; initialize PD entries to identity map first 1 GiB
.init_pdp:
    mov edi, pdp - KERNEL_BASE
    mov ecx, 512
    xor edx, edx
.next_pde:
    mov eax, edx
    shl eax, 21
    or al, 0x83
    stosd
    xor eax, eax
    stosd
    inc edx
    loop .next_pde

    ; enable PAE
.enable_pae:
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; load CR3
.load_cr3:
    mov eax, pml4 - KERNEL_BASE
    mov cr3, eax

    ; enable long mode
.enable_long_mode:
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8 | 1 << 0
    wrmsr

    ; enable paging
.enable_paging:
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; load new gdt
.load_gdt:
    lgdt [gdt_descr - KERNEL_BASE]

.jump_to_64:
    jmp 0x0028:.tramp - KERNEL_BASE
[bits 64]
.tramp:
    ; load new GDT with 64 bit address
    lgdt [gdt_descr - KERNEL_BASE]

    ; setup data/stack segment registers
    mov ax, 0x0030  ; kernel data
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov ax, 0x003b  ; user data
    mov fs, ax
    mov gs, ax

; set up TSS
.setup_tss:
    cld
    mov rax, KERNEL_BASE
    add rax, mainTSS - KERNEL_BASE
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

serialInitSequence:
    db 1, 0x00  ; disable interrupts
    db 3, 0x80  ; enable DLAB
    db 0, 0x01  ; set baud rate
    db 1, 0x00  ;   to 115200
    db 3, 0x03  ; disable DLAB and set 8N1 mode
    db 2, 0xC7  ; set FIFO to 14 bytes
    db 4, 0x03  ; assert RTS and DTR
.end:

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
    dq pdp - KERNEL_BASE + 3
    times 4096 - ($ - pml4e) db 0

gdt:
    dq 0x0000000000000000   ; null 0x0000

    dq 0x00CF9A000000FFFF   ; kernel32 code 0x0008
    dq 0x00CF92000000FFFF   ; kernel32 data 0x0010
    dq 0x00CFFA000000FFFF   ; user32 code 0x0018
    dq 0x00CFF2000000FFFF   ; user32 data 0x0020

    dq 0x00AF9A000000FFFF   ; kernel64 code 0x0028
    dq 0x00AF92000000FFFF   ; kernel64 data 0x0030
    dq 0x00AFF2000000FFFF   ; user64 data 0x0038
    dq 0x00AFFA000000FFFF   ; user64 code 0x0040

    ; tss 0x0048
    dq 0x0040890000000000 | (mainTSS.end - mainTSS)
    dq 0x0000000000000000
.end:

idt_descr:
    dw idt.end - idt;
    dq idt

segment .bss
multiboot_info_ptr:
    resd 1

align PAGE_SIZE
pdp:
    resb PAGE_SIZE

global mainTSS
mainTSS:
    resb 0x68
.end:

idt:
    resq 256 * 2
.end:
