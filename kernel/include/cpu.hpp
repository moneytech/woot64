#pragma once

#include <types.h>

extern "C" {

// interrupts
void cpuEnableInterrupts();             // enable interrupts
bool cpuDisableInterrupts();            // returns true if interrupts were enabled
bool cpuAreInterruptsEnabled();         // returns true if interrupts were enabled without disabling them
void cpuRestoreInterrupts(bool state);  // restores interrupt state returned by cpuDisableInterrupts

// halt instructions
void cpuSystemHalt(uintptr_t errcode);      // errcode will be put into eax before halting
void cpuWaitForInterrupt(uintptr_t debug);  // as above (for debug purpose)

// control registers
uintptr_t cpuGetCR0();
void cpuSetCR0(uintptr_t value);
uintptr_t cpuGetCR2();
uintptr_t cpuGetCR3();
void cpuSetCR3(uintptr_t value);
uintptr_t cpuGetCR4();
void cpuSetCR4(uintptr_t value);

// paging
void cpuInvalidatePage(uintptr_t addr);

// io port operations
uint8_t _inb(uint16_t port);
uint16_t _inw(uint16_t port);
uint64_t _inl(uint16_t port);
uint64_t _ind(uint16_t port);
void _outb(uint16_t port, uint8_t value);
void _outw(uint16_t port, uint16_t value);
void _outl(uint16_t port, uint64_t value);
void _outd(uint16_t port, uint64_t value);
void _insb(void *buffer, uint16_t port, size_t n);
void _insw(void *buffer, uint16_t port, size_t n);
void _insl(void *buffer, uint16_t port, size_t n);
void _insd(void *buffer, uint16_t port, size_t n);
void _outsb(const void *buffer, uint16_t port, size_t n);
void _outsw(const void *buffer, uint16_t port, size_t n);
void _outsl(const void *buffer, uint16_t port, size_t n);
void _outsd(const void *buffer, uint16_t port, size_t n);

void cpuIOSetBitsB(uint16_t port, uint8_t mask);
void cpuIOSetBitsW(uint16_t port, uint16_t mask);
void cpuIOSetBitsL(uint16_t port, uint64_t mask);
void cpuIOSetBitsD(uint16_t port, uint64_t mask);
void cpuIOClrBitsB(uint16_t port, uint8_t mask);
void cpuIOClrBitsW(uint16_t port, uint16_t mask);
void cpuIOClrBitsL(uint16_t port, uint64_t mask);
void cpuIOClrBitsD(uint16_t port, uint64_t mask);

// floating point stuff
void cpuFXSave(void *buffer);
void cpuFXRstor(void *buffer);
void cpuInitFPU(uint16_t cw);
void cpuEnableSSE();

}
