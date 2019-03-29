#include <bitmap.hpp>
#include <cpu.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <new.hpp>
#include <paging.hpp>
#include <sysdefs.h>

extern "C" void *_end;

uintptr_t Paging::memoryTop = (uintptr_t)&_end;
Bitmap *Paging::pageFrameBitmap;
AddressSpace Paging::kernelAddressSpace;
List<Paging::DMAPointerHead> Paging::dmaPtrList;

void *Paging::moveMemTop(intptr_t incr)
{
    uintptr_t oldTop = memoryTop;
    memoryTop += incr;
    return (void *)oldTop;
}

uint64_t Paging::getRAMSize(multiboot_info_t *mboot)
{
    uint64_t ramSize = 0;
    for(uintptr_t mmapAddr = mboot->mmap_addr,
        mmapEnd = mboot->mmap_addr + mboot->mmap_length;
        mmapAddr < mmapEnd;)
    {
        multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)(mmapAddr + KERNEL_BASE);
        uintptr_t blockEnd = mmap->addr + mmap->len;
        if(mmap->addr >= (1 << 20) && mmap->type == 1 && blockEnd > ramSize)
            ramSize += blockEnd;
        mmapAddr += mmap->size + 4;
    }
    return ramSize;
}

void Paging::Initialize(multiboot_info_t *mboot)
{
    // allocate kernel address space
    kernelAddressSpace = (uintptr_t)moveMemTop(PAGE_SIZE) - KERNEL_BASE;
    Memory::Zero((void *)kernelAddressSpace, PAGE_SIZE);

    // allocate page bitmap
    uint64_t ramSize = getRAMSize(mboot);
    size_t bitCount = ramSize / PAGE_SIZE;
    size_t byteCount = bitCount / 8;
    void *pageBitmapBits = moveMemTop(align(byteCount, PAGE_SIZE));
    void *pageBitmapStruct = moveMemTop(align(sizeof(Bitmap), PAGE_SIZE));
    pageFrameBitmap = new (pageBitmapStruct) Bitmap(bitCount, pageBitmapBits, false);

    // reserve used page frames
    uintptr_t memoryTopPA = memoryTop - KERNEL_BASE;
    for(uintptr_t pa = 0; pa < memoryTopPA; pa += PAGE_SIZE)
        pageFrameBitmap->SetBit(pa >> PAGE_SHIFT, true);

    // map whole physical memory to kernel space
    MapPages(kernelAddressSpace, KERNEL_BASE, 0, false, true, ramSize >> PAGE_SHIFT);

    cpuSetCR3(kernelAddressSpace);
}

AddressSpace Paging::GetCurrentAddressSpace()
{
    return (AddressSpace)cpuGetCR3();
}

void Paging::FlushTLB()
{
    cpuSetCR3(cpuGetCR3());
}

void Paging::InvalidatePage(uintptr_t addr)
{
    cpuInvalidatePage(addr);
}

bool Paging::MapPage(AddressSpace as, uintptr_t va, uintptr_t pa, bool user, bool write)
{
    va &= ~PAGE_MASK;
    pa &= ~PAGE_MASK;

    uint pml1idx = (va >> 12) & 511;
    uint pml2idx = (va >> 21) & 511;
    uint pml3idx = (va >> 30) & 511;
    uint pml4idx = (va >> 39) & 511;

    bool invalidate = false;
    if(as == PG_CURRENT_ADDR_SPC)
    {
        as = GetCurrentAddressSpace();
        invalidate = true;
    }

    uintptr_t *pml4 = (uintptr_t *)(as + KERNEL_BASE);
    if(!(pml4[pml4idx] & 1))
    {
        uintptr_t addr = AllocFrame();
        if(addr == PG_INVALID_ADDRESS)
            return false;
        pml4[pml4idx] = addr | 0x07;
        Memory::Zero((void *)(addr + KERNEL_BASE), PAGE_SIZE);
    }
    uintptr_t *pml3 = (uintptr_t *)((pml4[pml4idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml3[pml3idx] & 1))
    {
        uintptr_t addr = AllocFrame();
        if(addr == PG_INVALID_ADDRESS)
            return false;
        pml3[pml3idx] = addr | 0x07;
        Memory::Zero((void *)(addr + KERNEL_BASE), PAGE_SIZE);
    }
    uintptr_t *pml2 = (uintptr_t *)((pml3[pml3idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml2[pml2idx] & 1))
    {
        uintptr_t addr = AllocFrame();
        if(addr == PG_INVALID_ADDRESS)
            return false;
        pml2[pml2idx] = addr | 0x07;
        Memory::Zero((void *)(addr + KERNEL_BASE), PAGE_SIZE);
    }
    uintptr_t *pml1 = (uintptr_t *)((pml2[pml2idx] + KERNEL_BASE) & ~PAGE_MASK);

    pml1[pml1idx] = pa | 0x01 | (write ? 0x02 : 0x00) | (user ? 0x04 : 0x00);
    if(invalidate)
        InvalidatePage(va);
    return true;
}

bool Paging::UnMapPage(AddressSpace as, uintptr_t va)
{
    va &= ~PAGE_MASK;

    uint pml1idx = (va >> 12) & 511;
    uint pml2idx = (va >> 21) & 511;
    uint pml3idx = (va >> 30) & 511;
    uint pml4idx = (va >> 39) & 511;

    bool invalidate = false;
    if(as == PG_CURRENT_ADDR_SPC)
    {
        as = GetCurrentAddressSpace();
        invalidate = true;
    }

    uintptr_t *pml4 = (uintptr_t *)(as + KERNEL_BASE);

    if(!(pml4[pml4idx] & 1))
        return false;

    uintptr_t *pml3 = (uintptr_t *)((pml4[pml4idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml3[pml3idx] & 1))
    {
        bool freePML3 = true;
        for(uint i = 0; i < 512 && freePML3; ++i)
            freePML3 = !(pml3[i] & 1);
        if(freePML3)
        {
            pml4[pml4idx] = 0;
            FreeFrame((uintptr_t)pml3);
        }
        return false;
    }

    uintptr_t *pml2 = (uintptr_t *)((pml3[pml3idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml2[pml2idx] & 1))
    {
        bool freePML2 = true;
        for(uint i = 0; i < 512 && freePML2; ++i)
            freePML2 = !(pml2[i] & 1);
        if(freePML2)
        {
            pml3[pml3idx] = 0;
            FreeFrame((uintptr_t)pml2);
        }
        return false;
    }

    uintptr_t *pml1 = (uintptr_t *)((pml2[pml2idx] + KERNEL_BASE) & ~PAGE_MASK);
    pml1[pml1idx] = 0;

    bool freePML1 = true;
    for(uint i = 0; i < 512 && freePML1; ++i)
        freePML1 = !(pml1[i] & 1);
    if(freePML1)
    {
        pml2[pml2idx] = 0;
        FreeFrame((uintptr_t)pml1);
    }

    bool freePML2 = true;
    for(uint i = 0; i < 512 && freePML2; ++i)
        freePML2 = !(pml2[i] & 1);
    if(freePML2)
    {
        pml3[pml3idx] = 0;
        FreeFrame((uintptr_t)pml2);
    }

    bool freePML3 = true;
    for(uint i = 0; i < 512 && freePML3; ++i)
        freePML3 = !(pml3[i] & 1);
    if(freePML3)
    {
        pml4[pml4idx] = 0;
        FreeFrame((uintptr_t)pml3);
    }

    if(invalidate)
        InvalidatePage(va);
    return true;
}

bool Paging::MapPages(AddressSpace as, uintptr_t va, uintptr_t pa, bool user, bool write, size_t n)
{
    if(as == PG_INVALID_ADDRESS)
        as = GetCurrentAddressSpace();

    for(uintptr_t i = 0; i < n; ++i, va += PAGE_SIZE, pa += PAGE_SIZE)
    {
        if(!MapPage(as, va, pa, user, write))
            return false;
    }
    return true;
}

bool Paging::UnMapPages(AddressSpace as, uintptr_t va, size_t n)
{
    if(as == PG_CURRENT_ADDR_SPC)
        as = GetCurrentAddressSpace();

    for(uintptr_t i = 0; i < n; ++i, va += PAGE_SIZE)
    {
        if(!UnMapPage(as, va))
            return false;
    }
    return true;
}

uintptr_t Paging::GetPhysicalAddress(AddressSpace as, uintptr_t va)
{
    if(as == PG_CURRENT_ADDR_SPC)
        as = GetCurrentAddressSpace();

    va &= ~PAGE_MASK;

    uint pml1idx = (va >> 12) & 511;
    uint pml2idx = (va >> 21) & 511;
    uint pml3idx = (va >> 30) & 511;
    uint pml4idx = (va >> 39) & 511;

    uintptr_t *pml4 = (uintptr_t *)(as + KERNEL_BASE);
    if(!(pml4[pml4idx] & 1))
        return PG_INVALID_ADDRESS;
    uintptr_t *pml3 = (uintptr_t *)((pml4[pml4idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml3[pml3idx] & 1))
        return PG_INVALID_ADDRESS;
    uintptr_t *pml2 = (uintptr_t *)((pml3[pml3idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml2[pml2idx] & 1))
        return PG_INVALID_ADDRESS;
    uintptr_t *pml1 = (uintptr_t *)((pml2[pml2idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml1[pml1idx] & 1))
        return PG_INVALID_ADDRESS;
    return pml1[pml1idx] & ~PAGE_MASK;
}

uintptr_t Paging::AllocFrame()
{
    bool cs = cpuDisableInterrupts();
    uint bit = pageFrameBitmap->FindFirst(false);
    if(bit == ~0)
    {
        cpuRestoreInterrupts(cs);
        return ~0;
    }
    uintptr_t addr = bit * PAGE_SIZE;
    pageFrameBitmap->SetBit(bit, true);
    cpuRestoreInterrupts(cs);
    return addr;
}

uintptr_t Paging::AllocFrame(size_t alignment)
{
    if(alignment % PAGE_SIZE)
        return ~0; // alignment must be multiple of page size
    if(!alignment) alignment = PAGE_SIZE;
    uint bit = 0;
    uint bitCount = pageFrameBitmap->GetBitCount();
    uint step = alignment / PAGE_SIZE;
    bool cs = cpuDisableInterrupts();

    for(; bit < bitCount && pageFrameBitmap->GetBit(bit); bit += step);
    if(bit >= bitCount)
    {
        cpuRestoreInterrupts(cs);
        return ~0;
    }

    pageFrameBitmap->SetBit(bit, true);
    cpuRestoreInterrupts(cs);
    return bit * PAGE_SIZE;
}

uintptr_t Paging::AllocFrames(size_t n)
{
    bool cs = cpuDisableInterrupts();
    uint bit = pageFrameBitmap->FindFirst(false, n);
    if(bit == ~0)
    {
        cpuRestoreInterrupts(cs);
        return ~0;
    }
    for(uint i = 0; i < n; ++i)
        pageFrameBitmap->SetBit(bit + i, true);
    uintptr_t addr = bit * PAGE_SIZE;
    cpuRestoreInterrupts(cs);
    return addr;
}

uintptr_t Paging::AllocFrames(size_t n, size_t alignment)
{
    if(alignment % PAGE_SIZE)
        return ~0; // alignment must be multiple of page size
    if(!alignment) alignment = PAGE_SIZE;
    uint bit = 0;
    uint bitCount = pageFrameBitmap->GetBitCount();
    uint step = alignment / PAGE_SIZE;
    bool cs = cpuDisableInterrupts();

    for(; bit < bitCount; bit += step)
    {
        int obit = bit;
        int okbits = 0;
        for(; bit < bitCount && !pageFrameBitmap->GetBit(bit) && okbits < n ; ++bit, ++okbits);
        if(okbits >= n)
        {
            bit = obit;
            break;
        }
    }
    if(bit >= bitCount)
    {
        cpuRestoreInterrupts(cs);
        return ~0;
    }

    for(int i = 0; i < n; ++ i)
        pageFrameBitmap->SetBit(bit + i, true);
    cpuRestoreInterrupts(cs);
    return bit * PAGE_SIZE;
}

bool Paging::FreeFrame(uintptr_t pa)
{
    uint bit = pa / PAGE_SIZE;
    bool cs = cpuDisableInterrupts();
    bool state = pageFrameBitmap->GetBit(bit);
    if(!state)
    {
        cpuRestoreInterrupts(cs);
        return false;
    }
    pageFrameBitmap->SetBit(bit, false);
    cpuRestoreInterrupts(cs);
    return true;
}

bool Paging::FreeFrames(uintptr_t pa, size_t n)
{
    for(uint i = 0; i < n; ++i)
    {
        if(!FreeFrame(pa))
            return false;
        pa += PAGE_SIZE;
    }
    return true;
}

