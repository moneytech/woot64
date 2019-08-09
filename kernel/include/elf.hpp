#pragma once

#include <types.h>

#pragma pack(push, 1)

typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf32_Word;
typedef	int32_t  Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef	int32_t  Elf64_Sword;
typedef uint64_t Elf32_Xword;
typedef	int64_t  Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef	int64_t  Elf64_Sxword;
typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

/* The ELF file header.  This appears at the start of every ELF file.  */

#define EI_NIDENT (16)

typedef struct
{
    unsigned char   e_ident[EI_NIDENT];	/* Magic number and other info */
    Elf32_Half      e_type;             /* Object file type */
    Elf32_Half      e_machine;          /* Architecture */
    Elf32_Word      e_version;          /* Object file version */
    Elf32_Addr      e_entry;            /* Entry point virtual address */
    Elf32_Off       e_phoff;            /* Program header table file offset */
    Elf32_Off       e_shoff;            /* Section header table file offset */
    Elf32_Word      e_flags;            /* Processor-specific flags */
    Elf32_Half      e_ehsize;           /* ELF header size in bytes */
    Elf32_Half      e_phentsize;        /* Program header table entry size */
    Elf32_Half      e_phnum;            /* Program header table entry count */
    Elf32_Half      e_shentsize;        /* Section header table entry size */
    Elf32_Half      e_shnum;            /* Section header table entry count */
    Elf32_Half      e_shstrndx;         /* Section header string table index */
} Elf32_Ehdr;

typedef struct
{
    unsigned char   e_ident[EI_NIDENT]; /* ELF identification */
    Elf64_Half      e_type;             /* Object file type */
    Elf64_Half      e_machine;          /* Machine type */
    Elf64_Word      e_version;          /* Object file version */
    Elf64_Addr      e_entry;            /* Entry point address */
    Elf64_Off       e_phoff;            /* Program header offset */
    Elf64_Off       e_shoff;            /* Section header offset */
    Elf64_Word      e_flags;            /* Processor-specific flags */
    Elf64_Half      e_ehsize;           /* ELF header size */
    Elf64_Half      e_phentsize;        /* Size of program header entry */
    Elf64_Half      e_phnum;            /* Number of program header entries */
    Elf64_Half      e_shentsize;        /* Size of section header entry */
    Elf64_Half      e_shnum;            /* Number of section header entries */
    Elf64_Half      e_shstrndx;         /* Section name string table index */
} Elf64_Ehdr;

typedef struct
{
    Elf32_Word	p_type;
    Elf32_Off	p_offset;
    Elf32_Addr	p_vaddr;
    Elf32_Addr	p_paddr;
    Elf32_Word	p_filesz;
    Elf32_Word	p_memsz;
    Elf32_Word	p_flags;
    Elf32_Word	p_align;
} Elf32_Phdr;

typedef struct
{
    Elf64_Word  p_type;     /* Type of segment */
    Elf64_Word  p_flags;    /* Segment attributes */
    Elf64_Off   p_offset;   /* Offset in file */
    Elf64_Addr  p_vaddr;    /* Virtual address in memory */
    Elf64_Addr  p_paddr;    /* Reserved */
    Elf64_Xword p_filesz;   /* Size of segment in file */
    Elf64_Xword p_memsz;    /* Size of segment in memory */
    Elf64_Xword p_align;    /* Alignment of segment */
} Elf64_Phdr;

typedef struct
{
    Elf32_Word	sh_name;		/* Section name (string tbl index) */
    Elf32_Word	sh_type;		/* Section type */
    Elf32_Word	sh_flags;		/* Section flags */
    Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
    Elf32_Off	sh_offset;		/* Section file offset */
    Elf32_Word	sh_size;		/* Section size in bytes */
    Elf32_Word	sh_link;		/* Link to another section */
    Elf32_Word	sh_info;		/* Additional section information */
    Elf32_Word	sh_addralign;	/* Section alignment */
    Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;

typedef struct
{
    Elf64_Word  sh_name;        /* Section name */
    Elf64_Word  sh_type;        /* Section type */
    Elf64_Xword sh_flags;       /* Section attributes */
    Elf64_Addr  sh_addr;        /* Virtual address in memory */
    Elf64_Off   sh_offset;      /* Offset in file */
    Elf64_Xword sh_size;        /* Size of section */
    Elf64_Word  sh_link;        /* Link to other section */
    Elf64_Word  sh_info;        /* Miscellaneous information */
    Elf64_Xword sh_addralign;   /* Address alignment boundary */
    Elf64_Xword sh_entsize;     /* Size of entries, if section has table */
} Elf64_Shdr;

typedef struct
{
    Elf32_Sword	d_tag;
    union
    {
        Elf32_Word	d_val;
        Elf32_Addr	d_ptr;
    } d_un;
} Elf32_Dyn;

typedef struct
{
    Elf64_Sxword    d_tag;
    union
    {
        Elf64_Xword d_val;
        Elf64_Addr  d_ptr;
    } d_un;
} Elf64_Dyn;

typedef struct
{
    Elf32_Word      st_name;    /* Symbol name (string tbl index) */
    Elf32_Addr      st_value;   /* Symbol value */
    Elf32_Word      st_size;    /* Symbol size */
    unsigned char	st_info;    /* Symbol type and binding */
    unsigned char	st_other;   /* Symbol visibility */
    Elf32_Section	st_shndx;   /* Section index */
} Elf32_Sym;

typedef struct
{
    Elf64_Word      st_name;    /* Symbol name */
    unsigned char   st_info;    /* Type and Binding attributes */
    unsigned char   st_other;   /* Reserved */
    Elf64_Half      st_shndx;   /* Section table index */
    Elf64_Addr      st_value;   /* Symbol value */
    Elf64_Xword     st_size;    /* Size of object (e.g., common) */
} Elf64_Sym;

typedef struct
{
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
} Elf32_Rel;

typedef struct
{
    Elf64_Addr  r_offset;   /* Address of reference */
    Elf64_Xword r_info;     /* Symbol index and type of relocation */
} Elf64_Rel;

typedef struct
{
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct
{
    Elf64_Addr      r_offset;   /* Address of reference */
    Elf64_Xword     r_info;     /* Symbol index and type of relocation */
    Elf64_Sxword    r_addend;   /* Constant part of expression */
} Elf64_Rela;

#define PT_NULL     0
#define PT_LOAD     1
#define PT_DYNAMIC	2
#define PT_INTERP	3
#define PT_NOTE     4
#define PT_SHLIB	5
#define PT_PHDR     6
#define PT_TLS      7
#define PT_LOOS     0x60000000
#define PT_HIOS     0x6fffffff
#define PT_LOPROC	0x70000000
#define PT_HIPROC	0x7fffffff

#define SHT_NULL            0       /* Section header table entry unused */
#define SHT_PROGBITS        1		/* Program data */
#define SHT_SYMTAB          2		/* Symbol table */
#define SHT_STRTAB          3		/* String table */
#define SHT_RELA            4		/* Relocation entries with addends */
#define SHT_HASH            5		/* Symbol hash table */
#define SHT_DYNAMIC         6		/* Dynamic linking information */
#define SHT_NOTE            7		/* Notes */
#define SHT_NOBITS          8		/* Program space with no data (bss) */
#define SHT_REL             9		/* Relocation entries, no addends */
#define SHT_SHLIB           10		/* Reserved */
#define SHT_DYNSYM          11		/* Dynamic linker symbol table */
#define SHT_INIT_ARRAY      14		/* Array of constructors */
#define SHT_FINI_ARRAY      15		/* Array of destructors */
#define SHT_PREINIT_ARRAY   16		/* Array of pre-constructors */
#define SHT_GROUP           17		/* Section group */
#define SHT_SYMTAB_SHNDX    18		/* Extended section indeces */
#define	SHT_NUM             19		/* Number of defined types.  */

/* Legal values for d_tag (dynamic entry type).  */

#define DT_NULL             0		/* Marks end of dynamic section */
#define DT_NEEDED           1		/* Name of needed library */
#define DT_PLTRELSZ         2		/* Size in bytes of PLT relocs */
#define DT_PLTGOT           3		/* Processor defined value */
#define DT_HASH             4		/* Address of symbol hash table */
#define DT_STRTAB           5		/* Address of string table */
#define DT_SYMTAB           6		/* Address of symbol table */
#define DT_RELA             7		/* Address of Rela relocs */
#define DT_RELASZ           8		/* Total size of Rela relocs */
#define DT_RELAENT          9		/* Size of one Rela reloc */
#define DT_STRSZ            10		/* Size of string table */
#define DT_SYMENT           11		/* Size of one symbol table entry */
#define DT_INIT             12		/* Address of init function */
#define DT_FINI             13		/* Address of termination function */
#define DT_SONAME           14		/* Name of shared object */
#define DT_RPATH            15		/* Library search path (deprecated) */
#define DT_SYMBOLIC         16		/* Start symbol search here */
#define DT_REL              17		/* Address of Rel relocs */
#define DT_RELSZ            18		/* Total size of Rel relocs */
#define DT_RELENT           19		/* Size of one Rel reloc */
#define DT_PLTREL           20		/* Type of reloc in PLT */
#define DT_DEBUG            21		/* For debugging; unspecified */
#define DT_TEXTREL          22		/* Reloc might modify .text */
#define DT_JMPREL           23		/* Address of PLT relocs */
#define	DT_BIND_NOW         24		/* Process relocations of object */
#define	DT_INIT_ARRAY       25		/* Array with addresses of init fct */
#define	DT_FINI_ARRAY       26		/* Array with addresses of fini fct */
#define	DT_INIT_ARRAYSZ     27		/* Size in bytes of DT_INIT_ARRAY */
#define	DT_FINI_ARRAYSZ     28		/* Size in bytes of DT_FINI_ARRAY */
#define DT_RUNPATH          29		/* Library search path */
#define DT_FLAGS            30		/* Flags for the object being loaded */
#define DT_ENCODING         32		/* Start of encoded range */
#define DT_PREINIT_ARRAY    32		/* Array with addresses of preinit fct*/
#define DT_PREINIT_ARRAYSZ  33		/* size in bytes of DT_PREINIT_ARRAY */
#define	DT_NUM              34		/* Number used */

#define ELF32_ST_BIND(i)    ((i) >> 4)
#define ELF32_ST_TYPE(i)    ((i) & 0xf)
#define ELF32_ST_INFO(b, t) (((b) << 4) + ((t) & 0xf)

#define ELF64_ST_BIND(i)    ((i) >> 4)
#define ELF64_ST_TYPE(i)    ((i) & 0xf)

#if(__SIZE_WIDTH__ == 64)
#define ELF_ST_BIND ELF64_ST_BIND
#define ELF_ST_TYPE ELF64_ST_TYPE
#elif(__SIZE_WIDTH__ == 32)
#define ELF_ST_BIND ELF32_ST_BIND
#define ELF_ST_TYPE ELF32_ST_TYPE
#endif // __SIZE_WIDTH__

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_LOPROC  13
#define STT_HIPROC  15

#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_LOPROC  13
#define STB_HIPROC  15

#define ELF32_R_SYM(info)             ((info)>>8)
#define ELF32_R_TYPE(info)            ((unsigned char)(info))
#define ELF32_R_INFO(sym, type)       (((sym)<<8)+(unsigned char)(type))

#define ELF64_R_SYM(i)      ((i) >> 32)
#define ELF64_R_TYPE(i)     ((i) & 0xffffffffL)
#define ELF64_R_INFO(s, t)  (((s) << 32) + ((t) & 0xffffffffL))

#define R_386_NONE          0		/* No reloc */
#define R_386_32            1		/* Direct 32 bit  */
#define R_386_PC32          2		/* PC relative 32 bit */
#define R_386_GOT32         3		/* 32 bit GOT entry */
#define R_386_PLT32         4		/* 32 bit PLT address */
#define R_386_COPY          5		/* Copy symbol at runtime */
#define R_386_GLOB_DAT      6		/* Create GOT entry */
#define R_386_JMP_SLOT      7		/* Create PLT entry */
#define R_386_RELATIVE      8		/* Adjust by program base */
#define R_386_GOTOFF        9		/* 32 bit offset to GOT */
#define R_386_GOTPC         10		/* 32 bit PC relative offset to GOT */
#define R_386_32PLT         11
#define R_386_TLS_TPOFF     14		/* Offset in static TLS block */
#define R_386_TLS_IE        15		/* Address of GOT entry for static TLS block offset */
#define R_386_TLS_GOTIE     16		/* GOT entry for static TLS block offset */
#define R_386_TLS_LE        17		/* Offset relative to static TLS block */
#define R_386_TLS_GD        18		/* Direct 32 bit for GNU version of general dynamic thread local data */
#define R_386_TLS_LDM       19		/* Direct 32 bit for GNU version of local dynamic thread local data in LE code */
#define R_386_16            20
#define R_386_PC16          21
#define R_386_8             22
#define R_386_PC8           23
#define R_386_TLS_GD_32     24		/* Direct 32 bit for general dynamic thread local data */
#define R_386_TLS_GD_PUSH   25		/* Tag for pushl in GD TLS code */
#define R_386_TLS_GD_CALL   26		/* Relocation for call to __tls_get_addr() */
#define R_386_TLS_GD_POP    27		/* Tag for popl in GD TLS code */
#define R_386_TLS_LDM_32    28		/* Direct 32 bit for local dynamic thread local data in LE code */
#define R_386_TLS_LDM_PUSH  29		/* Tag for pushl in LDM TLS code */
#define R_386_TLS_LDM_CALL  30		/* Relocation for call to __tls_get_addr() in LDM code */
#define R_386_TLS_LDM_POP   31		/* Tag for popl in LDM TLS code */
#define R_386_TLS_LDO_32    32		/* Offset relative to TLS block */
#define R_386_TLS_IE_32     33		/* GOT entry for negated static TLS block offset */
#define R_386_TLS_LE_32     34		/* Negated offset relative to static TLS block */
#define R_386_TLS_DTPMOD32  35		/* ID of module containing symbol */
#define R_386_TLS_DTPOFF32  36		/* Offset in TLS block */
#define R_386_TLS_TPOFF32   37		/* Negated offset in static TLS block */
#define R_386_SIZE32        38 		/* 32-bit symbol size */
#define R_386_TLS_GOTDESC   39		/* GOT offset for TLS descriptor. */
#define R_386_TLS_DESC_CALL 40		/* Marker of call through TLS descriptor for relaxation. */
#define R_386_TLS_DESC      41		/* TLS descriptor containing pointer to code and to argument, returning the TLS offset for the symbol. */
#define R_386_IRELATIVE     42		/* Adjust indirectly by program base */
#define R_386_GOT32X        43		/* Load from 32 bit GOT entry, relaxable. */
#define R_386_NUM           44      /* Keep this the last entry. */

// A Represents the addend used to compute the value of the relocatable field.
// B Represents the base address at which a shared object has been loaded into memory during execution. Generally, a shared object is built with a 0 base virtual
//   address, but the execution address will be different.
// G Represents the offset into the global offset table at which the relocation entry’s
//   symbol will reside during execution.
// GOT Represents the address of the global offset table.
// L Represents the place (section offset or address) of the Procedure Linkage Table
//   entry for a symbol.
// P Represents the place (section offset or address) of the storage unit being relocated (computed using r_offset).
// S Represents the value of the symbol whose index resides in the relocation entry

#define R_X86_64_NONE       0   // none none
#define R_X86_64_64         1   // word64 S + A
#define R_X86_64_PC32       2   // word32 S + A - P
#define R_X86_64_GOT32      3   // word32 G + A
#define R_X86_64_PLT32      4   // word32 L + A - P
#define R_X86_64_COPY       5   // none none
#define R_X86_64_GLOB_DAT   6   // word64 S
#define R_X86_64_JUMP_SLOT  7   // word64 S
#define R_X86_64_RELATIVE   8   // word64 B + A
#define R_X86_64_GOTPCREL   9   // word32 G + GOT + A - P
#define R_X86_64_32         10  // word32 S + A
#define R_X86_64_32S        11  // word32 S + A
#define R_X86_64_16         12  // word16 S + A
#define R_X86_64_PC16       13  // word16 S + A - P
#define R_X86_64_8          14  // word8 S + A
#define R_X86_64_PC8        15  // word8 S + A - P
#define R_X86_64_DPTMOD64   16  // word64
#define R_X86_64_DTPOFF64   17  // word64
#define R_X86_64_TPOFF64    18  // word64
#define R_X86_64_TLSGD      19  // word32
#define R_X86_64_TLSLD      20  // word32
#define R_X86_64_DTPOFF32   21  // word32
#define R_X86_64_GOTTPOFF   22  // word32
#define R_X86_64_TPOFF32    23  // word32

#define BFD_HAS_RELOC      0x01
#define BFD_EXEC_P         0x02
#define BFD_HAS_SYMS       0x10
#define BFD_D_PAGED        0x100

#pragma pack(pop)

#if(__SIZE_WIDTH__ == 64)
typedef Elf64_Half Elf_Half;
typedef Elf64_Word Elf_Word;
typedef Elf64_Sword Elf_Sword;
typedef Elf64_Xword Elf_Xword;
typedef Elf64_Sxword Elf_Sxword;
typedef Elf64_Addr Elf_Addr;
typedef Elf64_Off Elf_Off;
typedef Elf64_Section Elf_Section;
typedef Elf64_Versym Elf_Versym;
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Phdr Elf_Phdr;
typedef Elf64_Shdr Elf_Shdr;
typedef Elf64_Dyn Elf_Dyn;
typedef Elf64_Sym Elf_Sym;
typedef Elf64_Rel Elf_Rel;
typedef Elf64_Rela Elf_Rela;

#define ELF_R_SYM ELF64_R_SYM
#define ELF_R_TYPE ELF64_R_TYPE
#define ELF_R_INFO ELF46_R_INFO
#elif(__SIZE_WIDTH__ == 32)
typedef Elf32_Half Elf_Half;
typedef Elf32_Word Elf_Word;
typedef Elf32_Sword Elf_Sword;
typedef Elf32_Xword Elf_Xword;
typedef Elf32_Sxword Elf_Sxword;
typedef Elf32_Addr Elf_Addr;
typedef Elf32_Off Elf_Off;
typedef Elf32_Section Elf_Section;
typedef Elf32_Versym Elf_Versym;
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Phdr Elf_Phdr;
typedef Elf32_Shdr Elf_Shdr;
typedef Elf32_Dyn Elf_Dyn;
typedef Elf32_Sym Elf_Sym;
typedef Elf32_Rel Elf_Rel;
typedef Elf32_Rela Elf_Rela;

#define ELF_R_SYM ELF32_R_SYM
#define ELF_R_TYPE ELF32_R_TYPE
#define ELF_R_INFO ELF32_R_INFO
#else
#error Can compile only for 32 or 64 bit architectures
#endif // __SIZE_WIDTH__

class DEntry;
class Process;

class ELF
{
    friend class Process;

    Elf_Ehdr *ehdr;
    uint8_t *phdrData;
    uint8_t *shdrData;
    uintptr_t base;
    uintptr_t top;
    uintptr_t baseDelta;
    bool releaseData;
    Process *process;
    bool user;
    uintptr_t endPtr;
    ELF(const char *name, Elf_Ehdr *ehdr, uint8_t *phdrData, uint8_t *shdrData, bool user);
    Elf_Shdr *getShdr(int i);
public:
    char *Name;
    char *FullPath;
    int (*EntryPoint)();
    void (*_module_fini)();

    static ELF *Load(const char *filename, bool user, bool onlyHeaders, bool applyRelocs);

    Elf_Sym *FindSymbol(const char *Name);
    const char *GetSymbolName(uintptr_t addr, ptrdiff_t *delta);
    bool ApplyRelocations();
    uintptr_t GetBase() const;
    uintptr_t GetEndPtr() const;
    void *GetPHdr() const;
    Elf_Ehdr *GetEHdr() const;
    ~ELF();
};
