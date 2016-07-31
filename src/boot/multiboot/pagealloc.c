#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <boot/multiboot/minilib.h>
#include <boot/multiboot/multiboot.h>
#include <boot/multiboot/terminal.h>
#include <boot/multiboot/pagealloc.h>

multiboot_memory_map_t *gpPhysMapMultiboot;
multiboot_memory_map_t *gpPhysMapMultibootStart;
uint32_t guPhysMapMultibootEnd;
uint32_t guPhysNextFree;
uint32_t *pml4e;

extern uint8_t __BOOT_IMAGE_START[];
extern uint8_t __BOOT_IMAGE_END[];

void phys_alloc_init(multiboot_memory_map_t *mmap, uint32_t mmap_lenght) {
	gpPhysMapMultibootStart = mmap;
	gpPhysMapMultiboot = mmap;
	guPhysMapMultibootEnd = ((uint32_t)gpPhysMapMultiboot) + mmap_lenght;

	guPhysNextFree = ((uint32_t) &__BOOT_IMAGE_END + PAGE_SIZE4 - 1) & (~(PAGE_SIZE4 - 1));
}

// More then 1 page allocation is poor
uint32_t phys_alloc_alloc(uint32_t count) {
	if (!count || ((uint32_t) gpPhysMapMultiboot >= guPhysMapMultibootEnd) ||
		((guPhysNextFree + count * PAGE_SIZE4) < guPhysNextFree)) // if overflow
		return 0;

	while (1) {
		if (guPhysNextFree > (uint32_t)gpPhysMapMultiboot->addr && gpPhysMapMultiboot->type == 1 &&
			(uint32_t)(guPhysNextFree + count * PAGE_SIZE4) < (uint32_t)(gpPhysMapMultiboot->addr + gpPhysMapMultiboot->len)) {
			uint32_t result = guPhysNextFree;
			guPhysNextFree += count * PAGE_SIZE4;
			return result;
		}

		gpPhysMapMultiboot = (multiboot_memory_map_t*)((uint32_t)gpPhysMapMultiboot +
			gpPhysMapMultiboot->size + sizeof(gpPhysMapMultiboot->size));

		if ((uint32_t)gpPhysMapMultiboot < guPhysMapMultibootEnd) {
			if (guPhysNextFree < (uint32_t)gpPhysMapMultiboot->addr)
				guPhysNextFree = gpPhysMapMultiboot->addr;
		} else
			break;
	}
	return 0;
}

uint32_t alloc_zeroes() {
	uint32_t p = phys_alloc_alloc(1);
	memset((void *) p, 0, PAGE_SIZE4);
	return p;
}

// Create pml4e table
bool virt_init() {
	return !!(pml4e = (uint32_t*) alloc_zeroes());
}

// Alloc 4kbyte physical page and assign to virtual address
uint32_t phys_alloc_to_virt4k(uint64_t virt_addr, bool write, bool execute) {
	uint32_t i_pml4e = (uint32_t)(virt_addr >> 39) & PAGE_MASK4;
	uint32_t i_pdp = (uint32_t)(virt_addr >> 30) & PAGE_MASK4;
	uint32_t i_pd = (uint32_t)(virt_addr >> 21) & PAGE_MASK4;
	uint32_t i_pte = (uint32_t)(virt_addr >> 12) & PAGE_MASK4;
	
#define CLEAN_BASE_ADDR(rg) ((rg) & (~((12 << 1) - 1)))
	
	uint32_t *pdp;
	if (!(pml4e[i_pml4e] & 1)) {
		uint32_t newpdp = alloc_zeroes();
		terminal_printf("Allocating new pdp: %d=> %p\n", i_pml4e, newpdp);
		pml4e[i_pml4e] = newpdp | PAGE_PRESENT;
		pdp = (uint32_t *) newpdp;
	} else {
		pdp = (uint32_t *) CLEAN_BASE_ADDR(pml4e[i_pml4e]);
	}
	
	uint32_t *pd;
	if (!(pdp[i_pdp] & 1)) {
		uint32_t newpd = alloc_zeroes();
		terminal_printf("Allocating new pd: %d=> %p\n", i_pdp, newpd);
		pdp[i_pdp] = newpd | PAGE_PRESENT;
		pd = (uint32_t *) newpd;
	} else {
		pd = (uint32_t *) CLEAN_BASE_ADDR(pdp[i_pdp]);
	}
		
	uint64_t *pte;
	if (!(pd[i_pd] & 1)) {
		uint32_t newpte = alloc_zeroes();
		terminal_printf("Allocating new pte: %d=> %p\n", i_pd, newpte);
		pd[i_pd] = newpte | PAGE_PRESENT;
		pte = (uint64_t *) newpte;
	} else {
		pte = (uint64_t *) CLEAN_BASE_ADDR(pd[i_pd]);
	}
	
	uint32_t page = alloc_zeroes();
	uint64_t pargs = PAGE_PRESENT;
	if (write)
		pargs |= PAGE_ALLOW_WRITE;

	// TODO: nx (no execute) bit
	// if (!execute)
	
	uint64_t pte_val = ((uint64_t) page) | pargs;
	pte[i_pte] = pte_val;
	
	terminal_printf("  pte[%d=>%d=>%d=>%d] = %l\n", i_pml4e, i_pdp, i_pd, i_pte, pte_val);
	
	return page;
}
