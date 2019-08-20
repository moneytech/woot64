#include <bitmap.hpp>
#include <cpu.hpp>
#include <debug.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <new.hpp>
#include <paging.hpp>
#include <sysdefs.h>

extern "C" void *_utext_start;
extern "C" void *_utext_end;
extern "C" void *_end;

#define PG_PRESENT  (1 << 0)
#define PG_WRITE    (1 << 1)
#define PG_USER     (1 << 2)
#define PG_COW      (1 << 9)

uintptr_t Paging::memoryTop = reinterpret_cast<uintptr_t>(&_end);
size_t Paging::pageFrameCount;
uint32_t *Paging::pageFrameMap;
AddressSpace Paging::kernelAddressSpace;
List<Paging::DMAPointerHead> Paging::dmaPtrList;
uintptr_t Paging::currentMMIOPtr = KERNEL_MMIO_BASE;
Ints::Handler Paging::cowHandler = { nullptr, cowHandlerFunc, nullptr };

void *Paging::moveMemTop(intptr_t incr)
{
    uintptr_t oldTop = memoryTop;
    memoryTop += static_cast<uintptr_t>(incr);
    return reinterpret_cast<void *>(oldTop);
}

uint64_t Paging::getRAMSize(multiboot_info_t *mboot)
{
    uint64_t ramSize = 0;
    for(uintptr_t mmapAddr = mboot->mmap_addr,
        mmapEnd = mboot->mmap_addr + mboot->mmap_length;
        mmapAddr < mmapEnd;)
    {
        multiboot_memory_map_t *mmap = reinterpret_cast<multiboot_memory_map_t *>(mmapAddr + KERNEL_BASE);
        uintptr_t blockEnd = mmap->addr + mmap->len;
        if(mmap->addr >= (1 << 20) && mmap->type == 1 && blockEnd > ramSize)
            ramSize += blockEnd;
        mmapAddr += mmap->size + 4;
    }
    return ramSize;
}

uintptr_t Paging::getPTE(AddressSpace as, uintptr_t va)
{
    if(as == PG_CURRENT_ADDR_SPC)
        as = GetCurrentAddressSpace();

    va &= ~PAGE_MASK;

    uint pml1idx = (va >> 12) & 511;
    uint pml2idx = (va >> 21) & 511;
    uint pml3idx = (va >> 30) & 511;
    uint pml4idx = (va >> 39) & 511;

    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(as + KERNEL_BASE);
    if(!(pml4[pml4idx] & PG_PRESENT))
        return PG_INVALID_ADDRESS;
    uintptr_t *pml3 = reinterpret_cast<uintptr_t *>((pml4[pml4idx] & ~PAGE_MASK) + KERNEL_BASE);
    if(!(pml3[pml3idx] & PG_PRESENT))
        return PG_INVALID_ADDRESS;
    uintptr_t *pml2 = reinterpret_cast<uintptr_t *>((pml3[pml3idx] & ~PAGE_MASK) + KERNEL_BASE);
    if(!(pml2[pml2idx] & PG_PRESENT))
        return PG_INVALID_ADDRESS;
    uintptr_t *pml1 = reinterpret_cast<uintptr_t *>((pml2[pml2idx] & ~PAGE_MASK) + KERNEL_BASE);
    if(!(pml1[pml1idx] & PG_PRESENT))
        return PG_INVALID_ADDRESS;
    return pml1[pml1idx];
}

bool Paging::cowHandlerFunc(Ints::State *state, void *context)
{
    (void)context;
    if(!(state->ErrorCode & PG_WRITE))
        return false;
    return HandleCOW(cpuGetCR2());
}

void Paging::Initialize(multiboot_info_t *mboot)
{
    // allocate kernel address space
    kernelAddressSpace = reinterpret_cast<uintptr_t>(moveMemTop(PAGE_SIZE)) - KERNEL_BASE;
    Memory::Zero(reinterpret_cast<void *>(kernelAddressSpace), PAGE_SIZE);

    // allocate page frame map
    uint64_t ramSize = getRAMSize(mboot);
    pageFrameCount = ramSize >> PAGE_SHIFT;
    size_t mapSize = pageFrameCount * sizeof(uint32_t);
    pageFrameMap = reinterpret_cast<uint32_t *>(moveMemTop(align(mapSize, PAGE_SIZE)));
    Memory::Zero(pageFrameMap, mapSize);

    // reserve used page frames
    uintptr_t memoryTopPA = memoryTop - KERNEL_BASE;
    ReserveFrames(0, memoryTopPA >> PAGE_SHIFT);

    // map whole physical memory to kernel space
    MapPages(kernelAddressSpace, KERNEL_BASE, 0, false, true, pageFrameCount);

    // map .text.user section as user code
    uintptr_t utext_start = reinterpret_cast<uintptr_t>(&_utext_start);
    uintptr_t utext_end = reinterpret_cast<uintptr_t>(&_utext_end);
    utext_start &= ~PAGE_MASK;
    utext_end = align(utext_end, PAGE_SIZE);
    size_t utext_pages = (utext_end - utext_start) >> PAGE_SHIFT;
    MapPages(kernelAddressSpace, utext_start,
             GetPhysicalAddress(kernelAddressSpace, utext_start),
             true, false, utext_pages);

    // TODO: allocate ALL (and never release) kernel PML4 entries to ensure kernel
    //       address space coherency over all processes

    cpuSetCR3(kernelAddressSpace);
    Ints::RegisterHandler(14, &cowHandler);
}

void Paging::BuildAddressSpace(AddressSpace as)
{
    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(as + KERNEL_BASE);
    uintptr_t *kernelPml4 = reinterpret_cast<uintptr_t *>(kernelAddressSpace + KERNEL_BASE);
    Memory::Zero(pml4, PAGE_SIZE);

    // copy all kernel pml4 entries
    Memory::Move(pml4 + 256, kernelPml4 + 256, 2048);
}

AddressSpace Paging::GetCurrentAddressSpace()
{
    bool ints = cpuDisableInterrupts();
    AddressSpace res = static_cast<AddressSpace>(cpuGetCR3());
    cpuRestoreInterrupts(ints);
    return res;
}

AddressSpace Paging::GetKernelAddressSpace()
{
    return kernelAddressSpace;
}

void Paging::FlushTLB()
{
    bool ints = cpuDisableInterrupts();
    cpuSetCR3(cpuGetCR3());
    cpuRestoreInterrupts(ints);
}

void Paging::InvalidatePage(uintptr_t addr)
{
    cpuInvalidatePage(addr);
}

bool Paging::MapPage(AddressSpace as, uintptr_t va, uintptr_t pa, bool user, bool write, bool cow)
{
    va &= ~PAGE_MASK;
    pa &= ~PAGE_MASK;

    uint pml1idx = (va >> 12) & 511;
    uint pml2idx = (va >> 21) & 511;
    uint pml3idx = (va >> 30) & 511;
    uint pml4idx = (va >> 39) & 511;

    if(as == PG_CURRENT_ADDR_SPC)
        as = GetCurrentAddressSpace();

    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(as + KERNEL_BASE);
    if(!(pml4[pml4idx] & 1))
    {
        uintptr_t addr = AllocFrame();
        if(addr == PG_INVALID_ADDRESS)
            return false;
        pml4[pml4idx] = addr | 0x07;
        Memory::Zero(reinterpret_cast<void *>(addr + KERNEL_BASE), PAGE_SIZE);
    }
    uintptr_t *pml3 = reinterpret_cast<uintptr_t *>((pml4[pml4idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml3[pml3idx] & 1))
    {
        uintptr_t addr = AllocFrame();
        if(addr == PG_INVALID_ADDRESS)
            return false;
        pml3[pml3idx] = addr | 0x07;
        Memory::Zero(reinterpret_cast<void *>(addr + KERNEL_BASE), PAGE_SIZE);
    }
    uintptr_t *pml2 = reinterpret_cast<uintptr_t *>((pml3[pml3idx] + KERNEL_BASE) & ~PAGE_MASK);
    if(!(pml2[pml2idx] & 1))
    {
        uintptr_t addr = AllocFrame();
        if(addr == PG_INVALID_ADDRESS)
            return false;
        pml2[pml2idx] = addr | 0x07;
        Memory::Zero(reinterpret_cast<void *>(addr + KERNEL_BASE), PAGE_SIZE);
    }
    uintptr_t *pml1 = reinterpret_cast<uintptr_t *>((pml2[pml2idx] + KERNEL_BASE) & ~PAGE_MASK);

    pml1[pml1idx] = pa | PG_PRESENT | (write ? PG_WRITE : 0) | (user ? PG_USER : 0) | (cow ? PG_COW : 0);
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

    if(as == PG_CURRENT_ADDR_SPC)
        as = GetCurrentAddressSpace();

    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(as + KERNEL_BASE);

    if(!(pml4[pml4idx] & 1))
        return false;

    uintptr_t pml3PA = pml4[pml4idx] & ~PAGE_MASK;
    uintptr_t *pml3 = reinterpret_cast<uintptr_t *>(pml3PA + KERNEL_BASE);
    if(!(pml3[pml3idx] & 1))
    {
        bool freePML3 = true;
        for(uint i = 0; i < 512 && freePML3; ++i)
            freePML3 = !(pml3[i] & 1);
        if(freePML3)
        {
            pml4[pml4idx] = 0;
            FreeFrame(reinterpret_cast<uintptr_t>(pml3));
        }
        return false;
    }

    uintptr_t pml2PA = pml3[pml3idx] & ~PAGE_MASK;
    uintptr_t *pml2 = reinterpret_cast<uintptr_t *>(pml2PA + KERNEL_BASE);
    if(!(pml2[pml2idx] & 1))
    {
        bool freePML2 = true;
        for(uint i = 0; i < 512 && freePML2; ++i)
            freePML2 = !(pml2[i] & 1);
        if(freePML2)
        {
            pml3[pml3idx] = 0;
            FreeFrame(reinterpret_cast<uintptr_t>(pml2));
        }
        return false;
    }

    uintptr_t pml1PA = pml2[pml2idx] & ~PAGE_MASK;
    uintptr_t *pml1 = reinterpret_cast<uintptr_t *>(pml1PA + KERNEL_BASE);
    pml1[pml1idx] = 0;

    bool freePML1 = true;
    for(uint i = 0; i < 512 && freePML1; ++i)
        freePML1 = !(pml1[i] & 1);
    if(freePML1)
    {
        pml2[pml2idx] = 0;
        FreeFrame(reinterpret_cast<uintptr_t>(pml1PA));
    }

    bool freePML2 = true;
    for(uint i = 0; i < 512 && freePML2; ++i)
        freePML2 = !(pml2[i] & 1);
    if(freePML2)
    {
        pml3[pml3idx] = 0;
        FreeFrame(reinterpret_cast<uintptr_t>(pml2PA));
    }

    if(va < KERNEL_BASE)
    {
        bool freePML3 = true;
        for(uint i = 0; i < 512 && freePML3; ++i)
            freePML3 = !(pml3[i] & 1);
        if(freePML3)
        {
            pml4[pml4idx] = 0;
            FreeFrame(reinterpret_cast<uintptr_t>(pml3PA));
        }
    }

    InvalidatePage(va);
    return true;
}

bool Paging::MapPages(AddressSpace as, uintptr_t va, uintptr_t pa, bool user, bool write, size_t n)
{
    if(as == PG_INVALID_ADDRESS)
        as = GetCurrentAddressSpace();

    for(uintptr_t i = 0; i < n; ++i, va += PAGE_SIZE, pa += PAGE_SIZE)
    {
        if(!MapPage(as, va, pa, user, write, false))
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

void Paging::UnmapRange(AddressSpace as, uintptr_t startVA, size_t rangeSize)
{
    AddressSpace curAS = GetCurrentAddressSpace();
    if(as == PG_CURRENT_ADDR_SPC)
        as = curAS;
    bool invalidate = as == curAS;

    uintptr_t endVA = startVA + rangeSize;
    uintptr_t scanStartVA = ((startVA >> 39) & 511) << 39;
    uintptr_t scanEndVA = align(endVA, (1ull << 39));
    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(as + KERNEL_BASE);
    for(uintptr_t scanVA = scanStartVA; scanVA < scanEndVA; scanVA += (1ull << 39))
    {
        if(scanVA >= KERNEL_BASE)
            return; // never unmap kernel memory

        uintptr_t pml4idx = (scanVA >> 39) & 511;
        if(!(pml4[pml4idx] & 1))
            continue;

        uintptr_t *pml3 = reinterpret_cast<uintptr_t *>((pml4[pml4idx] & ~PAGE_MASK) + KERNEL_BASE);
        for(uintptr_t pml3idx = 0; pml3idx < 512; ++pml3idx)
        {
            if(!(pml3[pml3idx] & 1))
                continue;

            uintptr_t *pml2 = reinterpret_cast<uintptr_t *>((pml3[pml3idx] & ~PAGE_MASK) + KERNEL_BASE);
            for(uintptr_t pml2idx = 0; pml2idx < 512; ++pml2idx)
            {
                if(!(pml2[pml2idx] & 1))
                    continue;

                uintptr_t *pml1 = reinterpret_cast<uintptr_t *>((pml2[pml2idx] & ~PAGE_MASK) + KERNEL_BASE);
                for(uintptr_t pml1idx = 0; pml1idx < 512; ++pml1idx)
                {
                    uintptr_t va = pml4idx << 39 | pml3idx << 30 | pml2idx << 21 | pml1idx << 12;
                    if(va < startVA || va >= endVA)
                        continue;
                    if(va >= USER_END && va < KERNEL_BASE)
                        return; // trying to unmap invalid address
                    if(!(pml1[pml1idx] & 1))
                        continue;
                    FreeFrame(pml1[pml1idx] & ~PAGE_MASK);
                    pml1[pml1idx] = 0;
                    if(invalidate)
                        InvalidatePage(va);
                }
            }
        }
    }
}

void Paging::CloneRange(AddressSpace dstAS, uintptr_t srcAS, uintptr_t startVA, size_t rangeSize)
{
    startVA &= ~PAGE_MASK;
    rangeSize &= ~PAGE_MASK;

    uintptr_t endVA = startVA + rangeSize;
    uintptr_t scanStartVA = ((startVA >> 39) & 511) << 39;
    uintptr_t scanEndVA = align(endVA, (1ull << 39));

    AddressSpace curAS = GetCurrentAddressSpace();
    if(dstAS == PG_CURRENT_ADDR_SPC) dstAS = curAS;
    if(srcAS == PG_CURRENT_ADDR_SPC) srcAS = curAS;
    if(dstAS == srcAS) return; // nothing to do
    bool invalidate = dstAS == curAS;

    bool ints = cpuDisableInterrupts();

    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(srcAS + KERNEL_BASE);
    for(uintptr_t scanVA = scanStartVA; scanVA < scanEndVA; scanVA += (1ull << 39))
    {
        uintptr_t pml4idx = (scanVA >> 39) & 511;
        if(!(pml4[pml4idx] & 1))
            continue;

        uintptr_t *pml3 = reinterpret_cast<uintptr_t *>((pml4[pml4idx] & ~PAGE_MASK) + KERNEL_BASE);
        for(uintptr_t pml3idx = 0; pml3idx < 512; ++pml3idx)
        {
            if(!(pml3[pml3idx] & 1))
                continue;

            uintptr_t *pml2 = reinterpret_cast<uintptr_t *>((pml3[pml3idx] & ~PAGE_MASK) + KERNEL_BASE);
            for(uintptr_t pml2idx = 0; pml2idx < 512; ++pml2idx)
            {
                if(!(pml2[pml2idx] & 1))
                    continue;

                uintptr_t *pml1 = reinterpret_cast<uintptr_t *>((pml2[pml2idx] & ~PAGE_MASK) + KERNEL_BASE);
                for(uintptr_t pml1idx = 0; pml1idx < 512; ++pml1idx)
                {
                    uintptr_t va = pml4idx << 39 | pml3idx << 30 | pml2idx << 21 | pml1idx << 12;
                    if(va < startVA || va >= endVA)
                        continue;
                    if(va >= USER_END && va < KERNEL_BASE)
                        return; // trying to clone invalid address
                    if(!(pml1[pml1idx] & 1))
                        continue;

                    uintptr_t entry = pml1[pml1idx];
                    bool user = entry & 0x04;
                    bool write = entry & 0x02;
                    entry &= ~PAGE_MASK;
                    uintptr_t srcPA = entry;

                    ReserveFrame(srcPA);
                    Paging::MapPage(dstAS, va, srcPA, user, false, write);
                    if(invalidate)
                        InvalidatePage(va);
                }
            }
        }
    }
    cpuRestoreInterrupts(ints);
}

uintptr_t Paging::GetPhysicalAddress(AddressSpace as, uintptr_t va)
{
    uintptr_t pte = getPTE(as, va);
    return pte == PG_INVALID_ADDRESS ? PG_INVALID_ADDRESS : pte & ~PAGE_MASK;
}

uintptr_t Paging::AllocFrame()
{   // TODO: Do something less dumb here
    uint32_t *pfm = pageFrameMap;
    bool cs = cpuDisableInterrupts();
    for(uintptr_t i = 0; i < pageFrameCount; ++i, ++pfm)
    {
        if(!(*pfm))
        {
            *pfm = 1;
            return i << PAGE_SHIFT;
        }
    }
    cpuRestoreInterrupts(cs);
    return PG_INVALID_ADDRESS;
}

uintptr_t Paging::AllocFrame(size_t alignment)
{
    if(alignment % PAGE_SIZE) return PG_INVALID_ADDRESS;  // alignment must be multiple of page size
    if(!alignment) alignment = PAGE_SIZE;    
    uintptr_t pg = 0;
    uintptr_t step = alignment / PAGE_SIZE;
    bool cs = cpuDisableInterrupts();    
    while(pg < pageFrameCount && pageFrameMap[pg] != 0)
        pg += step;
    if(pg >= pageFrameCount)
    {
        cpuRestoreInterrupts(cs);
        return PG_INVALID_ADDRESS;
    }
    pageFrameMap[pg] = 1;
    cpuRestoreInterrupts(cs);    
    return pg * PAGE_SIZE;
}

uintptr_t Paging::AllocFrames(size_t n)
{
    bool cs = cpuDisableInterrupts();
    for(uintptr_t pg = 0; pg < pageFrameCount;)
    {
        bool found = true;
        for(uintptr_t i = 0; i < n; ++i)
        {
            if(pageFrameMap[pg + i] != 0)
            {
                found = false;
                break;
            }
        }
        if(!found)
        {
            pg += n;
            continue;
        }

        for(uintptr_t i = 0; i < n; ++i)
            pageFrameMap[pg + i] = 1;
        return pg << PAGE_SHIFT;
    }

    cpuRestoreInterrupts(cs);
    return PG_INVALID_ADDRESS;
}

uintptr_t Paging::AllocFrames(size_t n, size_t alignment)
{
    if(alignment % PAGE_SIZE) return PG_INVALID_ADDRESS; // alignment must be multiple of page size
    if(!alignment) alignment = PAGE_SIZE;
    uintptr_t pg = 0;
    uintptr_t step = alignment / PAGE_SIZE;
    bool cs = cpuDisableInterrupts();
    for(; pg < pageFrameCount; pg += step)
    {
        uintptr_t opg = pg;
        uintptr_t okpgs = 0;
        while(pg < pageFrameCount && !pageFrameMap[pg] && okpgs < n)
        {
            ++pg;
            ++okpgs;
        }
        if(okpgs >= n)
        {
            pg = opg;
            break;
        }
    }
    if(pg >= pageFrameCount)
    {
        cpuRestoreInterrupts(cs);
        return PG_INVALID_ADDRESS;
    }
    for(uintptr_t i = 0; i < n; ++i)
        pageFrameMap[pg + i] = 1;
    cpuRestoreInterrupts(cs);
    return pg << PAGE_SHIFT;
}

void Paging::FreeFrame(uintptr_t pa)
{
    uint32_t *pfm = pageFrameMap + (pa >> PAGE_SHIFT);
    bool cs = cpuDisableInterrupts();
    if(*pfm) --(*pfm);
    cpuRestoreInterrupts(cs);
}

void Paging::FreeFrames(uintptr_t pa, size_t n)
{
    uint32_t *pfm = pageFrameMap + (pa >> PAGE_SHIFT);
    bool cs = cpuDisableInterrupts();
    while(n--) if(*pfm) --(*(pfm++));
    cpuRestoreInterrupts(cs);
}

void Paging::ReserveFrame(uintptr_t pa)
{
    bool cs = cpuDisableInterrupts();
    ++pageFrameMap[pa >> PAGE_SHIFT];
    cpuRestoreInterrupts(cs);
}

void Paging::ReserveFrames(uintptr_t pa, size_t n)
{
    uint32_t *pfm = pageFrameMap + (pa >> PAGE_SHIFT);
    bool cs = cpuDisableInterrupts();
    while(n--) ++(*(pfm++));
    cpuRestoreInterrupts(cs);
}

void *Paging::AllocDMA(size_t size)
{
    return AllocDMA(size, PAGE_SIZE);
}

void *Paging::AllocDMA(size_t size, size_t alignment)
{
    // TODO: make DMA physical addresses be allocatable in specific ranges
    if(!size) return nullptr;

    size = align(size, PAGE_SIZE);
    size_t nPages = size / PAGE_SIZE;
    uintptr_t pa = AllocFrames(nPages, alignment); // allocate n pages in ONE block
    if(pa == PG_INVALID_ADDRESS) return nullptr;
    bool ints = cpuDisableInterrupts();
    uintptr_t va = 0;
    if(!dmaPtrList.Count())
    {
        va = KERNEL_DMA_BASE;
        dmaPtrList.Append(DMAPointerHead { va, size });
    }
    else
    {
        for(auto it = dmaPtrList.begin(); it != dmaPtrList.end(); ++it)
        {
            DMAPointerHead ph = *it;
            uintptr_t blockEnd = ph.Address + ph.Size;
            auto nextNode = it.GetNextNode();

            if(!nextNode)
            {
                va = blockEnd;
                dmaPtrList.Append(DMAPointerHead { va, size });
                break;
            }

            DMAPointerHead nextPh = nextNode->Value;
            uintptr_t newBlockEnd = blockEnd + size;

            if(nextPh.Address >= newBlockEnd)
            {
                va = blockEnd;
                DMAPointerHead newPh = { va, size };
                dmaPtrList.InsertBefore(newPh, nextPh);
                break;
            }
        }
    }

    MapPages(PG_CURRENT_ADDR_SPC, va, pa, false, true, nPages);
    void *ptr = reinterpret_cast<void *>(va);
    Memory::Zero(ptr, size);
    cpuRestoreInterrupts(ints);
    return ptr;
}

uintptr_t Paging::GetDMAPhysicalAddress(void *ptr)
{
    return GetPhysicalAddress(PG_CURRENT_ADDR_SPC, reinterpret_cast<uintptr_t>(ptr));
}

void Paging::FreeDMA(void *ptr)
{
    uintptr_t va = reinterpret_cast<uintptr_t>(ptr);
    bool ints = cpuDisableInterrupts();
    DMAPointerHead ph = { va, 0 };
    ph = dmaPtrList.Find(ph, nullptr);
    if(!ph.Address || !ph.Size)
    {
        cpuRestoreInterrupts(ints);
        return;
    }
    dmaPtrList.Remove(ph, nullptr, false);
    size_t size = ph.Size;
    size_t nPages = size / PAGE_SIZE;

    uintptr_t pa = GetPhysicalAddress(PG_CURRENT_ADDR_SPC, va);
    if(pa == PG_INVALID_ADDRESS)
    {
        cpuRestoreInterrupts(ints);
        return;
    }
    UnMapPages(PG_CURRENT_ADDR_SPC, va, nPages);
    FreeFrames(pa, nPages);
    cpuRestoreInterrupts(ints);
}

void *Paging::AllocMMIO(size_t size, uintptr_t pa)
{
    // TODO: add proper allocator/deallocator
    if(!size) return nullptr;
    size = align(size, PAGE_SIZE);
    bool ints = cpuDisableInterrupts();
    uintptr_t va = currentMMIOPtr;
    currentMMIOPtr += size;
    MapPages(PG_CURRENT_ADDR_SPC, va, pa, false, true, size >> PAGE_SHIFT);
    cpuRestoreInterrupts(ints);
    return reinterpret_cast<void *>(va);
}

void Paging::FreeMMIO(void *ptr, size_t size)
{
    // TODO: add proper allocator/deallocator
    size = align(size, PAGE_SIZE);
    UnMapPages(PG_CURRENT_ADDR_SPC, reinterpret_cast<uintptr_t>(ptr), size >> PAGE_SHIFT);
}

size_t Paging::GetTotalFrames()
{
    return pageFrameCount;
}

size_t Paging::GetFreeFrames()
{
    uint32_t *pfm = pageFrameMap;
    bool ints = cpuDisableInterrupts();
    size_t res = 0;
    for(uintptr_t pg = 0; pg < pageFrameCount; ++pg, ++pfm)
    {
        if(!(*pfm))
            ++res;
    }
    cpuRestoreInterrupts(ints);
    return res;
}

size_t Paging::GetUsedFrames()
{
    uint32_t *pfm = pageFrameMap;
    bool ints = cpuDisableInterrupts();
    size_t res = 0;
    for(uintptr_t pg = 0; pg < pageFrameCount; ++pg, ++pfm)
    {
        if(*pfm != 0)
            ++res;
    }
    cpuRestoreInterrupts(ints);
    return res;
}

size_t Paging::GetTotalBytes()
{
    return PAGE_SIZE * GetTotalFrames();
}

size_t Paging::GetFreeBytes()
{
    return PAGE_SIZE * GetFreeFrames();
}

size_t Paging::GetUsedBytes()
{
    return PAGE_SIZE * GetUsedFrames();
}

size_t Paging::CountPresentPages(AddressSpace as, uintptr_t startVA, size_t rangeSize)
{
    AddressSpace curAS = GetCurrentAddressSpace();
    if(as == PG_CURRENT_ADDR_SPC)
        as = curAS;

    size_t res = 0;
    uintptr_t endVA = startVA + rangeSize;
    uintptr_t scanStartVA = ((startVA >> 39) & 511) << 39;
    uintptr_t scanEndVA = align(endVA, (1ull << 39));
    uintptr_t *pml4 = reinterpret_cast<uintptr_t *>(as + KERNEL_BASE);
    for(uintptr_t scanVA = scanStartVA; scanVA < scanEndVA; scanVA += (1ull << 39))
    {
        uintptr_t pml4idx = (scanVA >> 39) & 511;
        if(!(pml4[pml4idx] & 1))
            continue;

        uintptr_t *pml3 = reinterpret_cast<uintptr_t *>((pml4[pml4idx] & ~PAGE_MASK) + KERNEL_BASE);
        for(uintptr_t pml3idx = 0; pml3idx < 511; ++pml3idx)
        {
            if(!(pml3[pml3idx] & 1))
                continue;

            uintptr_t *pml2 = reinterpret_cast<uintptr_t *>((pml3[pml3idx] & ~PAGE_MASK) + KERNEL_BASE);
            for(uintptr_t pml2idx = 0; pml2idx < 511; ++pml2idx)
            {
                if(!(pml2[pml2idx] & 1))
                    continue;

                uintptr_t *pml1 = reinterpret_cast<uintptr_t *>((pml2[pml2idx] & ~PAGE_MASK) + KERNEL_BASE);
                for(uintptr_t pml1idx = 0; pml1idx < 511; ++pml1idx)
                {
                    uintptr_t va = pml4idx << 39 | pml3idx << 30 | pml2idx << 21 | pml1idx << 12;
                    if(va < startVA || va >= endVA)
                        continue;
                    if(!(pml1[pml1idx] & 1))
                        continue;
                    ++res;
                }
            }
        }
    }
    return res;
}

bool Paging::HandleCOW(uintptr_t va)
{
    AddressSpace as = GetCurrentAddressSpace();
    uintptr_t pte = getPTE(as, va);
    if(pte == PG_INVALID_ADDRESS || !(pte & PG_COW))
        return false;

    bool user = pte & PG_USER;
    uintptr_t srcPA = pte & ~PAGE_MASK;
    uintptr_t dstPA = AllocFrame();
    if(dstPA == PG_INVALID_ADDRESS)
        return false;
    Memory::Move(reinterpret_cast<void *>(dstPA + KERNEL_BASE),
                 reinterpret_cast<void *>(srcPA + KERNEL_BASE),
                 PAGE_SIZE);
    //UnMapPage(as, va); // I think this is not needed
    FreeFrame(srcPA);
    if(!MapPage(as, va, dstPA, user, true, false))
    {
        FreeFrame(dstPA);
        return false;
    }
    return true;
}

void Paging::DumpAddressSpace(AddressSpace as)
{
    bool ints = cpuDisableInterrupts();
    for(uintptr_t va = PAGE_SIZE; va; va += PAGE_SIZE)
    {
        uintptr_t pa = GetPhysicalAddress(as, va);
        if(pa == PG_INVALID_ADDRESS) continue;
        DEBUG("%p -> %p\n", va, pa);
    }
    cpuRestoreInterrupts(ints);
}
