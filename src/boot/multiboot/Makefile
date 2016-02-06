OUTPUT=$(BIN_DIR)/$(KERNEL_NAME)-multiboot.elf32
OBJECTS=loader.o multiboot.o kernel.o terminal.o

.PHONY: clean

$(OUTPUT): merge.ld $(OBJECTS)
	$(LD) $(LDFLAGS) -m elf_i386 -T $< -o $@ $(OBJECTS)

multiboot.o: multiboot.S
	$(AS) $(ASFLAGS) --32 $< -o $@

loader.o: loader.c $(INCLUDE_DIR)/boot/multiboot/multiboot.h
	$(CC) $(CFLAGS) -m32 $< -o $@

terminal.o: terminal.c $(INCLUDE_DIR)/boot/multiboot/terminal.h
	$(CC) $(CFLAGS) -m32 $< -o $@

kernel.o: $(KERNEL_FILE)
	objcopy -I binary -B i386 -O elf32-i386 $< $@

clean:
	$(RM) $(OBJECTS)
