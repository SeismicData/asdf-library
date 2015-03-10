FC=h5pfc
CC=h5pcc
LD=h5pfc

FCFLAGS=-g -O0
LDFLAGS=-g -O0

EXEC=writer
SRC=$(wildcard *.f90)
OBJ=$(SRC:.f90=.o)

all: $(EXEC) 

writer: $(OBJ) 
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.f90
	$(FC) -c -o $@ $< $(FCFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
