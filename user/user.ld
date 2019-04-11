ENTRY(_start)
PHDRS
{
	text PT_LOAD FILEHDR PHDRS;
	data PT_LOAD;
	dynamic PT_DYNAMIC;
}
SECTIONS
{
    PAGE_SIZE = 4096;
    . = 0x0000000008000000;
    .headers : { *(.headers*) } : text
    __headers = ADDR(.headers);
    . += SIZEOF_HEADERS;
    . = ALIGN(PAGE_SIZE);
    .text : { *(.text*) } : text
    .rodata : { *(.rodata*) } : text
    . = ALIGN(PAGE_SIZE);
    .data : { *(.data*) } : data

    __init_array_start = .;
    .init_array : { *(.init_array*) }
    __init_array_end = .;

    __fini_array_start = .;
    .fini_array : { *(.fini_array*) }
    __fini_array_end = .;

    .dynamic : { *(.dynamic*) } : data : dynamic
    _DYNAMIC = ADDR(.dynamic);
    .bss : { *(.bss*) *(COMMON) } : data
    end = .;
}
