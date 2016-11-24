F90=ifort
COPTS = -g -O0 -assume no2underscore -fPIC -shared-intel
LIBS =   -L$(UAL)/lib  -lUALFORTRANInterface_ifort
INCLUDES = -I$(UAL)/include/amd64_ifort

all: hdf5save

hdf5save: hdf5save.f90
	$(F90) $(COPTS) -o $@ $^ ${INCLUDES} $(LIBS)

clean:
	rm -f *.o hdf5save

