OUTPUT=$(OUTPUT_FILE)

OBJECTS=main.o

.PHONY: clean

$(OUTPUT): linker.ld $(OBJECTS) 
	$(LD) $(LDFLAGS) -T $< -o $@ $(OBJECTS) --nmagic 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS)
