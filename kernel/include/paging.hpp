#pragma once

#include <list.hpp>
#include <multiboot.h>
#include <types.h>

typedef uintptr_t AddressSpace;
class Bitmap;

#define PG_INVALID_ADDRESS  (static_cast<uintptr_t>(-1))
#define PG_CURRENT_ADDR_SPC PG_INVALID_ADDRESS

class Paging
{
    struct DMAPointerHead
    {
        uintptr_t Address = 0;
        size_t Size = 0;

        bool operator ==(DMAPointerHead ph) { return Address == ph.Address; }
    };

    static uintptr_t memoryTop;
    static Bitmap *pageFrameBitmap;
    static AddressSpace kernelAddressSpace;
    static List<DMAPointerHead> dmaPtrList;
    static uintptr_t currentMMIOPtr;

    static void *moveMemTop(intptr_t incr);
    static uint64_t getRAMSize(multiboot_info_t *mboot);
    static void *map4k(uint slot, uintptr_t pa);
    static void *alloc4k(uintptr_t pa);
    static void free4k(void *ptr);
    static void mapUser(uintptr_t as, void *start, void *end, bool write);
public:
    static void Initialize(multiboot_info_t *mboot);
    static void BuildAddressSpace(AddressSpace as);
    static AddressSpace GetCurrentAddressSpace();
    static AddressSpace GetKernelAddressSpace();
    static void FlushTLB();
    static void InvalidatePage(uintptr_t addr);
    static bool MapPage(AddressSpace as, uintptr_t va, uintptr_t pa, bool user, bool write);
    static bool UnMapPage(AddressSpace as, uintptr_t va);
    static bool MapPages(AddressSpace as, uintptr_t va, uintptr_t pa, bool user, bool write, size_t n);
    static bool UnMapPages(AddressSpace as, uintptr_t va, size_t n);
    static void UnmapRange(AddressSpace as, uintptr_t startVA, size_t rangeSize);
    static void CloneRange(AddressSpace dstAS, uintptr_t srcAS, uintptr_t startVA, size_t rangeSize);
    static uintptr_t GetPhysicalAddress(AddressSpace as, uintptr_t va);
    static uintptr_t AllocFrame();
    static uintptr_t AllocFrame(size_t alignment);
    static uintptr_t AllocFrames(size_t n);
    static uintptr_t AllocFrames(size_t n, size_t alignment);
    static bool FreeFrame(uintptr_t pa);
    static bool FreeFrames(uintptr_t pa, size_t n);
    static void ReserveFrame(uintptr_t pa);
    static void ReserveFrames(uintptr_t pa, size_t n);
    static void *AllocDMA(size_t size);
    static void *AllocDMA(size_t size, size_t alignment);
    static uintptr_t GetDMAPhysicalAddress(void *ptr);
    static void FreeDMA(void *ptr);
    static void *AllocMMIO(size_t size, uintptr_t pa);
    static void FreeMMIO(void *ptr, size_t size);
    static size_t GetTotalFrames();
    static size_t GetFreeFrames();
    static size_t GetUsedFrames();
    static size_t GetTotalBytes();
    static size_t GetFreeBytes();
    static size_t GetUsedBytes();
    static size_t CountPresentPages(AddressSpace as, uintptr_t startVA, size_t rangeSize);
    static void DumpAddressSpace(AddressSpace as);
};
