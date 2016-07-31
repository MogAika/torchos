#ifndef _BOOT_KERNEL_INPUT_HEADER
#define _BOOT_KERNEL_INPUT_HEADER 1

typedef struct {
	uint64_t size; // first bit - type, if 1, then can use, else cannot
} kernel_input_phys_region_t;

typedef struct {
	uint64_t phys_memory_map_virt;
	uint64_t total_memory_bytes;
	uint32_t phys_regions_count;
	uint32_t page_table_place_phys;
	uint64_t page_table_place_virt;
} kernel_input_t;

#endif
