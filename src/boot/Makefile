CFLAGS += -mtune=i386
.PHONY: all multiboot clean

multiboot: multiboot/Makefile
	$(MAKE) -C multiboot KERNEL_FILE=$(KERNEL_FILE)

clean:
	$(MAKE) -C multiboot clean
