CC=h5pcc
LD=h5pcc

CFLAGS=-O3
LDFLAGS= -O3

EXEC=ASDF_writer

SRCDIR=src
INCDIR=inc

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

print:
	echo $(OBJ)

$(EXEC): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) -I$(INCDIR)

doc:
	doxygen Doxyfile

clean:
	rm -f *.o $(SRCDIR)/*.o

mrproper: clean
	rm -f $(EXEC)
	rm -rf doc
