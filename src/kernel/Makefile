OUTPUT=$(OUTPUT_FILE)

OBJECTS_CUSTOM=main.o
OBJECTS=torchos.o

.PHONY: clean

$(OUTPUT): linker.ld $(OBJECTS) $(OBJECTS_CUSTOM)
	$(LD) $(LDFLAGS) -T $< -o $@ $(OBJECTS) $(OBJECTS_CUSTOM) --nmagic 

main.o: main.S
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(OBJECTS_CUSTOM)
