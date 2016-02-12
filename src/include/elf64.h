#ifndef ELF64_HEADER_H
#define ELF64_HEADER_H 1

typedef uint64_t Elf64_Addr_t;
typedef uint64_t Elf64_Off_t;
typedef uint16_t Elf64_Half_t;
typedef uint32_t Elf64_Word_t;
typedef int32_t Elf64_Sword_t;
typedef uint64_t Elf64_Xword_t;
typedef int64_t Elf64_Sxword_t;

typedef struct {
	unsigned char e_ident[16];
	Elf64_Half_t e_type;
	Elf64_Half_t e_machine;
	Elf64_Word_t e_version;
	Elf64_Addr_t e_entry;
	Elf64_Off_t  e_phoff;
	Elf64_Off_t  e_shoff;
	Elf64_Word_t e_flags;
	Elf64_Half_t e_ehsize;
	Elf64_Half_t e_phentsize;
	Elf64_Half_t e_phnum;
	Elf64_Half_t e_shentsize;
	Elf64_Half_t e_shnum;
	Elf64_Half_t e_shstrndx;
} Elf64_Endr_t;

typedef struct {
	Elf64_Word_t  sh_name;
	Elf64_Word_t  sh_type;
	Elf64_Xword_t sh_flags;
	Elf64_Addr_t  sh_addr;
	Elf64_Off_t   sh_offset;
	Elf64_Xword_t sh_size;
	Elf64_Word_t  sh_link;
	Elf64_Word_t  sh_info;
	Elf64_Xword_t sh_addralign;
	Elf64_Xword_t sh_entsize;
} Elf64_Shdr_t;

#define ELF64_SHT_NULL     0
#define ELF64_SHT_PROGBITS 1
#define ELF64_SHT_SYMTAB   2 
#define ELF64_SHT_STRTAB   3
#define ELF64_SHT_RELA     4
#define ELF64_SHT_HASH     5
#define ELF64_SHT_DYNAMIC  6
#define ELF64_SHT_NOTE     7
#define ELF64_SHT_NOBITS   8
#define ELF64_SHT_REL      9
#define ELF64_SHT_SHLIB    10
#define ELF64_SHT_DYNSYM   11

#define ELF64_SHF_WRITE     (1<<0)
#define ELF64_SHF_ALLOC     (1<<1)
#define ELF64_SHF_EXECINSTR (1<<2)

typedef struct {
	Elf64_Word_t  p_type;
	Elf64_Word_t  p_flags;
	Elf64_Off_t   p_offset;
	Elf64_Addr_t  p_vaddr;
	Elf64_Addr_t  p_paddr;
	Elf64_Xword_t p_filesz;
	Elf64_Xword_t p_memsz;
	Elf64_Xword_t p_align;
} Elf64_Phdr_t;

#define ELF64_PT_NULL    0
#define ELF64_PT_LOAD    1
#define ELF64_PT_DYNAMIC 2
#define ELF64_PT_INTERP  3
#define ELF64_PT_NOTE    4
#define ELF64_PT_SHLIB   5
#define ELF64_PT_PHDR    6

#define ELF64_PF_X (1<<0)
#define ELF64_PF_W (1<<1)
#define ELF64_PF_R (1<<2)

#endif
