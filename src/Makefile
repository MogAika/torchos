CROSS_COMPILER_PREFIX=/opt/gcc-4.6.3-nolibc/x86_64-linux/bin/x86_64-linux-

export INCLUDE_DIR=$(PWD)/include
export BIN_DIR=$(PWD)/../bin

export VERSION=0.0.1
export KERNEL_NAME=torchos-$(VERSION)

export CC=$(CROSS_COMPILER_PREFIX)gcc
export AS=$(CROSS_COMPILER_PREFIX)as
export LD=$(CROSS_COMPILER_PREFIX)ld

export EXTERNAL_DEFINITONS=-D__KERNEL_VERSION=\"$(VERSION)\" -D__KERNEL_NAME=\"$(KERNEL_NAME)\"
export CFLAGS=-std=gnu99 -c -I $(INCLUDE_DIR) -ffreestanding -nostdlib -O2 -Wall -Wextra -fno-exceptions -Qn -fdata-sections -ffunction-sections -mtune=i386 $(EXTERNAL_DEFINITONS)
export LDFLAGS=-s -nostdlib -O2 -I $(INCLUDE_DIR)
export ASFLAGS=-I $(INCLUDE_DIR)

KERNEL_FILE=$(BIN_DIR)/$(KERNEL_NAME).bin64

.PHONY: all test kernel boot qemu clean

all: kernel boot

kernel: kernel/Makefile
	$(MAKE) -C kernel OUTPUT_FILE=$(KERNEL_FILE)

boot: boot/Makefile
	$(MAKE) -C boot KERNEL_FILE=$(KERNEL_FILE)

clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C boot clean
