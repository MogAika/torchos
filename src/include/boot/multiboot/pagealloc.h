#ifndef _BOOT_MULTIBOOT_PAGEALLOC_HEADER
#define _BOOT_MULTIBOOT_PAGEALLOC_HEADER 1

void phys_alloc_init(multiboot_memory_map_t *mmap, uint32_t mmap_lenght);

uint32_t phys_alloc_alloc(uint32_t count);

bool virt_init();

uint32_t phys_alloc_to_virt4k(uint64_t virt_addr, bool write, bool execute);

#define PAGE_PRESENT (1<<0)
#define PAGE_ALLOW_WRITE (1<<1)
#define PAGE_ALLOW_USER (1<<2)
#define PAGE_CACHE_WRITETHROUGH (1<<3)
#define PAGE_CACHE_DISABLE (1<<4)
#define PAGE_SIZE_BIT (1<<7)
#define PAGE_GLOBAL_BIT (1<<8)

#define PAGE_SIZE4 (1<<12)
#define PAGE_MASK4 ((1<<9)-1)



#endif
