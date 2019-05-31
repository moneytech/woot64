#include <debug.hpp>
#include <elf.hpp>
#include <file.hpp>
#include <filesystem.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <paging.hpp>
#include <process.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <sysdefs.h>

static const char *libDir = "WOOT_OS~/lib";

static char *dupBase(const char *name)
{
    char *basename = nullptr;
    int sepLen = String::Length(PATH_SEPARATORS);
    for(int i = 0; i < sepLen; ++i)
    {
        basename = String::Find(name, PATH_SEPARATORS[i], true);
        if(basename) break;
    }
    if(!basename) return String::Duplicate(name);
    return String::Duplicate(basename + 1);
}

ELF::ELF(const char *name, Elf_Ehdr *ehdr, uint8_t *phdrData, uint8_t *shdrData, bool user) :
    Name(dupBase(name)), ehdr(ehdr), phdrData(phdrData), shdrData(shdrData), user(user),
    EntryPoint((int (*)())ehdr->e_entry)
{
}

Elf_Shdr *ELF::getShdr(int i)
{
    return (Elf_Shdr *)(shdrData + i * ehdr->e_shentsize);
}

ELF *ELF::Load(const char *filename, bool user, bool onlyHeaders, bool applyRelocs)
{
    File *f = File::Open(filename, O_RDONLY, 0);
    if(!f)
    {
        DEBUG("[elf] Couldn't find '%s' file\n", filename);
        return nullptr;
    }
    Elf_Ehdr *ehdr = new Elf_Ehdr;
    if(f->Read(ehdr, sizeof(Elf_Ehdr)) != sizeof(Elf_Ehdr))
    {
        DEBUG("[elf] Couldn't load ELF header\n", filename);
        delete ehdr;
        delete f;
        return nullptr;
    }
    if(ehdr->e_ident[0] != 127 || ehdr->e_ident[1] != 'E' || ehdr->e_ident[2] != 'L' || ehdr->e_ident[3] != 'F')
    {
        DEBUG("[elf] Invalid ELF header magic\n", filename);
        delete ehdr;
        delete f;
        return nullptr;
    }
    // load program headers
    if(f->Seek(ehdr->e_phoff, SEEK_SET) != ehdr->e_phoff)
    {
        DEBUG("[elf] Couldn't seek to program headers\n", filename);
        delete ehdr;
        delete f;
        return nullptr;
    }
    size_t phSize = ehdr->e_phentsize * ehdr->e_phnum;
    uint8_t *phdrData = new uint8_t[phSize];
    if(f->Read(phdrData, phSize) != phSize)
    {
        DEBUG("[elf] Couldn't load program headers\n", filename);
        delete[] phdrData;
        delete ehdr;
        delete f;
        return nullptr;
    }

    // load section headers
    if(f->Seek(ehdr->e_shoff, SEEK_SET) != ehdr->e_shoff)
    {
        DEBUG("[elf] Couldn't seek to section headers\n", filename);
        delete[] phdrData;
        delete ehdr;
        delete f;
        return nullptr;
    }
    size_t shSize = ehdr->e_shentsize * ehdr->e_shnum;
    uint8_t *shdrData = new uint8_t[shSize];
    if(f->Read(shdrData, shSize) != shSize)
    {
        DEBUG("[elf] Couldn't load section headers\n", filename);
        delete[] shdrData;
        delete[] phdrData;
        delete ehdr;
        delete f;
        return nullptr;
    }

    ELF *elf = new ELF(filename, ehdr, phdrData, shdrData, user);
    Process *proc = Process::GetCurrent();
    proc->AddELF(elf);

    // calculate boundaries
    uintptr_t lowest_vaddr = ~0;
    uintptr_t highest_vaddr = 0;
    for(int i = 0; i < ehdr->e_phnum; ++i)
    {
        Elf_Phdr *phdr = (Elf_Phdr *)(phdrData + ehdr->e_phentsize * i);
        if(phdr->p_type != PT_LOAD)
            continue;
        if(phdr->p_vaddr < lowest_vaddr)
            lowest_vaddr = phdr->p_vaddr;
        if((phdr->p_vaddr + phdr->p_memsz) > highest_vaddr)
            highest_vaddr = phdr->p_vaddr + phdr->p_memsz;
    }
    elf->base = lowest_vaddr = PAGE_SIZE * (lowest_vaddr / PAGE_SIZE);
    elf->top = highest_vaddr = align(highest_vaddr, PAGE_SIZE);

    //DEBUG("%s la: %p ha %p\n", elf->Name, lowest_vaddr, highest_vaddr);

    if(!onlyHeaders && proc)
    {
        elf->process = proc;
        elf->releaseData = true;

        // check if image can be mapped where it wants
        bool fits = true;
        if(lowest_vaddr >= (1 << 20))
        {
            for(uintptr_t va = lowest_vaddr; va < highest_vaddr; va += PAGE_SIZE)
            {
                uintptr_t pa = Paging::GetPhysicalAddress(proc->AddressSpace, va);
                if(pa != ~0)
                {
                    fits = false;
                    break;
                }
            }
        } else fits = false;
        if(!fits)
        {   // nope, we have to find some other place
            uintptr_t candidateStart = user ? max(1 << 20, lowest_vaddr) : lowest_vaddr;

            while(candidateStart < (user ? KERNEL_BASE : 0xFFFFE000))
            {
                uintptr_t candidateEnd = candidateStart + highest_vaddr - lowest_vaddr;
                fits = true;
                size_t checkedBytes = 0;
                for(uintptr_t va = candidateStart; va < candidateEnd; va += PAGE_SIZE)
                {
                    checkedBytes += PAGE_SIZE;
                    uintptr_t pa = Paging::GetPhysicalAddress(proc->AddressSpace, va);
                    if(pa != ~0)
                    {
                        fits = false;
                        break;
                    }
                }
                if(!fits)
                {
                    candidateStart += checkedBytes;
                    continue;
                } else break;
            }

            elf->baseDelta = candidateStart - lowest_vaddr;
        }
        elf->base += elf->baseDelta;
        elf->top += elf->baseDelta;
        DEBUG("[elf] Loading '%s' at %p\n", elf->Name, elf->base);

        // load the data
        for(uint i = 0; i < ehdr->e_phnum; ++i)
        {
            Elf_Phdr *phdr = (Elf_Phdr *)(phdrData + ehdr->e_phentsize * i);
            if(phdr->p_type != PT_LOAD)
                continue;
            if(f->Seek(phdr->p_offset, SEEK_SET) != phdr->p_offset)
            {
                DEBUG("[elf] Couldn't seek to data of program header %d in file '%s'\n", i, filename);
                delete f;
                delete[] phdrData;
                delete ehdr;
                delete elf;
                return nullptr;
            }

            uintptr_t endva = phdr->p_vaddr + phdr->p_memsz;
            uintptr_t s = phdr->p_vaddr / PAGE_SIZE;
            uintptr_t e = align(endva, PAGE_SIZE) / PAGE_SIZE;
            size_t pageCount = e - s;
            for(uint i = 0; i < pageCount; ++i)
            {
                uintptr_t va = elf->baseDelta + phdr->p_vaddr + i * PAGE_SIZE;
                if(user && va >= KERNEL_BASE)
                {   // user elf can't map any kernel memory
                    DEBUG("[elf] Invalid user address %p in file '%s'\n", va, filename);
                    delete f;
                    delete[] phdrData;
                    delete ehdr;
                    delete elf;
                    return nullptr;
                }
                uintptr_t pa = Paging::GetPhysicalAddress(proc->AddressSpace, va);
                if(!user && pa != ~0)
                {
                    /*printf("[elf] Address conflict at %p in file '%s'\n", va, filename);
                    delete f;
                    return nullptr;*/
                    continue;
                }
                pa = Paging::AllocFrame();
                if(pa == ~0)
                {
                    DEBUG("[elf] Couldn't allocate memory for data in file '%s'\n", filename);
                    delete f;
                    delete[] phdrData;
                    delete ehdr;
                    delete elf;
                    return nullptr;
                }
                if(!Paging::MapPage(proc->AddressSpace, va, pa, user, true))
                {
                    DEBUG("[elf] Couldn't map memory for data in file '%s'\n", filename);
                    delete f;
                    delete[] phdrData;
                    delete ehdr;
                    delete elf;
                    return nullptr;
                }
                elf->endPtr = max(elf->endPtr, va + PAGE_SIZE);
            }
            uint8_t *buffer = (uint8_t *)(phdr->p_vaddr + elf->baseDelta);
            Memory::Zero(buffer, phdr->p_memsz);
            if(f->Read(buffer, phdr->p_filesz) != phdr->p_filesz)
            {
                DEBUG("[elf] Couldn't read data of program header %d in file '%s'\n", i, filename);
                delete f;
                delete[] phdrData;
                delete ehdr;
                delete elf;
                return nullptr;
            }
        }
        delete f;

        // load needed shared objects
        if(user)
        { // ignore DT_NEEDED for kernel modules for now
            // find soname if possible
            for(uint i = 0; i < ehdr->e_shnum; ++i)
            {
                Elf_Shdr *shdr = elf->getShdr(i);
                if(shdr->sh_type != SHT_DYNAMIC)
                    continue;
                uint8_t *dyntab = (uint8_t *)(shdr->sh_addr + elf->baseDelta);
                char *_strtab = (char *)(elf->getShdr(shdr->sh_link)->sh_addr + elf->baseDelta);
                for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
                {
                    Elf_Dyn *dyn = (Elf_Dyn *)(dyntab + coffs);
                    if(dyn->d_tag != DT_SONAME)
                        continue;
                    if(elf->Name) delete[] elf->Name;
                    char *soname = _strtab + dyn->d_un.d_val;
                    elf->Name = String::Duplicate(soname);
                }
            }

            for(uint i = 0; i < ehdr->e_shnum; ++i)
            {
                Elf_Shdr *shdr = elf->getShdr(i);
                if(shdr->sh_type != SHT_DYNAMIC)
                    continue;
                uint8_t *dyntab = (uint8_t *)(shdr->sh_addr + elf->baseDelta);
                char *_strtab = (char *)(elf->getShdr(shdr->sh_link)->sh_addr + elf->baseDelta);
                for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
                {
                    Elf_Dyn *dyn = (Elf_Dyn *)(dyntab + coffs);
                    if(dyn->d_tag != DT_NEEDED)
                        continue;
                    char *soname = _strtab + dyn->d_un.d_val;
                    if(proc->GetELF(soname))
                        continue;
                    //DEBUG("[elf] loading DT_NEEDED %s for %s\n", soname, elf->Name);
                    StringBuilder sb(MAX_PATH_LENGTH);
                    sb.WriteFmt("%s/%s", libDir, soname);
                    ELF *soELF = Load(sb.String(), user, false, applyRelocs);
                }
            }
        }
    } else delete f;

    delete[] phdrData;
    delete ehdr;

    elf->ehdr = (Elf_Ehdr *)elf->base;
    elf->phdrData = (uint8_t *)(elf->base + elf->ehdr->e_phoff);

    if(applyRelocs && !elf->ApplyRelocations())
    {
        delete elf;
        return nullptr;
    }

    // adjust entry point
    if(elf->baseDelta)
        elf->EntryPoint = (int (*)())((uint8_t *)elf->EntryPoint + elf->baseDelta);

    Elf_Sym *_module_fini_sym = elf->FindSymbol("_module_fini");
    if(_module_fini_sym)
        elf->_module_fini = (decltype(_module_fini))(_module_fini_sym->st_value + elf->baseDelta);

    return elf;
}

Elf_Sym *ELF::FindSymbol(const char *name)
{
    for(uint i = 0; i < ehdr->e_shnum; ++i)
    {
        Elf_Shdr *shdr = getShdr(i);
        if(shdr->sh_type != SHT_DYNSYM)
            continue;
        if(!shdr->sh_addr)
            continue;
        char *strtab = (char *)(getShdr(shdr->sh_link)->sh_addr + baseDelta);
        uint8_t *symtab = (uint8_t *)(shdr->sh_addr + baseDelta);
        for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
        {
            Elf_Sym *sym = (Elf_Sym *)(symtab + coffs);
            //int type = ELF_ST_TYPE(sym->st_info);
            if(!sym->st_shndx || !sym->st_name)
                continue;
            if(!String::Compare(name, strtab + sym->st_name))
                return sym;
        }
    }
    return nullptr;
}

const char *ELF::GetSymbolName(uintptr_t addr, ptrdiff_t *delta)
{
    if(addr < base || addr >= top)
        return nullptr;

    ptrdiff_t minDelta = __PTRDIFF_MAX__;
    const char *minSymName = nullptr;
    for(uint i = 0; i < ehdr->e_shnum; ++i)
    {
        Elf_Shdr *shdr = getShdr(i);
        if(shdr->sh_type != SHT_DYNSYM && shdr->sh_type != SHT_SYMTAB)
            continue;
        if(!shdr->sh_addr)
            continue;
        char *strtab = (char *)(getShdr(shdr->sh_link)->sh_addr + baseDelta);
        uint8_t *symtab = (uint8_t *)(shdr->sh_addr + baseDelta);
        for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
        {
            Elf_Sym *sym = (Elf_Sym *)(symtab + coffs);
            if(!sym->st_shndx || !sym->st_name)
                continue;
            const char *symName = strtab + sym->st_name;
            ptrdiff_t delta = addr - (sym->st_value + baseDelta);
            if(delta >= 0 && delta < minDelta)
            {
                minDelta = delta;
                minSymName = symName;
                if(!minDelta)
                    break;
            }
        }
    }
    if(delta) *delta = minDelta;
    return minSymName;
}

bool ELF::ApplyRelocations()
{
    for(uint i = 0; i < ehdr->e_shnum; ++i)
    {
        Elf_Shdr *shdr = getShdr(i);
        if(shdr->sh_type != SHT_REL && shdr->sh_type != SHT_RELA)
            continue;
        bool isRela = shdr->sh_type == SHT_RELA;

        uint8_t *reltab = (uint8_t *)(shdr->sh_addr + baseDelta);
        Elf_Sym *_symtab = (Elf_Sym *)(getShdr(shdr->sh_link)->sh_addr + baseDelta);
        char *_strtab = (char *)(getShdr(getShdr(shdr->sh_link)->sh_link)->sh_addr + baseDelta);

        for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
        {
            Elf_Rel *rel = (Elf_Rel *)(reltab + coffs);
            Elf_Rela *rela = (Elf_Rela *)(reltab + coffs);
            uint symIdx = isRela ? ELF_R_SYM(rela->r_info) : ELF_R_SYM(rel->r_info);
            uint rType = isRela ? ELF_R_TYPE(rela->r_info) : ELF_R_TYPE(rel->r_info);
            Elf_Sym *symbol = _symtab + symIdx;
            Elf_Sym *fSymbol = nullptr;
            uintptr_t A = isRela ? rela->r_addend : 0;

            uintptr_t symAddr = 0;
            char *name = _strtab + symbol->st_name;
            ELF *fElf = nullptr;
            if(symIdx && name[0])
            {
                fSymbol = process->FindSymbol(name, this, &fElf);
                if(!fSymbol)
                {
                    fSymbol = FindSymbol(name);
                    if(fSymbol) fElf = this;
                }
                if(fSymbol)
                    symAddr = fSymbol->st_value + (fElf ? fElf->baseDelta : 0);
                else
                {
                    DEBUG("[elf] Couldn't find symbol '%s' for '%s'\n", name, Name);
                    return false;
                }
            }
            else symAddr = symbol->st_value;

            uintptr_t *val = (uintptr_t *)((isRela ? rela->r_offset : rel->r_offset) + baseDelta);

            //printf("%s: rel: %d ", Name, rType);
            //printf("sym: %s S: %.8x A: %.8x P: %.8x\n", symbol->st_name ? name : "<no symbol>", S, A, P);
            //uintptr_t prevVal = *val;

#ifdef __i386__
            switch(rType)
            {
            case R_386_NONE:
                break;
            case R_386_32:
                *val += symAddr;
                break;
            case R_386_PC32:
                *val += symAddr - (uintptr_t)val;
                break;
            case R_386_COPY:
                Memory::Move(val, (void *)symAddr, symbol->st_size);
                break;
            case R_386_GLOB_DAT:
            case R_386_JMP_SLOT:
                *val = symAddr;
                break;
            case R_386_RELATIVE:
                *val += baseDelta;
                break;
            default:
                DEBUG("[elf] Unsupported relocation type: %d in '%s'\n", rType, Name);
                return false;
            }
#endif // __i386__
#ifdef __amd64__
            switch(rType)
            {
            case R_X86_64_NONE:
                break;
            case R_X86_64_64:
                *val = symAddr + A;
                break;
            case R_X86_64_COPY:
                Memory::Move(val, (void *)symAddr, symbol->st_size);
                break;
            case R_X86_64_GLOB_DAT:
            case R_X86_64_JUMP_SLOT:
                *val = symAddr + A;
                break;
            case R_X86_64_RELATIVE:
                *val = baseDelta + A;
                break;
            default:
                DEBUG("[elf] Unsupported relocation type: %d in '%s'\n", rType, Name);
                return false;
            }
#endif // __amd64__
            //printf("%-16s  %p ->(%d)-> %p\n", name, prevVal, rType, *val);
        }
    }
    return true;
}

uintptr_t ELF::GetBase()
{
    return base;
}

uintptr_t ELF::GetEndPtr()
{
    return endPtr;
}

ELF::~ELF()
{
    if(Name) delete[] Name;
    if(process)
    {
        AddressSpace as = process->AddressSpace;
        for(uintptr_t va = base; va < top; va += PAGE_SIZE)
        {
            uintptr_t pa = Paging::GetPhysicalAddress(as, va);
            Paging::UnMapPage(as, va);
            if(pa == ~0) continue;
            Paging::FreeFrame(pa);
        }
    }
    if(shdrData) delete[] shdrData;
}
