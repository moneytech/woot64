#objdump: --syms --special-syms
#name: ARM Mapping Symbols Test 3
# This test is only valid on ELF based ports.
#notarget: *-*-pe *-*-wince

.*: +file format.*arm.*

SYMBOL TABLE:
00000000 l    d  .text	00000000 .text
00000000 l    d  .data	00000000 .data
00000000 l    d  .bss	00000000 .bss
00000000 l       .text	00000000 \$d
00000004 l       .text	00000000 \$a
00000000 l    d  .ARM.attributes	00000000 .ARM.attributes


