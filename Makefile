CC=h5pcc
LD=h5pcc

CFLAGS=-g -O0
LDFLAGS=-g -O0

EXEC=ASDF_writer

all: $(EXEC)

$(EXEC): write_ASDF.o ASDF_init.o ASDF_write.o
	$(LD) -o $@ $^ $(LDFLAGS)

write_ASDF.o: write_ASDF.c
	$(CC) -c -o $@ $< $(CFLAGS)

ASDF_init.o: ASDF_init.c
	$(CC) -c -o $@ $< $(CFLAGS)

ASDF_write.o: ASDF_write.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
