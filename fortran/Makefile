F90=ifort
COPTS = -g -O0 -assume no2underscore -fPIC -shared-intel
LIBS =   -L$(UAL)/lib  -lUALFORTRANInterface_ifort
INCLUDES = -I$(UAL)/include/amd64_ifort

all: newdist.o libnewdist

libnewdist: newdist.o
	ar -rvs libnewdist.a newdist.o

newdist.o: newdist.f90
	$(F90) $(COPTS) -c -o $@ $^ ${INCLUDES} $(LIBS)

clean:
	rm -f *.o *.a


