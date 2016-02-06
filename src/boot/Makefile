.PHONY: all multiboot clean

all: multiboot

multiboot: multiboot/Makefile
	$(MAKE) -C multiboot KERNEL_FILE=$(KERNEL_FILE)

clean:
	$(MAKE) -C multiboot clean
