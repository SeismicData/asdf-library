FC=h5pfc
LD=h5pfc

EXEC=writer
SRC=$(wildcard *.f90)
OBJ=$(SRC:.f90=.o)

all: $(EXEC) 

writer: $(OBJ) 
	$(LD) $^ -o $@

%.o: %.f90
	$(FC) -c $< -o $@

clean:
	rm -f *.o

mrproper: clean
	rm -f $(EXEC)
