#as:
#objdump: -dw -Mintel
#name: i386 AVX512BW insns (Intel disassembly)
#source: avx512bw.s

.*: +file format .*


Disassembly of section \.text:

00000000 <_start>:
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c f5[ 	]*vpabsb zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 1c f5[ 	]*vpabsb zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 1c f5[ 	]*vpabsb zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c 31[ 	]*vpabsb zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c b4 f4 c0 1d fe ff[ 	]*vpabsb zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c 72 7f[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c b2 00 20 00 00[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c 72 80[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c b2 c0 df ff ff[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d f5[ 	]*vpabsw zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 1d f5[ 	]*vpabsw zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 1d f5[ 	]*vpabsw zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d 31[ 	]*vpabsw zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d b4 f4 c0 1d fe ff[ 	]*vpabsw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d 72 7f[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d b2 00 20 00 00[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d 72 80[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d b2 c0 df ff ff[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b f4[ 	]*vpackssdw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 6b f4[ 	]*vpackssdw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 6b f4[ 	]*vpackssdw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b 31[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b b4 f4 c0 1d fe ff[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b 30[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[eax\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b 72 7f[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b b2 00 20 00 00[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b 72 80[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b b2 c0 df ff ff[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b 72 7f[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx\+0x1fc\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b b2 00 02 00 00[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx\+0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b 72 80[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx-0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b b2 fc fd ff ff[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx-0x204\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 f4[ 	]*vpacksswb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 63 f4[ 	]*vpacksswb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 63 f4[ 	]*vpacksswb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 31[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 b4 f4 c0 1d fe ff[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 72 7f[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 b2 00 20 00 00[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 72 80[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 b2 c0 df ff ff[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b f4[ 	]*vpackusdw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 2b f4[ 	]*vpackusdw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 2b f4[ 	]*vpackusdw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b 31[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b b4 f4 c0 1d fe ff[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b 30[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[eax\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b 72 7f[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b b2 00 20 00 00[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b 72 80[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b b2 c0 df ff ff[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b 72 7f[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx\+0x1fc\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b b2 00 02 00 00[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx\+0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b 72 80[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx-0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b b2 fc fd ff ff[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx-0x204\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 f4[ 	]*vpackuswb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 67 f4[ 	]*vpackuswb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 67 f4[ 	]*vpackuswb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 31[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 b4 f4 c0 1d fe ff[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 72 7f[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 b2 00 20 00 00[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 72 80[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 b2 c0 df ff ff[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc f4[ 	]*vpaddb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f fc f4[ 	]*vpaddb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf fc f4[ 	]*vpaddb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc 31[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc b4 f4 c0 1d fe ff[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc 72 7f[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc b2 00 20 00 00[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc 72 80[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc b2 c0 df ff ff[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec f4[ 	]*vpaddsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ec f4[ 	]*vpaddsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ec f4[ 	]*vpaddsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec 31[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec b4 f4 c0 1d fe ff[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec 72 7f[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec b2 00 20 00 00[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec 72 80[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec b2 c0 df ff ff[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed f4[ 	]*vpaddsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ed f4[ 	]*vpaddsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ed f4[ 	]*vpaddsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed 31[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed b4 f4 c0 1d fe ff[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed 72 7f[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed b2 00 20 00 00[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed 72 80[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed b2 c0 df ff ff[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc f4[ 	]*vpaddusb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f dc f4[ 	]*vpaddusb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf dc f4[ 	]*vpaddusb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc 31[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc b4 f4 c0 1d fe ff[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc 72 7f[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc b2 00 20 00 00[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc 72 80[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc b2 c0 df ff ff[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd f4[ 	]*vpaddusw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f dd f4[ 	]*vpaddusw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf dd f4[ 	]*vpaddusw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd 31[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd b4 f4 c0 1d fe ff[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd 72 7f[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd b2 00 20 00 00[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd 72 80[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd b2 c0 df ff ff[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd f4[ 	]*vpaddw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f fd f4[ 	]*vpaddw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf fd f4[ 	]*vpaddw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd 31[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd b4 f4 c0 1d fe ff[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd 72 7f[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd b2 00 20 00 00[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd 72 80[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd b2 c0 df ff ff[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f f4 ab[ 	]*vpalignr zmm6,zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 4f 0f f4 ab[ 	]*vpalignr zmm6\{k7\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 cf 0f f4 ab[ 	]*vpalignr zmm6\{k7\}\{z\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f f4 7b[ 	]*vpalignr zmm6,zmm5,zmm4,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f 31 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f b4 f4 c0 1d fe ff 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f 72 7f 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f b2 00 20 00 00 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f 72 80 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f b2 c0 df ff ff 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 f4[ 	]*vpavgb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e0 f4[ 	]*vpavgb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e0 f4[ 	]*vpavgb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 31[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 b4 f4 c0 1d fe ff[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 72 7f[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 b2 00 20 00 00[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 72 80[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 b2 c0 df ff ff[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 f4[ 	]*vpavgw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e3 f4[ 	]*vpavgw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e3 f4[ 	]*vpavgw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 31[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 b4 f4 c0 1d fe ff[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 72 7f[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 b2 00 20 00 00[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 72 80[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 b2 c0 df ff ff[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 f4[ 	]*vpblendmb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 66 f4[ 	]*vpblendmb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 66 f4[ 	]*vpblendmb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 31[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 b4 f4 c0 1d fe ff[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 72 7f[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 b2 00 20 00 00[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 72 80[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 b2 c0 df ff ff[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 f5[ 	]*vpbroadcastb zmm6\{k7\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 78 f5[ 	]*vpbroadcastb zmm6\{k7\}\{z\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 31[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 b4 f4 c0 1d fe ff[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 72 7f[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx\+0x7f\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 b2 80 00 00 00[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx\+0x80\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 72 80[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx-0x80\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 b2 7f ff ff ff[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx-0x81\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7a f0[ 	]*vpbroadcastb zmm6,eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 7a f0[ 	]*vpbroadcastb zmm6\{k7\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 7a f0[ 	]*vpbroadcastb zmm6\{k7\}\{z\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7a f5[ 	]*vpbroadcastb zmm6,ebp
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 f5[ 	]*vpbroadcastw zmm6\{k7\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 79 f5[ 	]*vpbroadcastw zmm6\{k7\}\{z\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 31[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 b4 f4 c0 1d fe ff[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 72 7f[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx\+0xfe\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 b2 00 01 00 00[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx\+0x100\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 72 80[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx-0x100\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 b2 fe fe ff ff[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx-0x102\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7b f0[ 	]*vpbroadcastw zmm6,eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 7b f0[ 	]*vpbroadcastw zmm6\{k7\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 7b f0[ 	]*vpbroadcastw zmm6\{k7\}\{z\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7b f5[ 	]*vpbroadcastw zmm6,ebp
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 ed[ 	]*vpcmpeqb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 74 ed[ 	]*vpcmpeqb k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 29[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 ac f4 c0 1d fe ff[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 6a 7f[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 aa 00 20 00 00[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 6a 80[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 aa c0 df ff ff[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 ed[ 	]*vpcmpeqw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 75 ed[ 	]*vpcmpeqw k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 29[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 ac f4 c0 1d fe ff[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 6a 7f[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 aa 00 20 00 00[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 6a 80[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 aa c0 df ff ff[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 ed[ 	]*vpcmpgtb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 64 ed[ 	]*vpcmpgtb k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 29[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 ac f4 c0 1d fe ff[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 6a 7f[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 aa 00 20 00 00[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 6a 80[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 aa c0 df ff ff[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 ed[ 	]*vpcmpgtw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 65 ed[ 	]*vpcmpgtw k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 29[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 ac f4 c0 1d fe ff[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 6a 7f[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 aa 00 20 00 00[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 6a 80[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 aa c0 df ff ff[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 f4[ 	]*vpblendmw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 66 f4[ 	]*vpblendmw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 66 f4[ 	]*vpblendmw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 31[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 b4 f4 c0 1d fe ff[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 72 7f[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 b2 00 20 00 00[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 72 80[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 b2 c0 df ff ff[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 f4[ 	]*vpmaddubsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 04 f4[ 	]*vpmaddubsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 04 f4[ 	]*vpmaddubsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 31[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 b4 f4 c0 1d fe ff[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 72 7f[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 b2 00 20 00 00[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 72 80[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 b2 c0 df ff ff[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 f4[ 	]*vpmaddwd zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f5 f4[ 	]*vpmaddwd zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f5 f4[ 	]*vpmaddwd zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 31[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 b4 f4 c0 1d fe ff[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 72 7f[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 b2 00 20 00 00[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 72 80[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 b2 c0 df ff ff[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c f4[ 	]*vpmaxsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 3c f4[ 	]*vpmaxsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 3c f4[ 	]*vpmaxsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c 31[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c b4 f4 c0 1d fe ff[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c 72 7f[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c b2 00 20 00 00[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c 72 80[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c b2 c0 df ff ff[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee f4[ 	]*vpmaxsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ee f4[ 	]*vpmaxsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ee f4[ 	]*vpmaxsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee 31[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee b4 f4 c0 1d fe ff[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee 72 7f[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee b2 00 20 00 00[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee 72 80[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee b2 c0 df ff ff[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de f4[ 	]*vpmaxub zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f de f4[ 	]*vpmaxub zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf de f4[ 	]*vpmaxub zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de 31[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de b4 f4 c0 1d fe ff[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de 72 7f[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de b2 00 20 00 00[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de 72 80[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de b2 c0 df ff ff[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e f4[ 	]*vpmaxuw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 3e f4[ 	]*vpmaxuw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 3e f4[ 	]*vpmaxuw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e 31[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e b4 f4 c0 1d fe ff[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e 72 7f[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e b2 00 20 00 00[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e 72 80[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e b2 c0 df ff ff[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 f4[ 	]*vpminsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 38 f4[ 	]*vpminsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 38 f4[ 	]*vpminsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 31[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 b4 f4 c0 1d fe ff[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 72 7f[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 b2 00 20 00 00[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 72 80[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 b2 c0 df ff ff[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea f4[ 	]*vpminsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ea f4[ 	]*vpminsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ea f4[ 	]*vpminsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea 31[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea b4 f4 c0 1d fe ff[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea 72 7f[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea b2 00 20 00 00[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea 72 80[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea b2 c0 df ff ff[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da f4[ 	]*vpminub zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f da f4[ 	]*vpminub zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf da f4[ 	]*vpminub zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da 31[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da b4 f4 c0 1d fe ff[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da 72 7f[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da b2 00 20 00 00[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da 72 80[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da b2 c0 df ff ff[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a f4[ 	]*vpminuw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 3a f4[ 	]*vpminuw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 3a f4[ 	]*vpminuw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a 31[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a b4 f4 c0 1d fe ff[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a 72 7f[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a b2 00 20 00 00[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a 72 80[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a b2 c0 df ff ff[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 f5[ 	]*vpmovsxbw zmm6\{k7\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 20 f5[ 	]*vpmovsxbw zmm6\{k7\}\{z\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 31[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 b4 f4 c0 1d fe ff[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 72 7f[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx\+0xfe0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 b2 00 10 00 00[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx\+0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 72 80[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 b2 e0 ef ff ff[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1020\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 f5[ 	]*vpmovzxbw zmm6\{k7\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 30 f5[ 	]*vpmovzxbw zmm6\{k7\}\{z\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 31[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 b4 f4 c0 1d fe ff[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 72 7f[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx\+0xfe0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 b2 00 10 00 00[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx\+0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 72 80[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 b2 e0 ef ff ff[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1020\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b f4[ 	]*vpmulhrsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 0b f4[ 	]*vpmulhrsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 0b f4[ 	]*vpmulhrsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b 31[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b b4 f4 c0 1d fe ff[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b 72 7f[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b b2 00 20 00 00[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b 72 80[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b b2 c0 df ff ff[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 f4[ 	]*vpmulhuw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e4 f4[ 	]*vpmulhuw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e4 f4[ 	]*vpmulhuw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 31[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 b4 f4 c0 1d fe ff[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 72 7f[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 b2 00 20 00 00[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 72 80[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 b2 c0 df ff ff[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 f4[ 	]*vpmulhw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e5 f4[ 	]*vpmulhw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e5 f4[ 	]*vpmulhw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 31[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 b4 f4 c0 1d fe ff[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 72 7f[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 b2 00 20 00 00[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 72 80[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 b2 c0 df ff ff[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 f4[ 	]*vpmullw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d5 f4[ 	]*vpmullw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d5 f4[ 	]*vpmullw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 31[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 b4 f4 c0 1d fe ff[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 72 7f[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 b2 00 20 00 00[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 72 80[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 b2 c0 df ff ff[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 f4[ 	]*vpsadbw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 31[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 b4 f4 c0 1d fe ff[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 72 7f[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 b2 00 20 00 00[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 72 80[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 b2 c0 df ff ff[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 f4[ 	]*vpshufb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 00 f4[ 	]*vpshufb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 00 f4[ 	]*vpshufb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 31[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 b4 f4 c0 1d fe ff[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 72 7f[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 b2 00 20 00 00[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 72 80[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 b2 c0 df ff ff[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 f5 ab[ 	]*vpshufhw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 4f 70 f5 ab[ 	]*vpshufhw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e cf 70 f5 ab[ 	]*vpshufhw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 f5 7b[ 	]*vpshufhw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 31 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 b4 f4 c0 1d fe ff 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 72 7f 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 b2 00 20 00 00 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 72 80 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 b2 c0 df ff ff 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 f5 ab[ 	]*vpshuflw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 4f 70 f5 ab[ 	]*vpshuflw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f cf 70 f5 ab[ 	]*vpshuflw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 f5 7b[ 	]*vpshuflw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 31 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 b4 f4 c0 1d fe ff 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 72 7f 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 b2 00 20 00 00 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 72 80 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 b2 c0 df ff ff 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 f4[ 	]*vpsllw zmm6\{k7\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f1 f4[ 	]*vpsllw zmm6\{k7\}\{z\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 31[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 b4 f4 c0 1d fe ff[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 72 7f[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x7f0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 b2 00 08 00 00[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 72 80[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 b2 f0 f7 ff ff[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x810\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 f4[ 	]*vpsraw zmm6\{k7\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e1 f4[ 	]*vpsraw zmm6\{k7\}\{z\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 31[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 b4 f4 c0 1d fe ff[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 72 7f[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x7f0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 b2 00 08 00 00[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 72 80[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 b2 f0 f7 ff ff[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x810\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 f4[ 	]*vpsrlw zmm6\{k7\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d1 f4[ 	]*vpsrlw zmm6\{k7\}\{z\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 31[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 b4 f4 c0 1d fe ff[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 72 7f[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x7f0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 b2 00 08 00 00[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 72 80[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 b2 f0 f7 ff ff[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x810\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 dd ab[ 	]*vpsrldq zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 dd 7b[ 	]*vpsrldq zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 19 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 9c f4 c0 1d fe ff 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 5a 7f 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 9a 00 20 00 00 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 5a 80 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 9a c0 df ff ff 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 d5 ab[ 	]*vpsrlw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 71 d5 ab[ 	]*vpsrlw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d cf 71 d5 ab[ 	]*vpsrlw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 d5 7b[ 	]*vpsrlw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 11 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 94 f4 c0 1d fe ff 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 52 7f 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 92 00 20 00 00 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 52 80 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 92 c0 df ff ff 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 e5 ab[ 	]*vpsraw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 71 e5 ab[ 	]*vpsraw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d cf 71 e5 ab[ 	]*vpsraw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 e5 7b[ 	]*vpsraw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 21 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 a4 f4 c0 1d fe ff 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 62 7f 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 a2 00 20 00 00 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 62 80 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 a2 c0 df ff ff 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 f4[ 	]*vpsrlvw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 10 f4[ 	]*vpsrlvw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 10 f4[ 	]*vpsrlvw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 31[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 b4 f4 c0 1d fe ff[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 72 7f[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 b2 00 20 00 00[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 72 80[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 b2 c0 df ff ff[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 f4[ 	]*vpsravw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 11 f4[ 	]*vpsravw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 11 f4[ 	]*vpsravw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 31[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 b4 f4 c0 1d fe ff[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 72 7f[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 b2 00 20 00 00[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 72 80[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 b2 c0 df ff ff[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 f4[ 	]*vpsubb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f8 f4[ 	]*vpsubb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f8 f4[ 	]*vpsubb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 31[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 b4 f4 c0 1d fe ff[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 72 7f[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 b2 00 20 00 00[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 72 80[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 b2 c0 df ff ff[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 f4[ 	]*vpsubsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e8 f4[ 	]*vpsubsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e8 f4[ 	]*vpsubsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 31[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 b4 f4 c0 1d fe ff[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 72 7f[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 b2 00 20 00 00[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 72 80[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 b2 c0 df ff ff[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 f4[ 	]*vpsubsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e9 f4[ 	]*vpsubsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e9 f4[ 	]*vpsubsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 31[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 b4 f4 c0 1d fe ff[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 72 7f[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 b2 00 20 00 00[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 72 80[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 b2 c0 df ff ff[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 f4[ 	]*vpsubusb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d8 f4[ 	]*vpsubusb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d8 f4[ 	]*vpsubusb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 31[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 b4 f4 c0 1d fe ff[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 72 7f[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 b2 00 20 00 00[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 72 80[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 b2 c0 df ff ff[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 f4[ 	]*vpsubusw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d9 f4[ 	]*vpsubusw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d9 f4[ 	]*vpsubusw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 31[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 b4 f4 c0 1d fe ff[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 72 7f[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 b2 00 20 00 00[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 72 80[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 b2 c0 df ff ff[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 f4[ 	]*vpsubw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f9 f4[ 	]*vpsubw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f9 f4[ 	]*vpsubw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 31[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 b4 f4 c0 1d fe ff[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 72 7f[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 b2 00 20 00 00[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 72 80[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 b2 c0 df ff ff[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 f4[ 	]*vpunpckhbw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 68 f4[ 	]*vpunpckhbw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 68 f4[ 	]*vpunpckhbw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 31[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 b4 f4 c0 1d fe ff[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 72 7f[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 b2 00 20 00 00[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 72 80[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 b2 c0 df ff ff[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 f4[ 	]*vpunpckhwd zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 69 f4[ 	]*vpunpckhwd zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 69 f4[ 	]*vpunpckhwd zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 31[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 b4 f4 c0 1d fe ff[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 72 7f[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 b2 00 20 00 00[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 72 80[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 b2 c0 df ff ff[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 f4[ 	]*vpunpcklbw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 60 f4[ 	]*vpunpcklbw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 60 f4[ 	]*vpunpcklbw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 31[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 b4 f4 c0 1d fe ff[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 72 7f[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 b2 00 20 00 00[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 72 80[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 b2 c0 df ff ff[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 f4[ 	]*vpunpcklwd zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 61 f4[ 	]*vpunpcklwd zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 61 f4[ 	]*vpunpcklwd zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 31[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 b4 f4 c0 1d fe ff[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 72 7f[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 b2 00 20 00 00[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 72 80[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 b2 c0 df ff ff[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 30 ee[ 	]*vpmovwb ymm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e cf 30 ee[ 	]*vpmovwb ymm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 20 ee[ 	]*vpmovswb ymm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e cf 20 ee[ 	]*vpmovswb ymm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 10 ee[ 	]*vpmovuswb ymm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e cf 10 ee[ 	]*vpmovuswb ymm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 f4 ab[ 	]*vdbpsadbw zmm6,zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 4f 42 f4 ab[ 	]*vdbpsadbw zmm6\{k7\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 cf 42 f4 ab[ 	]*vdbpsadbw zmm6\{k7\}\{z\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 f4 7b[ 	]*vdbpsadbw zmm6,zmm5,zmm4,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 31 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 b4 f4 c0 1d fe ff 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 72 7f 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 b2 00 20 00 00 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 72 80 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 b2 c0 df ff ff 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d f4[ 	]*vpermw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 8d f4[ 	]*vpermw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 8d f4[ 	]*vpermw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d 31[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d b4 f4 c0 1d fe ff[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d 72 7f[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d b2 00 20 00 00[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d 72 80[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d b2 c0 df ff ff[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d f4[ 	]*vpermt2w zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 7d f4[ 	]*vpermt2w zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 7d f4[ 	]*vpermt2w zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d 31[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d b4 f4 c0 1d fe ff[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d 72 7f[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d b2 00 20 00 00[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d 72 80[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d b2 c0 df ff ff[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 fd ab[ 	]*vpslldq zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 fd 7b[ 	]*vpslldq zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 39 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 bc f4 c0 1d fe ff 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 7a 7f 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 ba 00 20 00 00 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 7a 80 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 ba c0 df ff ff 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 f5 ab[ 	]*vpsllw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 71 f5 ab[ 	]*vpsllw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d cf 71 f5 ab[ 	]*vpsllw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 f5 7b[ 	]*vpsllw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 31 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 b4 f4 c0 1d fe ff 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 72 7f 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 b2 00 20 00 00 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 72 80 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 b2 c0 df ff ff 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 f4[ 	]*vpsllvw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 12 f4[ 	]*vpsllvw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 12 f4[ 	]*vpsllvw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 31[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 b4 f4 c0 1d fe ff[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 72 7f[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 b2 00 20 00 00[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 72 80[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 b2 c0 df ff ff[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f f5[ 	]*vmovdqu8 zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 4f 6f f5[ 	]*vmovdqu8 zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f cf 6f f5[ 	]*vmovdqu8 zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f 31[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f b4 f4 c0 1d fe ff[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f 72 7f[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f b2 00 20 00 00[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f 72 80[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f b2 c0 df ff ff[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f f5[ 	]*vmovdqu16 zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 4f 6f f5[ 	]*vmovdqu16 zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff cf 6f f5[ 	]*vmovdqu16 zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f 31[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f b4 f4 c0 1d fe ff[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f 72 7f[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f b2 00 20 00 00[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f 72 80[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f b2 c0 df ff ff[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 41 ef[ 	]*kandq  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 41 ef[ 	]*kandd  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 42 ef[ 	]*kandnq k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 42 ef[ 	]*kandnd k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 45 ef[ 	]*korq   k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 45 ef[ 	]*kord   k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 46 ef[ 	]*kxnorq k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 46 ef[ 	]*kxnord k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 47 ef[ 	]*kxorq  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 47 ef[ 	]*kxord  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 44 ee[ 	]*knotq  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 44 ee[ 	]*knotd  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 98 ee[ 	]*kortestq k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 98 ee[ 	]*kortestd k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 99 ee[ 	]*ktestq k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 99 ee[ 	]*ktestd k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 31 ee ab[ 	]*kshiftrq k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 31 ee 7b[ 	]*kshiftrq k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 31 ee ab[ 	]*kshiftrd k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 31 ee 7b[ 	]*kshiftrd k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 33 ee ab[ 	]*kshiftlq k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 33 ee 7b[ 	]*kshiftlq k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 33 ee ab[ 	]*kshiftld k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 33 ee 7b[ 	]*kshiftld k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 90 ee[ 	]*kmovq  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 90 29[ 	]*kmovq  k5,QWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 90 ac f4 c0 1d fe ff[ 	]*kmovq  k5,QWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 90 ee[ 	]*kmovd  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 90 29[ 	]*kmovd  k5,DWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 90 ac f4 c0 1d fe ff[ 	]*kmovd  k5,DWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 91 29[ 	]*kmovq  QWORD PTR \[ecx\],k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 91 ac f4 c0 1d fe ff[ 	]*kmovq  QWORD PTR \[esp\+esi\*8-0x1e240\],k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 91 29[ 	]*kmovd  DWORD PTR \[ecx\],k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 91 ac f4 c0 1d fe ff[ 	]*kmovd  DWORD PTR \[esp\+esi\*8-0x1e240\],k5
[ 	]*[a-f0-9]+:[ 	]*c5 fb 92 e8[ 	]*kmovd  k5,eax
[ 	]*[a-f0-9]+:[ 	]*c5 fb 92 ed[ 	]*kmovd  k5,ebp
[ 	]*[a-f0-9]+:[ 	]*c5 fb 93 c5[ 	]*kmovd  eax,k5
[ 	]*[a-f0-9]+:[ 	]*c5 fb 93 ed[ 	]*kmovd  ebp,k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 4a ef[ 	]*kaddq  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 4a ef[ 	]*kaddd  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c5 cc 4b ef[ 	]*kunpckwd k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 4b ef[ 	]*kunpckdq k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 31[ 	]*vpmovwb YMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 30 31[ 	]*vpmovwb YMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 b4 f4 c0 1d fe ff[ 	]*vpmovwb YMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 72 7f[ 	]*vpmovwb YMMWORD PTR \[edx\+0xfe0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 b2 00 10 00 00[ 	]*vpmovwb YMMWORD PTR \[edx\+0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 72 80[ 	]*vpmovwb YMMWORD PTR \[edx-0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 b2 e0 ef ff ff[ 	]*vpmovwb YMMWORD PTR \[edx-0x1020\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 31[ 	]*vpmovswb YMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 20 31[ 	]*vpmovswb YMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 b4 f4 c0 1d fe ff[ 	]*vpmovswb YMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 72 7f[ 	]*vpmovswb YMMWORD PTR \[edx\+0xfe0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 b2 00 10 00 00[ 	]*vpmovswb YMMWORD PTR \[edx\+0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 72 80[ 	]*vpmovswb YMMWORD PTR \[edx-0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 b2 e0 ef ff ff[ 	]*vpmovswb YMMWORD PTR \[edx-0x1020\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 31[ 	]*vpmovuswb YMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 10 31[ 	]*vpmovuswb YMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 b4 f4 c0 1d fe ff[ 	]*vpmovuswb YMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 72 7f[ 	]*vpmovuswb YMMWORD PTR \[edx\+0xfe0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 b2 00 10 00 00[ 	]*vpmovuswb YMMWORD PTR \[edx\+0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 72 80[ 	]*vpmovuswb YMMWORD PTR \[edx-0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 b2 e0 ef ff ff[ 	]*vpmovuswb YMMWORD PTR \[edx-0x1020\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f 31[ 	]*vmovdqu8 ZMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 4f 7f 31[ 	]*vmovdqu8 ZMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f b4 f4 c0 1d fe ff[ 	]*vmovdqu8 ZMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f 72 7f[ 	]*vmovdqu8 ZMMWORD PTR \[edx\+0x1fc0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f b2 00 20 00 00[ 	]*vmovdqu8 ZMMWORD PTR \[edx\+0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f 72 80[ 	]*vmovdqu8 ZMMWORD PTR \[edx-0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f b2 c0 df ff ff[ 	]*vmovdqu8 ZMMWORD PTR \[edx-0x2040\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f 31[ 	]*vmovdqu16 ZMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 4f 7f 31[ 	]*vmovdqu16 ZMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f b4 f4 c0 1d fe ff[ 	]*vmovdqu16 ZMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f 72 7f[ 	]*vmovdqu16 ZMMWORD PTR \[edx\+0x1fc0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f b2 00 20 00 00[ 	]*vmovdqu16 ZMMWORD PTR \[edx\+0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f 72 80[ 	]*vmovdqu16 ZMMWORD PTR \[edx-0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f b2 c0 df ff ff[ 	]*vmovdqu16 ZMMWORD PTR \[edx-0x2040\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 f4[ 	]*vpermi2w zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 75 f4[ 	]*vpermi2w zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 75 f4[ 	]*vpermi2w zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 31[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 b4 f4 c0 1d fe ff[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 72 7f[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 b2 00 20 00 00[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 72 80[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 b2 c0 df ff ff[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 ed[ 	]*vptestmb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 4f 26 ed[ 	]*vptestmb k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 29[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 ac f4 c0 1d fe ff[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 6a 7f[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 aa 00 20 00 00[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 6a 80[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 aa c0 df ff ff[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 ed[ 	]*vptestmw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 4f 26 ed[ 	]*vptestmw k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 29[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 ac f4 c0 1d fe ff[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 6a 7f[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 aa 00 20 00 00[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 6a 80[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 aa c0 df ff ff[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 29 ee[ 	]*vpmovb2m k5,zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 fe 48 29 ee[ 	]*vpmovw2m k5,zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 28 f5[ 	]*vpmovm2b zmm6,k5
[ 	]*[a-f0-9]+:[ 	]*62 f2 fe 48 28 f5[ 	]*vpmovm2w zmm6,k5
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 ec[ 	]*vptestnmb k5,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 4f 26 ec[ 	]*vptestnmb k5\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 29[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 ac f4 c0 1d fe ff[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 6a 7f[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 aa 00 20 00 00[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 6a 80[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 aa c0 df ff ff[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 ec[ 	]*vptestnmw k5,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 4f 26 ec[ 	]*vptestnmw k5\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 29[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 ac f4 c0 1d fe ff[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 6a 7f[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 aa 00 20 00 00[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 6a 80[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 aa c0 df ff ff[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed ab[ 	]*vpcmpb k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 4f 3f ed ab[ 	]*vpcmpb k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 7b[ 	]*vpcmpb k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 29 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ac f4 c0 1d fe ff 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 6a 7f 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f aa 00 20 00 00 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 6a 80 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f aa c0 df ff ff 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 00[ 	]*vpcmpeqb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 02[ 	]*vpcmpleb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 68 7f 02[ 	]*vpcmpleb k5,zmm6,ZMMWORD PTR \[eax\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f a8 00 20 00 00 02[ 	]*vpcmpleb k5,zmm6,ZMMWORD PTR \[eax\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 01[ 	]*vpcmpltb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 04[ 	]*vpcmpneqb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 06[ 	]*vpcmpnleb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 05[ 	]*vpcmpnltb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed ab[ 	]*vpcmpw k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 4f 3f ed ab[ 	]*vpcmpw k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 7b[ 	]*vpcmpw k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 29 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ac f4 c0 1d fe ff 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 6a 7f 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f aa 00 20 00 00 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 6a 80 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f aa c0 df ff ff 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 00[ 	]*vpcmpeqw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 02[ 	]*vpcmplew k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 68 7f 02[ 	]*vpcmplew k5,zmm6,ZMMWORD PTR \[eax\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f a8 00 20 00 00 02[ 	]*vpcmplew k5,zmm6,ZMMWORD PTR \[eax\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 01[ 	]*vpcmpltw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 04[ 	]*vpcmpneqw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 06[ 	]*vpcmpnlew k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 05[ 	]*vpcmpnltw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed ab[ 	]*vpcmpub k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 4f 3e ed ab[ 	]*vpcmpub k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 7b[ 	]*vpcmpub k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e 29 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ac f4 c0 1d fe ff 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e 6a 7f 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e aa 00 20 00 00 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e 6a 80 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e aa c0 df ff ff 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 00[ 	]*vpcmpequb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 02[ 	]*vpcmpleub k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 01[ 	]*vpcmpltub k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 04[ 	]*vpcmpnequb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 06[ 	]*vpcmpnleub k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 05[ 	]*vpcmpnltub k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed ab[ 	]*vpcmpuw k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 4f 3e ed ab[ 	]*vpcmpuw k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 7b[ 	]*vpcmpuw k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e 29 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ac f4 c0 1d fe ff 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e 6a 7f 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e aa 00 20 00 00 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e 6a 80 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e aa c0 df ff ff 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 00[ 	]*vpcmpequw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 02[ 	]*vpcmpleuw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 01[ 	]*vpcmpltuw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 04[ 	]*vpcmpnequw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 06[ 	]*vpcmpnleuw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 05[ 	]*vpcmpnltuw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c f5[ 	]*vpabsb zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 1c f5[ 	]*vpabsb zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 1c f5[ 	]*vpabsb zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c 31[ 	]*vpabsb zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c b4 f4 c0 1d fe ff[ 	]*vpabsb zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c 72 7f[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c b2 00 20 00 00[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c 72 80[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1c b2 c0 df ff ff[ 	]*vpabsb zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d f5[ 	]*vpabsw zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 1d f5[ 	]*vpabsw zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 1d f5[ 	]*vpabsw zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d 31[ 	]*vpabsw zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d b4 f4 c0 1d fe ff[ 	]*vpabsw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d 72 7f[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d b2 00 20 00 00[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d 72 80[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 1d b2 c0 df ff ff[ 	]*vpabsw zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b f4[ 	]*vpackssdw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 6b f4[ 	]*vpackssdw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 6b f4[ 	]*vpackssdw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b 31[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b b4 f4 c0 1d fe ff[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b 30[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[eax\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b 72 7f[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b b2 00 20 00 00[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b 72 80[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 6b b2 c0 df ff ff[ 	]*vpackssdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b 72 7f[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx\+0x1fc\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b b2 00 02 00 00[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx\+0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b 72 80[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx-0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 58 6b b2 fc fd ff ff[ 	]*vpackssdw zmm6,zmm5,DWORD PTR \[edx-0x204\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 f4[ 	]*vpacksswb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 63 f4[ 	]*vpacksswb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 63 f4[ 	]*vpacksswb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 31[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 b4 f4 c0 1d fe ff[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 72 7f[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 b2 00 20 00 00[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 72 80[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 63 b2 c0 df ff ff[ 	]*vpacksswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b f4[ 	]*vpackusdw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 2b f4[ 	]*vpackusdw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 2b f4[ 	]*vpackusdw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b 31[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b b4 f4 c0 1d fe ff[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b 30[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[eax\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b 72 7f[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b b2 00 20 00 00[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b 72 80[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 2b b2 c0 df ff ff[ 	]*vpackusdw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b 72 7f[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx\+0x1fc\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b b2 00 02 00 00[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx\+0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b 72 80[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx-0x200\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 58 2b b2 fc fd ff ff[ 	]*vpackusdw zmm6,zmm5,DWORD PTR \[edx-0x204\]\{1to16\}
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 f4[ 	]*vpackuswb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 67 f4[ 	]*vpackuswb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 67 f4[ 	]*vpackuswb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 31[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 b4 f4 c0 1d fe ff[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 72 7f[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 b2 00 20 00 00[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 72 80[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 67 b2 c0 df ff ff[ 	]*vpackuswb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc f4[ 	]*vpaddb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f fc f4[ 	]*vpaddb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf fc f4[ 	]*vpaddb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc 31[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc b4 f4 c0 1d fe ff[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc 72 7f[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc b2 00 20 00 00[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc 72 80[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fc b2 c0 df ff ff[ 	]*vpaddb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec f4[ 	]*vpaddsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ec f4[ 	]*vpaddsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ec f4[ 	]*vpaddsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec 31[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec b4 f4 c0 1d fe ff[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec 72 7f[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec b2 00 20 00 00[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec 72 80[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ec b2 c0 df ff ff[ 	]*vpaddsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed f4[ 	]*vpaddsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ed f4[ 	]*vpaddsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ed f4[ 	]*vpaddsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed 31[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed b4 f4 c0 1d fe ff[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed 72 7f[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed b2 00 20 00 00[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed 72 80[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ed b2 c0 df ff ff[ 	]*vpaddsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc f4[ 	]*vpaddusb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f dc f4[ 	]*vpaddusb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf dc f4[ 	]*vpaddusb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc 31[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc b4 f4 c0 1d fe ff[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc 72 7f[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc b2 00 20 00 00[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc 72 80[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dc b2 c0 df ff ff[ 	]*vpaddusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd f4[ 	]*vpaddusw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f dd f4[ 	]*vpaddusw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf dd f4[ 	]*vpaddusw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd 31[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd b4 f4 c0 1d fe ff[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd 72 7f[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd b2 00 20 00 00[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd 72 80[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 dd b2 c0 df ff ff[ 	]*vpaddusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd f4[ 	]*vpaddw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f fd f4[ 	]*vpaddw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf fd f4[ 	]*vpaddw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd 31[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd b4 f4 c0 1d fe ff[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd 72 7f[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd b2 00 20 00 00[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd 72 80[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 fd b2 c0 df ff ff[ 	]*vpaddw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f f4 ab[ 	]*vpalignr zmm6,zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 4f 0f f4 ab[ 	]*vpalignr zmm6\{k7\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 cf 0f f4 ab[ 	]*vpalignr zmm6\{k7\}\{z\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f f4 7b[ 	]*vpalignr zmm6,zmm5,zmm4,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f 31 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f b4 f4 c0 1d fe ff 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f 72 7f 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f b2 00 20 00 00 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f 72 80 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 0f b2 c0 df ff ff 7b[ 	]*vpalignr zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 f4[ 	]*vpavgb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e0 f4[ 	]*vpavgb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e0 f4[ 	]*vpavgb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 31[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 b4 f4 c0 1d fe ff[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 72 7f[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 b2 00 20 00 00[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 72 80[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e0 b2 c0 df ff ff[ 	]*vpavgb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 f4[ 	]*vpavgw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e3 f4[ 	]*vpavgw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e3 f4[ 	]*vpavgw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 31[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 b4 f4 c0 1d fe ff[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 72 7f[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 b2 00 20 00 00[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 72 80[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e3 b2 c0 df ff ff[ 	]*vpavgw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 f4[ 	]*vpblendmb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 66 f4[ 	]*vpblendmb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 66 f4[ 	]*vpblendmb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 31[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 b4 f4 c0 1d fe ff[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 72 7f[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 b2 00 20 00 00[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 72 80[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 66 b2 c0 df ff ff[ 	]*vpblendmb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 f5[ 	]*vpbroadcastb zmm6\{k7\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 78 f5[ 	]*vpbroadcastb zmm6\{k7\}\{z\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 31[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 b4 f4 c0 1d fe ff[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 72 7f[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx\+0x7f\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 b2 80 00 00 00[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx\+0x80\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 72 80[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx-0x80\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 78 b2 7f ff ff ff[ 	]*vpbroadcastb zmm6\{k7\},BYTE PTR \[edx-0x81\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7a f0[ 	]*vpbroadcastb zmm6,eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 7a f0[ 	]*vpbroadcastb zmm6\{k7\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 7a f0[ 	]*vpbroadcastb zmm6\{k7\}\{z\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7a f5[ 	]*vpbroadcastb zmm6,ebp
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 f5[ 	]*vpbroadcastw zmm6\{k7\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 79 f5[ 	]*vpbroadcastw zmm6\{k7\}\{z\},xmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 31[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 b4 f4 c0 1d fe ff[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 72 7f[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx\+0xfe\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 b2 00 01 00 00[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx\+0x100\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 72 80[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx-0x100\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 79 b2 fe fe ff ff[ 	]*vpbroadcastw zmm6\{k7\},WORD PTR \[edx-0x102\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7b f0[ 	]*vpbroadcastw zmm6,eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 7b f0[ 	]*vpbroadcastw zmm6\{k7\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 7b f0[ 	]*vpbroadcastw zmm6\{k7\}\{z\},eax
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 48 7b f5[ 	]*vpbroadcastw zmm6,ebp
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 ed[ 	]*vpcmpeqb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 74 ed[ 	]*vpcmpeqb k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 29[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 ac f4 c0 1d fe ff[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 6a 7f[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 aa 00 20 00 00[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 6a 80[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 74 aa c0 df ff ff[ 	]*vpcmpeqb k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 ed[ 	]*vpcmpeqw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 75 ed[ 	]*vpcmpeqw k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 29[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 ac f4 c0 1d fe ff[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 6a 7f[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 aa 00 20 00 00[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 6a 80[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 75 aa c0 df ff ff[ 	]*vpcmpeqw k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 ed[ 	]*vpcmpgtb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 64 ed[ 	]*vpcmpgtb k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 29[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 ac f4 c0 1d fe ff[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 6a 7f[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 aa 00 20 00 00[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 6a 80[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 64 aa c0 df ff ff[ 	]*vpcmpgtb k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 ed[ 	]*vpcmpgtw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 65 ed[ 	]*vpcmpgtw k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 29[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 ac f4 c0 1d fe ff[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 6a 7f[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 aa 00 20 00 00[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 6a 80[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 65 aa c0 df ff ff[ 	]*vpcmpgtw k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 f4[ 	]*vpblendmw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 66 f4[ 	]*vpblendmw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 66 f4[ 	]*vpblendmw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 31[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 b4 f4 c0 1d fe ff[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 72 7f[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 b2 00 20 00 00[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 72 80[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 66 b2 c0 df ff ff[ 	]*vpblendmw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 f4[ 	]*vpmaddubsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 04 f4[ 	]*vpmaddubsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 04 f4[ 	]*vpmaddubsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 31[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 b4 f4 c0 1d fe ff[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 72 7f[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 b2 00 20 00 00[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 72 80[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 04 b2 c0 df ff ff[ 	]*vpmaddubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 f4[ 	]*vpmaddwd zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f5 f4[ 	]*vpmaddwd zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f5 f4[ 	]*vpmaddwd zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 31[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 b4 f4 c0 1d fe ff[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 72 7f[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 b2 00 20 00 00[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 72 80[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f5 b2 c0 df ff ff[ 	]*vpmaddwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c f4[ 	]*vpmaxsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 3c f4[ 	]*vpmaxsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 3c f4[ 	]*vpmaxsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c 31[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c b4 f4 c0 1d fe ff[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c 72 7f[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c b2 00 20 00 00[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c 72 80[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3c b2 c0 df ff ff[ 	]*vpmaxsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee f4[ 	]*vpmaxsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ee f4[ 	]*vpmaxsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ee f4[ 	]*vpmaxsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee 31[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee b4 f4 c0 1d fe ff[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee 72 7f[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee b2 00 20 00 00[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee 72 80[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ee b2 c0 df ff ff[ 	]*vpmaxsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de f4[ 	]*vpmaxub zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f de f4[ 	]*vpmaxub zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf de f4[ 	]*vpmaxub zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de 31[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de b4 f4 c0 1d fe ff[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de 72 7f[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de b2 00 20 00 00[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de 72 80[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 de b2 c0 df ff ff[ 	]*vpmaxub zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e f4[ 	]*vpmaxuw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 3e f4[ 	]*vpmaxuw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 3e f4[ 	]*vpmaxuw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e 31[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e b4 f4 c0 1d fe ff[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e 72 7f[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e b2 00 20 00 00[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e 72 80[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3e b2 c0 df ff ff[ 	]*vpmaxuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 f4[ 	]*vpminsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 38 f4[ 	]*vpminsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 38 f4[ 	]*vpminsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 31[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 b4 f4 c0 1d fe ff[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 72 7f[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 b2 00 20 00 00[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 72 80[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 38 b2 c0 df ff ff[ 	]*vpminsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea f4[ 	]*vpminsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f ea f4[ 	]*vpminsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf ea f4[ 	]*vpminsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea 31[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea b4 f4 c0 1d fe ff[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea 72 7f[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea b2 00 20 00 00[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea 72 80[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 ea b2 c0 df ff ff[ 	]*vpminsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da f4[ 	]*vpminub zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f da f4[ 	]*vpminub zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf da f4[ 	]*vpminub zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da 31[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da b4 f4 c0 1d fe ff[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da 72 7f[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da b2 00 20 00 00[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da 72 80[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 da b2 c0 df ff ff[ 	]*vpminub zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a f4[ 	]*vpminuw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 3a f4[ 	]*vpminuw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 3a f4[ 	]*vpminuw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a 31[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a b4 f4 c0 1d fe ff[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a 72 7f[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a b2 00 20 00 00[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a 72 80[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 3a b2 c0 df ff ff[ 	]*vpminuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 f5[ 	]*vpmovsxbw zmm6\{k7\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 20 f5[ 	]*vpmovsxbw zmm6\{k7\}\{z\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 31[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 b4 f4 c0 1d fe ff[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 72 7f[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx\+0xfe0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 b2 00 10 00 00[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx\+0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 72 80[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 20 b2 e0 ef ff ff[ 	]*vpmovsxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1020\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 f5[ 	]*vpmovzxbw zmm6\{k7\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d cf 30 f5[ 	]*vpmovzxbw zmm6\{k7\}\{z\},ymm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 31[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 b4 f4 c0 1d fe ff[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 72 7f[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx\+0xfe0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 b2 00 10 00 00[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx\+0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 72 80[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7d 4f 30 b2 e0 ef ff ff[ 	]*vpmovzxbw zmm6\{k7\},YMMWORD PTR \[edx-0x1020\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b f4[ 	]*vpmulhrsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 0b f4[ 	]*vpmulhrsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 0b f4[ 	]*vpmulhrsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b 31[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b b4 f4 c0 1d fe ff[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b 72 7f[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b b2 00 20 00 00[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b 72 80[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 0b b2 c0 df ff ff[ 	]*vpmulhrsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 f4[ 	]*vpmulhuw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e4 f4[ 	]*vpmulhuw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e4 f4[ 	]*vpmulhuw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 31[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 b4 f4 c0 1d fe ff[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 72 7f[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 b2 00 20 00 00[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 72 80[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e4 b2 c0 df ff ff[ 	]*vpmulhuw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 f4[ 	]*vpmulhw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e5 f4[ 	]*vpmulhw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e5 f4[ 	]*vpmulhw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 31[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 b4 f4 c0 1d fe ff[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 72 7f[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 b2 00 20 00 00[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 72 80[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e5 b2 c0 df ff ff[ 	]*vpmulhw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 f4[ 	]*vpmullw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d5 f4[ 	]*vpmullw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d5 f4[ 	]*vpmullw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 31[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 b4 f4 c0 1d fe ff[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 72 7f[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 b2 00 20 00 00[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 72 80[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d5 b2 c0 df ff ff[ 	]*vpmullw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 f4[ 	]*vpsadbw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 31[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 b4 f4 c0 1d fe ff[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 72 7f[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 b2 00 20 00 00[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 72 80[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f6 b2 c0 df ff ff[ 	]*vpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 f4[ 	]*vpshufb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 4f 00 f4[ 	]*vpshufb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 cf 00 f4[ 	]*vpshufb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 31[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 b4 f4 c0 1d fe ff[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 72 7f[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 b2 00 20 00 00[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 72 80[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 55 48 00 b2 c0 df ff ff[ 	]*vpshufb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 f5 ab[ 	]*vpshufhw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 4f 70 f5 ab[ 	]*vpshufhw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e cf 70 f5 ab[ 	]*vpshufhw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 f5 7b[ 	]*vpshufhw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 31 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 b4 f4 c0 1d fe ff 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 72 7f 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 b2 00 20 00 00 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 72 80 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7e 48 70 b2 c0 df ff ff 7b[ 	]*vpshufhw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 f5 ab[ 	]*vpshuflw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 4f 70 f5 ab[ 	]*vpshuflw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f cf 70 f5 ab[ 	]*vpshuflw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 f5 7b[ 	]*vpshuflw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 31 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 b4 f4 c0 1d fe ff 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 72 7f 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 b2 00 20 00 00 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 72 80 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 70 b2 c0 df ff ff 7b[ 	]*vpshuflw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 f4[ 	]*vpsllw zmm6\{k7\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f1 f4[ 	]*vpsllw zmm6\{k7\}\{z\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 31[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 b4 f4 c0 1d fe ff[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 72 7f[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x7f0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 b2 00 08 00 00[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 72 80[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f1 b2 f0 f7 ff ff[ 	]*vpsllw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x810\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 f4[ 	]*vpsraw zmm6\{k7\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e1 f4[ 	]*vpsraw zmm6\{k7\}\{z\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 31[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 b4 f4 c0 1d fe ff[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 72 7f[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x7f0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 b2 00 08 00 00[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 72 80[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e1 b2 f0 f7 ff ff[ 	]*vpsraw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x810\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 f4[ 	]*vpsrlw zmm6\{k7\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d1 f4[ 	]*vpsrlw zmm6\{k7\}\{z\},zmm5,xmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 31[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 b4 f4 c0 1d fe ff[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 72 7f[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x7f0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 b2 00 08 00 00[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx\+0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 72 80[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x800\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d1 b2 f0 f7 ff ff[ 	]*vpsrlw zmm6\{k7\},zmm5,XMMWORD PTR \[edx-0x810\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 dd ab[ 	]*vpsrldq zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 dd 7b[ 	]*vpsrldq zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 19 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 9c f4 c0 1d fe ff 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 5a 7f 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 9a 00 20 00 00 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 5a 80 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 9a c0 df ff ff 7b[ 	]*vpsrldq zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 d5 ab[ 	]*vpsrlw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 71 d5 ab[ 	]*vpsrlw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d cf 71 d5 ab[ 	]*vpsrlw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 d5 7b[ 	]*vpsrlw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 11 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 94 f4 c0 1d fe ff 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 52 7f 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 92 00 20 00 00 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 52 80 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 92 c0 df ff ff 7b[ 	]*vpsrlw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 e5 ab[ 	]*vpsraw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 71 e5 ab[ 	]*vpsraw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d cf 71 e5 ab[ 	]*vpsraw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 e5 7b[ 	]*vpsraw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 21 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 a4 f4 c0 1d fe ff 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 62 7f 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 a2 00 20 00 00 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 62 80 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 a2 c0 df ff ff 7b[ 	]*vpsraw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 f4[ 	]*vpsrlvw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 10 f4[ 	]*vpsrlvw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 10 f4[ 	]*vpsrlvw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 31[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 b4 f4 c0 1d fe ff[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 72 7f[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 b2 00 20 00 00[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 72 80[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 10 b2 c0 df ff ff[ 	]*vpsrlvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 f4[ 	]*vpsravw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 11 f4[ 	]*vpsravw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 11 f4[ 	]*vpsravw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 31[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 b4 f4 c0 1d fe ff[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 72 7f[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 b2 00 20 00 00[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 72 80[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 11 b2 c0 df ff ff[ 	]*vpsravw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 f4[ 	]*vpsubb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f8 f4[ 	]*vpsubb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f8 f4[ 	]*vpsubb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 31[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 b4 f4 c0 1d fe ff[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 72 7f[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 b2 00 20 00 00[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 72 80[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f8 b2 c0 df ff ff[ 	]*vpsubb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 f4[ 	]*vpsubsb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e8 f4[ 	]*vpsubsb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e8 f4[ 	]*vpsubsb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 31[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 b4 f4 c0 1d fe ff[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 72 7f[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 b2 00 20 00 00[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 72 80[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e8 b2 c0 df ff ff[ 	]*vpsubsb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 f4[ 	]*vpsubsw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f e9 f4[ 	]*vpsubsw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf e9 f4[ 	]*vpsubsw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 31[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 b4 f4 c0 1d fe ff[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 72 7f[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 b2 00 20 00 00[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 72 80[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 e9 b2 c0 df ff ff[ 	]*vpsubsw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 f4[ 	]*vpsubusb zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d8 f4[ 	]*vpsubusb zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d8 f4[ 	]*vpsubusb zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 31[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 b4 f4 c0 1d fe ff[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 72 7f[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 b2 00 20 00 00[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 72 80[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d8 b2 c0 df ff ff[ 	]*vpsubusb zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 f4[ 	]*vpsubusw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f d9 f4[ 	]*vpsubusw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf d9 f4[ 	]*vpsubusw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 31[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 b4 f4 c0 1d fe ff[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 72 7f[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 b2 00 20 00 00[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 72 80[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 d9 b2 c0 df ff ff[ 	]*vpsubusw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 f4[ 	]*vpsubw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f f9 f4[ 	]*vpsubw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf f9 f4[ 	]*vpsubw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 31[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 b4 f4 c0 1d fe ff[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 72 7f[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 b2 00 20 00 00[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 72 80[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 f9 b2 c0 df ff ff[ 	]*vpsubw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 f4[ 	]*vpunpckhbw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 68 f4[ 	]*vpunpckhbw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 68 f4[ 	]*vpunpckhbw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 31[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 b4 f4 c0 1d fe ff[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 72 7f[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 b2 00 20 00 00[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 72 80[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 68 b2 c0 df ff ff[ 	]*vpunpckhbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 f4[ 	]*vpunpckhwd zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 69 f4[ 	]*vpunpckhwd zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 69 f4[ 	]*vpunpckhwd zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 31[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 b4 f4 c0 1d fe ff[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 72 7f[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 b2 00 20 00 00[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 72 80[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 69 b2 c0 df ff ff[ 	]*vpunpckhwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 f4[ 	]*vpunpcklbw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 60 f4[ 	]*vpunpcklbw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 60 f4[ 	]*vpunpcklbw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 31[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 b4 f4 c0 1d fe ff[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 72 7f[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 b2 00 20 00 00[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 72 80[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 60 b2 c0 df ff ff[ 	]*vpunpcklbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 f4[ 	]*vpunpcklwd zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 4f 61 f4[ 	]*vpunpcklwd zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 cf 61 f4[ 	]*vpunpcklwd zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 31[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 b4 f4 c0 1d fe ff[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 72 7f[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 b2 00 20 00 00[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 72 80[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 55 48 61 b2 c0 df ff ff[ 	]*vpunpcklwd zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 30 ee[ 	]*vpmovwb ymm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e cf 30 ee[ 	]*vpmovwb ymm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 20 ee[ 	]*vpmovswb ymm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e cf 20 ee[ 	]*vpmovswb ymm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 10 ee[ 	]*vpmovuswb ymm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e cf 10 ee[ 	]*vpmovuswb ymm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 f4 ab[ 	]*vdbpsadbw zmm6,zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 4f 42 f4 ab[ 	]*vdbpsadbw zmm6\{k7\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 cf 42 f4 ab[ 	]*vdbpsadbw zmm6\{k7\}\{z\},zmm5,zmm4,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 f4 7b[ 	]*vdbpsadbw zmm6,zmm5,zmm4,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 31 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 b4 f4 c0 1d fe ff 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 72 7f 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 b2 00 20 00 00 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 72 80 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 55 48 42 b2 c0 df ff ff 7b[ 	]*vdbpsadbw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d f4[ 	]*vpermw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 8d f4[ 	]*vpermw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 8d f4[ 	]*vpermw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d 31[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d b4 f4 c0 1d fe ff[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d 72 7f[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d b2 00 20 00 00[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d 72 80[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 8d b2 c0 df ff ff[ 	]*vpermw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d f4[ 	]*vpermt2w zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 7d f4[ 	]*vpermt2w zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 7d f4[ 	]*vpermt2w zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d 31[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d b4 f4 c0 1d fe ff[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d 72 7f[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d b2 00 20 00 00[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d 72 80[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 7d b2 c0 df ff ff[ 	]*vpermt2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 fd ab[ 	]*vpslldq zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 fd 7b[ 	]*vpslldq zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 39 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 bc f4 c0 1d fe ff 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 7a 7f 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 ba 00 20 00 00 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 7a 80 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 73 ba c0 df ff ff 7b[ 	]*vpslldq zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 f5 ab[ 	]*vpsllw zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 4f 71 f5 ab[ 	]*vpsllw zmm6\{k7\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d cf 71 f5 ab[ 	]*vpsllw zmm6\{k7\}\{z\},zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 f5 7b[ 	]*vpsllw zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 31 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 b4 f4 c0 1d fe ff 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 72 7f 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 b2 00 20 00 00 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 72 80 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f1 4d 48 71 b2 c0 df ff ff 7b[ 	]*vpsllw zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 f4[ 	]*vpsllvw zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 12 f4[ 	]*vpsllvw zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 12 f4[ 	]*vpsllvw zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 31[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 b4 f4 c0 1d fe ff[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 72 7f[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 b2 00 20 00 00[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 72 80[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 12 b2 c0 df ff ff[ 	]*vpsllvw zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f f5[ 	]*vmovdqu8 zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 4f 6f f5[ 	]*vmovdqu8 zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f cf 6f f5[ 	]*vmovdqu8 zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f 31[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f b4 f4 c0 1d fe ff[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f 72 7f[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f b2 00 20 00 00[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f 72 80[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 6f b2 c0 df ff ff[ 	]*vmovdqu8 zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f f5[ 	]*vmovdqu16 zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 4f 6f f5[ 	]*vmovdqu16 zmm6\{k7\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff cf 6f f5[ 	]*vmovdqu16 zmm6\{k7\}\{z\},zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f 31[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f b4 f4 c0 1d fe ff[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f 72 7f[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f b2 00 20 00 00[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f 72 80[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 6f b2 c0 df ff ff[ 	]*vmovdqu16 zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 41 ef[ 	]*kandq  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 41 ef[ 	]*kandd  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 42 ef[ 	]*kandnq k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 42 ef[ 	]*kandnd k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 45 ef[ 	]*korq   k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 45 ef[ 	]*kord   k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 46 ef[ 	]*kxnorq k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 46 ef[ 	]*kxnord k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 47 ef[ 	]*kxorq  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 47 ef[ 	]*kxord  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 44 ee[ 	]*knotq  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 44 ee[ 	]*knotd  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 98 ee[ 	]*kortestq k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 98 ee[ 	]*kortestd k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 99 ee[ 	]*ktestq k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 99 ee[ 	]*ktestd k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 31 ee ab[ 	]*kshiftrq k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 31 ee 7b[ 	]*kshiftrq k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 31 ee ab[ 	]*kshiftrd k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 31 ee 7b[ 	]*kshiftrd k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 33 ee ab[ 	]*kshiftlq k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 f9 33 ee 7b[ 	]*kshiftlq k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 33 ee ab[ 	]*kshiftld k5,k6,0xab
[ 	]*[a-f0-9]+:[ 	]*c4 e3 79 33 ee 7b[ 	]*kshiftld k5,k6,0x7b
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 90 ee[ 	]*kmovq  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 90 29[ 	]*kmovq  k5,QWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 90 ac f4 c0 1d fe ff[ 	]*kmovq  k5,QWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 90 ee[ 	]*kmovd  k5,k6
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 90 29[ 	]*kmovd  k5,DWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 90 ac f4 c0 1d fe ff[ 	]*kmovd  k5,DWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 91 29[ 	]*kmovq  QWORD PTR \[ecx\],k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f8 91 ac f4 c0 1d fe ff[ 	]*kmovq  QWORD PTR \[esp\+esi\*8-0x1e240\],k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 91 29[ 	]*kmovd  DWORD PTR \[ecx\],k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 f9 91 ac f4 c0 1d fe ff[ 	]*kmovd  DWORD PTR \[esp\+esi\*8-0x1e240\],k5
[ 	]*[a-f0-9]+:[ 	]*c5 fb 92 e8[ 	]*kmovd  k5,eax
[ 	]*[a-f0-9]+:[ 	]*c5 fb 92 ed[ 	]*kmovd  k5,ebp
[ 	]*[a-f0-9]+:[ 	]*c5 fb 93 c5[ 	]*kmovd  eax,k5
[ 	]*[a-f0-9]+:[ 	]*c5 fb 93 ed[ 	]*kmovd  ebp,k5
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 4a ef[ 	]*kaddq  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cd 4a ef[ 	]*kaddd  k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c5 cc 4b ef[ 	]*kunpckwd k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*c4 e1 cc 4b ef[ 	]*kunpckdq k5,k6,k7
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 31[ 	]*vpmovwb YMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 30 31[ 	]*vpmovwb YMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 b4 f4 c0 1d fe ff[ 	]*vpmovwb YMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 72 7f[ 	]*vpmovwb YMMWORD PTR \[edx\+0xfe0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 b2 00 10 00 00[ 	]*vpmovwb YMMWORD PTR \[edx\+0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 72 80[ 	]*vpmovwb YMMWORD PTR \[edx-0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 30 b2 e0 ef ff ff[ 	]*vpmovwb YMMWORD PTR \[edx-0x1020\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 31[ 	]*vpmovswb YMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 20 31[ 	]*vpmovswb YMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 b4 f4 c0 1d fe ff[ 	]*vpmovswb YMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 72 7f[ 	]*vpmovswb YMMWORD PTR \[edx\+0xfe0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 b2 00 10 00 00[ 	]*vpmovswb YMMWORD PTR \[edx\+0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 72 80[ 	]*vpmovswb YMMWORD PTR \[edx-0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 20 b2 e0 ef ff ff[ 	]*vpmovswb YMMWORD PTR \[edx-0x1020\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 31[ 	]*vpmovuswb YMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 4f 10 31[ 	]*vpmovuswb YMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 b4 f4 c0 1d fe ff[ 	]*vpmovuswb YMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 72 7f[ 	]*vpmovuswb YMMWORD PTR \[edx\+0xfe0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 b2 00 10 00 00[ 	]*vpmovuswb YMMWORD PTR \[edx\+0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 72 80[ 	]*vpmovuswb YMMWORD PTR \[edx-0x1000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 10 b2 e0 ef ff ff[ 	]*vpmovuswb YMMWORD PTR \[edx-0x1020\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f 31[ 	]*vmovdqu8 ZMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 4f 7f 31[ 	]*vmovdqu8 ZMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f b4 f4 c0 1d fe ff[ 	]*vmovdqu8 ZMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f 72 7f[ 	]*vmovdqu8 ZMMWORD PTR \[edx\+0x1fc0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f b2 00 20 00 00[ 	]*vmovdqu8 ZMMWORD PTR \[edx\+0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f 72 80[ 	]*vmovdqu8 ZMMWORD PTR \[edx-0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 7f 48 7f b2 c0 df ff ff[ 	]*vmovdqu8 ZMMWORD PTR \[edx-0x2040\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f 31[ 	]*vmovdqu16 ZMMWORD PTR \[ecx\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 4f 7f 31[ 	]*vmovdqu16 ZMMWORD PTR \[ecx\]\{k7\},zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f b4 f4 c0 1d fe ff[ 	]*vmovdqu16 ZMMWORD PTR \[esp\+esi\*8-0x1e240\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f 72 7f[ 	]*vmovdqu16 ZMMWORD PTR \[edx\+0x1fc0\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f b2 00 20 00 00[ 	]*vmovdqu16 ZMMWORD PTR \[edx\+0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f 72 80[ 	]*vmovdqu16 ZMMWORD PTR \[edx-0x2000\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f1 ff 48 7f b2 c0 df ff ff[ 	]*vmovdqu16 ZMMWORD PTR \[edx-0x2040\],zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 f4[ 	]*vpermi2w zmm6,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 4f 75 f4[ 	]*vpermi2w zmm6\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 cf 75 f4[ 	]*vpermi2w zmm6\{k7\}\{z\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 31[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 b4 f4 c0 1d fe ff[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 72 7f[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 b2 00 20 00 00[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 72 80[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d5 48 75 b2 c0 df ff ff[ 	]*vpermi2w zmm6,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 ed[ 	]*vptestmb k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 4f 26 ed[ 	]*vptestmb k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 29[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 ac f4 c0 1d fe ff[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 6a 7f[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 aa 00 20 00 00[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 6a 80[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 4d 48 26 aa c0 df ff ff[ 	]*vptestmb k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 ed[ 	]*vptestmw k5,zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 4f 26 ed[ 	]*vptestmw k5\{k7\},zmm6,zmm5
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 29[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 ac f4 c0 1d fe ff[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 6a 7f[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 aa 00 20 00 00[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 6a 80[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 cd 48 26 aa c0 df ff ff[ 	]*vptestmw k5,zmm6,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 29 ee[ 	]*vpmovb2m k5,zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 fe 48 29 ee[ 	]*vpmovw2m k5,zmm6
[ 	]*[a-f0-9]+:[ 	]*62 f2 7e 48 28 f5[ 	]*vpmovm2b zmm6,k5
[ 	]*[a-f0-9]+:[ 	]*62 f2 fe 48 28 f5[ 	]*vpmovm2w zmm6,k5
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 ec[ 	]*vptestnmb k5,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 4f 26 ec[ 	]*vptestnmb k5\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 29[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 ac f4 c0 1d fe ff[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 6a 7f[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 aa 00 20 00 00[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 6a 80[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 56 48 26 aa c0 df ff ff[ 	]*vptestnmb k5,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 ec[ 	]*vptestnmw k5,zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 4f 26 ec[ 	]*vptestnmw k5\{k7\},zmm5,zmm4
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 29[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[ecx\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 ac f4 c0 1d fe ff[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[esp\+esi\*8-0x1e240\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 6a 7f[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx\+0x1fc0\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 aa 00 20 00 00[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx\+0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 6a 80[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx-0x2000\]
[ 	]*[a-f0-9]+:[ 	]*62 f2 d6 48 26 aa c0 df ff ff[ 	]*vptestnmw k5,zmm5,ZMMWORD PTR \[edx-0x2040\]
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed ab[ 	]*vpcmpb k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 4f 3f ed ab[ 	]*vpcmpb k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ed 7b[ 	]*vpcmpb k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 29 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f ac f4 c0 1d fe ff 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 6a 7f 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f aa 00 20 00 00 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f 6a 80 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3f aa c0 df ff ff 7b[ 	]*vpcmpb k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed ab[ 	]*vpcmpw k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 4f 3f ed ab[ 	]*vpcmpw k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ed 7b[ 	]*vpcmpw k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 29 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f ac f4 c0 1d fe ff 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 6a 7f 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f aa 00 20 00 00 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f 6a 80 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3f aa c0 df ff ff 7b[ 	]*vpcmpw k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed ab[ 	]*vpcmpub k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 4f 3e ed ab[ 	]*vpcmpub k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ed 7b[ 	]*vpcmpub k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e 29 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e ac f4 c0 1d fe ff 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e 6a 7f 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e aa 00 20 00 00 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e 6a 80 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 4d 48 3e aa c0 df ff ff 7b[ 	]*vpcmpub k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed ab[ 	]*vpcmpuw k5,zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 4f 3e ed ab[ 	]*vpcmpuw k5\{k7\},zmm6,zmm5,0xab
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ed 7b[ 	]*vpcmpuw k5,zmm6,zmm5,0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e 29 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[ecx\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e ac f4 c0 1d fe ff 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[esp\+esi\*8-0x1e240\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e 6a 7f 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx\+0x1fc0\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e aa 00 20 00 00 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx\+0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e 6a 80 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx-0x2000\],0x7b
[ 	]*[a-f0-9]+:[ 	]*62 f3 cd 48 3e aa c0 df ff ff 7b[ 	]*vpcmpuw k5,zmm6,ZMMWORD PTR \[edx-0x2040\],0x7b
#pass
