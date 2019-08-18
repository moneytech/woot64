#include <debug.hpp>
#include <elf.hpp>
#include <file.hpp>
#include <filestream.hpp>
#include <filesystem.hpp>
#include <memory.hpp>
#include <misc.hpp>
#include <paging.hpp>
#include <process.hpp>
#include <string.hpp>
#include <stringbuilder.hpp>
#include <sysdefs.h>
#include <tokenizer.hpp>

#define MIN_LOAD_ADDR   (1 << 20)

static const char *libDir = "/lib";

Mutex ELF::knownLibsLock(false, "ELF::knownLibsLock");
List<ELF::KnownLib *> ELF::knownLibs;

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

ELF::KnownLib *ELF::GetKnownLib(const char *libName)
{
    if(!libName || !libName[0])
        return nullptr;
    knownLibsLock.Acquire(10000);
    for(KnownLib *lib : knownLibs)
    {
        if(!String::Compare(libName, lib->LibName))
        {
            knownLibsLock.Release();
            return lib;
        }
    }
    knownLibsLock.Release();
    return nullptr;
}

ELF::ELF(const char *name, Elf_Ehdr *ehdr, uint8_t *phdrData, uint8_t *shdrData, bool user) :
    Name(dupBase(name)), ehdr(ehdr), phdrData(phdrData), shdrData(shdrData), user(user),
    EntryPoint((int (*)())ehdr->e_entry)
{
}

Elf_Shdr *ELF::getShdr(int i)
{
    return reinterpret_cast<Elf_Shdr *>(shdrData + i * ehdr->e_shentsize);
}

void ELF::LoadKnownLibs()
{
    DEBUG("[elf] Loading known library list\n");
    File *f = File::Open("/system/knownlibs", O_RDONLY, 0, true);
    size_t lineBufSize = MAX_PATH_LENGTH + 1 + 128;
    char *lineBuf = new char[lineBufSize];
    if(f)
    {
        FileStream fs(f);
        while(fs.ReadLine(lineBuf, static_cast<int64_t>(lineBufSize)))
        {
            String::Replace(lineBuf, '#', 0); // deal with comments
            char *trimmedLine = String::Trim(lineBuf, " \t");
            if(!trimmedLine[0])
                continue;

            Tokenizer t(trimmedLine, ":", 2);
            if(t.Tokens.Count() < 2)
            {
                DEBUG("[elf] Malformed line in know library list. Skipping.\n");
                continue;
            }

            char *libName = String::TrimEnd(t[0], " \t");
            uintptr_t libAddr = String::ToULong(t[1]);
            RegisterKnownLib(libName, libAddr);
        }
        delete f;
    }
    else DEBUG("[elf] Couldn't open known library list\n");
    delete[] lineBuf;
}

void ELF::RegisterKnownLib(char *libName, uintptr_t address)
{
    DEBUG("[elf] Registering known lib '%s' at %p\n", libName, address);
    KnownLib *lib = new KnownLib(libName, address);
    if(!lib->Image)
    {
        delete lib;
        DEBUG("[elf] Couldn't load known lib '%s'\n", libName);
        return;
    }
    knownLibsLock.Acquire(10000);
    knownLibs.Append(lib);
    knownLibsLock.Release();
}

ELF *ELF::Load(const char *filename, bool user, bool onlyHeaders, bool applyRelocs, uintptr_t loadAddr, bool skipNeeded)
{
    File *f = File::Open(filename, O_RDONLY, 0, true);
    if(!f)
    {
        DEBUG("[elf] Couldn't find '%s' file\n", filename);
        return nullptr;
    }

    // get full path name of the file
    char *fullPath = new char[MAX_PATH_LENGTH + 1];
    f->GetFullPath(fullPath, MAX_PATH_LENGTH + 1);

    ELF *elf = nullptr;
    bool usingKnownLib = false;
    KnownLib *knownLib = GetKnownLib(fullPath);
    if(knownLib)
    {
        // check if there is a free space in current address space
        bool fits = true;
        uintptr_t startVA = knownLib->Image->base;
        uintptr_t endVA = knownLib->Image->endPtr;
        for(uintptr_t va = startVA; va < endVA; va += PAGE_SIZE)
        {
            uintptr_t pa = Paging::GetPhysicalAddress(PG_CURRENT_ADDR_SPC, va);
            if(pa == PG_INVALID_ADDRESS)
                continue;
            fits = false;
            break;
        }

        if(fits)
        {
            DEBUG("[elf] Found known lib '%s' at %p\n", filename, knownLib->Address);
            elf = knownLib->CloneImage(Process::GetCurrent());
            usingKnownLib = elf != nullptr;
        }
    }

    Elf_Ehdr *ehdr = nullptr;
    uint8_t *phdrData = nullptr;
    if(!elf)
    {
        ehdr = new Elf_Ehdr;
        if(f->Read(ehdr, sizeof(Elf_Ehdr)) != sizeof(Elf_Ehdr))
        {
            DEBUG("[elf] Couldn't load ELF header\n", filename);
            delete ehdr;
            delete[] fullPath;
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
            delete[] fullPath;
            delete f;
            return nullptr;
        }
        size_t phSize = ehdr->e_phentsize * ehdr->e_phnum;
        phdrData = new uint8_t[phSize];
        if(f->Read(phdrData, phSize) != phSize)
        {
            DEBUG("[elf] Couldn't load program headers\n", filename);
            delete[] phdrData;
            delete ehdr;
            delete[] fullPath;
            delete f;
            return nullptr;
        }

        // load section headers
        if(f->Seek(ehdr->e_shoff, SEEK_SET) != ehdr->e_shoff)
        {
            DEBUG("[elf] Couldn't seek to section headers\n", filename);
            delete[] phdrData;
            delete ehdr;
            delete[] fullPath;
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
            delete[] fullPath;
            delete f;
            return nullptr;
        }

        // create ELF object itself
        elf = new ELF(filename, ehdr, phdrData, shdrData, user);
        elf->FullPath = fullPath;
    }
    else
    {
        ehdr = elf->ehdr;
        phdrData = elf->phdrData;
    }

    // attach it to the process
    Process *proc = Process::GetCurrent();
    proc->AddELF(elf);

    // calculate boundaries
    uintptr_t lowest_vaddr = __UINTPTR_MAX__;
    uintptr_t highest_vaddr = 0;
    if(!usingKnownLib)
    {
        for(int i = 0; i < ehdr->e_phnum; ++i)
        {
            Elf_Phdr *phdr = reinterpret_cast<Elf_Phdr *>(phdrData + ehdr->e_phentsize * i);
            if(phdr->p_type != PT_LOAD)
                continue;
            if(phdr->p_vaddr < lowest_vaddr)
                lowest_vaddr = phdr->p_vaddr;
            if((phdr->p_vaddr + phdr->p_memsz) > highest_vaddr)
                highest_vaddr = phdr->p_vaddr + phdr->p_memsz;
        }
        elf->base = lowest_vaddr = PAGE_SIZE * (lowest_vaddr / PAGE_SIZE);
        elf->top = highest_vaddr = align(highest_vaddr, PAGE_SIZE);
    }
    else
    {
        lowest_vaddr = elf->base;
        highest_vaddr = elf->endPtr;
    }

    if(!onlyHeaders && proc)
    {
        elf->process = proc;

        if(!usingKnownLib)
        {
            // check if image can be mapped where it wants
            bool fits = true;
            if(lowest_vaddr >= MIN_LOAD_ADDR && loadAddr == PG_INVALID_ADDRESS)
            {
                for(uintptr_t va = lowest_vaddr; va < highest_vaddr; va += PAGE_SIZE)
                {
                    uintptr_t pa = Paging::GetPhysicalAddress(proc->AddressSpace, va);
                    if(pa != PG_INVALID_ADDRESS)
                    {
                        fits = false;
                        break;
                    }
                }
            } else fits = false;
            if(!fits)
            {   // nope, we have to find some other place
                uintptr_t candidateStart = user ? max(1 << 20, lowest_vaddr) : lowest_vaddr;
                if(loadAddr != PG_INVALID_ADDRESS) candidateStart = loadAddr;

                while(candidateStart < (user ? KERNEL_BASE : 0xFFFFE000))
                {
                    uintptr_t candidateEnd = candidateStart + highest_vaddr - lowest_vaddr;
                    fits = true;
                    size_t checkedBytes = 0;
                    for(uintptr_t va = candidateStart; va < candidateEnd; va += PAGE_SIZE)
                    {
                        checkedBytes += PAGE_SIZE;
                        uintptr_t pa = Paging::GetPhysicalAddress(proc->AddressSpace, va);
                        if(pa != PG_INVALID_ADDRESS)
                        {
                            fits = false;
                            break;
                        }
                    }
                    if(!fits)
                    {
                        if(loadAddr != PG_INVALID_ADDRESS)
                        {
                            DEBUG("[elf] Couldn't load '%s' at %p\n", filename, loadAddr);
                            delete f;
                            delete[] phdrData;
                            delete ehdr;
                            delete elf;
                            return nullptr;
                        }

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
                Elf_Phdr *phdr = reinterpret_cast<Elf_Phdr *>(phdrData + ehdr->e_phentsize * i);
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
                    if(!user && pa != PG_INVALID_ADDRESS)
                    {
                        /*printf("[elf] Address conflict at %p in file '%s'\n", va, filename);
                        delete f;
                        return nullptr;*/
                        continue;
                    }
                    pa = Paging::AllocFrame();
                    if(pa == PG_INVALID_ADDRESS)
                    {
                        DEBUG("[elf] Couldn't allocate memory for data in file '%s'\n", filename);
                        delete f;
                        delete[] phdrData;
                        delete ehdr;
                        delete elf;
                        return nullptr;
                    }
                    if(!Paging::MapPage(proc->AddressSpace, va, pa, user, true, false))
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
                uint8_t *buffer = reinterpret_cast<uint8_t *>(phdr->p_vaddr + elf->baseDelta);
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
        }

        // load needed shared objects
        if(user && !skipNeeded)
        { // ignore DT_NEEDED for kernel modules for now
            // find soname if possible
            for(uint i = 0; i < ehdr->e_shnum; ++i)
            {
                Elf_Shdr *shdr = elf->getShdr(i);
                if(shdr->sh_type != SHT_DYNAMIC)
                    continue;
                uint8_t *dyntab = reinterpret_cast<uint8_t *>(shdr->sh_addr + elf->baseDelta);
                char *_strtab = reinterpret_cast<char *>(elf->getShdr(shdr->sh_link)->sh_addr + elf->baseDelta);
                for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
                {
                    Elf_Dyn *dyn = reinterpret_cast<Elf_Dyn *>(dyntab + coffs);
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
                uint8_t *dyntab = reinterpret_cast<uint8_t *>(shdr->sh_addr + elf->baseDelta);
                char *_strtab = reinterpret_cast<char *>(elf->getShdr(shdr->sh_link)->sh_addr + elf->baseDelta);
                for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
                {
                    Elf_Dyn *dyn = reinterpret_cast<Elf_Dyn *>(dyntab + coffs);
                    if(dyn->d_tag != DT_NEEDED)
                        continue;
                    char *soname = _strtab + dyn->d_un.d_val;
                    if(proc->GetELF(soname))
                        continue;
                    //DEBUG("[elf] loading DT_NEEDED %s for %s\n", soname, elf->Name);
                    StringBuilder sb(MAX_PATH_LENGTH);
                    sb.WriteFmt("%s/%s", libDir, soname);
                    ELF *soELF = Load(sb.String(), user, false, applyRelocs, PG_INVALID_ADDRESS, false);
                    (void)soELF;
                }
            }
        }
    } else delete f;

    if(!usingKnownLib)
    {
        delete[] phdrData;
        delete ehdr;

        elf->ehdr = reinterpret_cast<Elf_Ehdr *>(elf->base);
        elf->phdrData = reinterpret_cast<uint8_t *>(elf->base + elf->ehdr->e_phoff);
    }


    if(applyRelocs && !elf->ApplyRelocations())
    {
        delete elf;
        return nullptr;
    }

    // adjust entry point
    if(elf->baseDelta)
        elf->EntryPoint = reinterpret_cast<int (*)()>(reinterpret_cast<uintptr_t>(elf->EntryPoint) + elf->baseDelta);

    Elf_Sym *_module_fini_sym = elf->FindSymbol("_module_fini");
    if(_module_fini_sym)
        elf->_module_fini = reinterpret_cast<decltype(_module_fini)>(_module_fini_sym->st_value + elf->baseDelta);

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
        char *strtab = reinterpret_cast<char *>(getShdr(shdr->sh_link)->sh_addr + baseDelta);
        uint8_t *symtab = reinterpret_cast<uint8_t *>(shdr->sh_addr + baseDelta);
        for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
        {
            Elf_Sym *sym = reinterpret_cast<Elf_Sym *>(symtab + coffs);
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
        char *strtab = reinterpret_cast<char *>(getShdr(shdr->sh_link)->sh_addr + baseDelta);
        uint8_t *symtab = reinterpret_cast<uint8_t *>(shdr->sh_addr + baseDelta);
        for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
        {
            Elf_Sym *sym = reinterpret_cast<Elf_Sym *>(symtab + coffs);
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

        uint8_t *reltab = reinterpret_cast<uint8_t *>(shdr->sh_addr + baseDelta);
        Elf_Sym *_symtab = reinterpret_cast<Elf_Sym *>(getShdr(shdr->sh_link)->sh_addr + baseDelta);
        char *_strtab = reinterpret_cast<char *>(getShdr(getShdr(shdr->sh_link)->sh_link)->sh_addr + baseDelta);

        for(uint coffs = 0; coffs < shdr->sh_size; coffs += shdr->sh_entsize)
        {
            Elf_Rel *rel = reinterpret_cast<Elf_Rel *>(reltab + coffs);
            Elf_Rela *rela = reinterpret_cast<Elf_Rela *>(reltab + coffs);
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
                else if(ELF_ST_BIND(symbol->st_info) != STB_WEAK) // weak symbols don't have to be defined
                {
                    DEBUG("[elf] Couldn't find symbol '%s' for '%s'\n", name, Name);                    
                    return false;
                }
            }
            else symAddr = symbol->st_value;

            uintptr_t *val = reinterpret_cast<uintptr_t *>((isRela ? rela->r_offset : rel->r_offset) + baseDelta);

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
                Memory::Move(val, reinterpret_cast<void *>(symAddr), symbol->st_size);
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

uintptr_t ELF::GetBase() const
{
    return base;
}

uintptr_t ELF::GetBaseDelta() const
{
    return baseDelta;
}

uintptr_t ELF::GetEndPtr() const
{
    return endPtr;
}

void *ELF::GetPHdr() const
{
    return phdrData;
}

Elf_Ehdr *ELF::GetEHdr() const
{
    return ehdr;
}

ELF::~ELF()
{
    if(Name) delete[] Name;
    if(FullPath) delete[] FullPath;
    if(process)
    {
        AddressSpace as = process->AddressSpace;
        for(uintptr_t va = base; va < top; va += PAGE_SIZE)
        {
            uintptr_t pa = Paging::GetPhysicalAddress(as, va);
            Paging::UnMapPage(as, va);
            if(pa == PG_INVALID_ADDRESS)
                continue;
            Paging::FreeFrame(pa);
        }
    }
    if(shdrData) delete[] shdrData;
}

ELF::KnownLib::KnownLib() :
    LibName(nullptr), Address(0),
    Image(nullptr)
{
}

ELF::KnownLib::KnownLib(const char *libName, uintptr_t address) :
    LibName(String::Duplicate(libName)), Address(address), Image(nullptr)
{
    Image = ELF::Load(libName, true, false, false, address, true);

}

ELF *ELF::KnownLib::CloneImage(Process *dstProc)
{
    if(!dstProc) return nullptr;
    knownLibsLock.Acquire(10000);

    // clone image data
    Paging::CloneRange(dstProc->AddressSpace,
                       Paging::GetKernelAddressSpace(),
                       Image->base,
                       Image->endPtr - Image->base);

    // clone ELF structure itself
    ELF *elf = new ELF(Image->Name, reinterpret_cast<Elf_Ehdr *>(Image->base), reinterpret_cast<uint8_t *>(Image->base + Image->ehdr->e_phoff),
                       reinterpret_cast<uint8_t *>(Memory::Duplicate(Image->shdrData, Image->ehdr->e_shentsize * Image->ehdr->e_shnum)), true);
    elf->FullPath = String::Duplicate(Image->FullPath);
    elf->base = Image->base;
    elf->top = Image->top;
    elf->baseDelta = Image->baseDelta;
    elf->endPtr = Image->endPtr;

    knownLibsLock.Release();
    return elf;
}

ELF::KnownLib::~KnownLib()
{
    if(Image) delete Image;
    if(LibName) delete[] LibName;
}
