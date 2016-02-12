OUTPUT=$(BIN_DIR)/$(KERNEL_NAME)-multiboot.elf32

OBJECTS_CUSTOM=kernel.o multiboot.o
OBJECTS=loader.o terminal.o minilib.o

.PHONY: clean

$(OUTPUT): merge.ld $(OBJECTS) $(OBJECTS_CUSTOM)
	$(LD) $(LDFLAGS) -m elf_i386 -T $< -o $@ $(OBJECTS) $(OBJECTS_CUSTOM)

multiboot.o: multiboot.S
	$(AS) $(ASFLAGS) --32 $< -o $@

kernel.o: $(KERNEL_FILE)
	objcopy -I binary -B i386 -O elf32-i386 $< $@

%.o: %.c
	$(CC) $(CFLAGS) -m32 $< -o $@

clean:
	$(RM) $(OBJECTS) $(OBJECTS_CUSTOM)
