F90=ifort
COPTS = -r8 -assume no2underscore  -fPIC $(shell eval-pkg-config --cflags ual-ifort) -shared-intel
LIBS = $(shell eval-pkg-config --libs ual-ifort) -lm
INCLUDES = $(shell eval-pkg-config --cflags ual-ifort)


all: hdf5save hdf5compress

hdf5save: hdf5save.f90
	$(F90) $(COPTS) -o $@ $^ ${INCLUDES} $(LIBS)

hdf5compress: hdf5compress.f90
	$(F90) $(COPTS) -o $@ $^ ${INCLUDES} $(LIBS)
	
clean:
	rm -f *.o hdf5save

