#pragma once

#define KERNEL_BASE         0xFFFF800000000000
#define KERNEL_HEAP_BASE    0xFFFFC00000000000
#define KERNEL_HEAP_END     0xFFFFD00000000000

#define DEFAULT_HEAP_ALIGNMENT  16
#define DEFAULT_STACK_SIZE      (64 << 10)
#define DEFAULT_USER_STACK_SIZE (128 << 10)

#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)
#define PAGE_MASK   (PAGE_SIZE - 1)

#define KERNEL_VERSION_MAJOR 0
#define KERNEL_VERSION_MINOR 3
#define KERNEL_VERSION_DESCR "64-bit"

#define SEG_KERNEL_CODE 0x0028
#define SEG_KERNEL_DATA 0x0030

#define INLINE_ASM_SYNTAX   ".intel_syntax noprefix\n"
#define NORMAL_ASM_SYNTAX   ".intel_syntax noprefix\n"
