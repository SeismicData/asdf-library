CC=h5pcc
LD=h5pcc

EXEC=ASDF_writer

all: $(EXEC)

$(EXEC): write_ASDF.o ASDF_init.o ASDF_write.o
	$(LD) -o $@ $^

write_ASDF.o: write_ASDF.c
	$(CC) -c -o $@ $<

ASDF_init.o: ASDF_init.c
	$(CC) -c -o $@ $<

ASDF_write.o: ASDF_write.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
