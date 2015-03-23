CC=h5pcc
LD=h5pcc
AR=ar
RANLIB=ranlib

CFLAGS=-O3
LDFLAGS= -O3

EXEC=ASDF_writer

SRCDIR=src
INCDIR=inc
LIBDIR=lib

LIB_SRC=$(wildcard $(SRCDIR)/ASDF*.c)
LIB_OBJ=$(LIB_SRC:.c=.o)

ASDF_LIB=${LIBDIR}/libasdf.a

all: $(EXEC)

print:
	echo $(OBJ)

$(EXEC): ${SRCDIR}/write_ASDF.o ${ASDF_LIB}
	$(LD) -o $@ $^ ${LDFLAGS} 

${ASDF_LIB}: ${LIB_OBJ}
	mkdir -p ${LIBDIR}
	${AR} cr ${ASDF_LIB} $^
	$(RANLIB) ${ASDF_LIB}

%.o: %.c
	$(CC) -c -o $@ $< ${CFLAGS} -I${INCDIR}

doc:
	doxygen Doxyfile

clean:
	rm -f *.o $(SRCDIR)/*.o

mrproper: clean
	rm -f $(EXEC)
	rm -f ${LIBDIR}/*
	rm -rf doc
