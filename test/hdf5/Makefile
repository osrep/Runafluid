F90=ifort
COPTS = -g -O0 -assume no2underscore -fPIC -shared-intel
LIBS =   -L$(UAL)/lib  -lUALFORTRANInterface_ifort
INCLUDES = -I$(UAL)/include/amd64_ifort

all: readEquilibrium writeEquilibrium

readEquilibrium: readEquilibrium.f90
	$(F90) $(COPTS) -o $@ $^ ${INCLUDES} $(LIBS)


writeEquilibrium: writeEquilibrium.f90
	$(F90) $(COPTS) -o $@ $^ ${INCLUDES} $(LIBS)

clean:
	rm -f *.o readEquilibrium writeEquilibrium

