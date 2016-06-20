CXX=g++

CXXFLAGS = -pthread -g -fPIC 
CXXFLAGS+=$(shell eval-pkg-config --cflags ual-cpp-gnu)
LDFLAGS=$(shell eval-pkg-config --libs ual-cpp-gnu)
#CXXFLAGS += -I$(ITMLIBDIR)/itmconstants/include/


F90=ifort
F90COPTS = -g -O0 -assume no2underscore -fPIC -shared-intel
F90LIBS =   -L$(UAL)/lib  -lUALFORTRANInterface_ifort
F90INCLUDES = -I$(UAL)/include/amd64_ifort

all: libRunafluid.a libDistInit.a libSliceDist.a fortran/libnewdist.a

t: libRunafluid.a libDistInit.a  libSliceDist.a  fortran/libnewdist.a test/libEfieldEdit.a test/libTeEdit.a test/libNeEdit.a test/libDAin.a test/libDAout.a test/libCPin.a test/libMycpp.a
 
#dreicer.o avalanche.o
libRunafluid.a: runafluid.o  control.o  cpo_utils.o
	ar -rvs $@ $^	
 	
libDistInit.a: distinit.o
	ar -rvs $@ $^	
	
libSliceDist.a: slice_dist.o
	ar -rvs $@ $^		
	
test/libEfieldEdit.a: test/efieldedit.o cpo_utils.o 
	ar -rvs $@ $^
	
test/libTeEdit.a: test/te_edit.o cpo_utils.o 
	ar -rvs $@ $^
	
test/libNeEdit.a: test/ne_edit.o cpo_utils.o 
	ar -rvs $@ $^
	
test/libDAin.a: test/dain.o 
	ar -rvs $@ $^
	
test/libDAout.a: test/daout.o 
	ar -rvs $@ $^
	
	
test/libCPin.a: test/cpin.o 
	ar -rvs $@ $^
	
	
test/libMycpp.a: test/mycpp.o 
	ar -rvs $@ $^
	
#test/test.o: test/test.cpp
#	$(CXX) -include UALClasses.h $(CXXFLAGS) -I$(ITMWORK)/gtest-1.7.0/include/ -c -o $@ $^

#test: runafluid.o init.o cpo_utils.o critical_field.o dreicer.o avalanche.o test/test.o
#	$(CXX) $(LDFLAGS) -L$(ITMWORK)/gtest-1.7.0/ -lgtest_main $^ -lgtest -o test.bin

.o: .cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
	
fortran/libnewdist.a: fortran/newdist.o
	ar -rvs $@ $^

fortran/newdist.o: fortran/newdist.f90
	$(F90) $(F90COPTS) -c -o $@ $^ ${F90INCLUDES} $(F90LIBS)	
	
ual:
	$(CXX) $(CXXFLAGS) $(UAL)/include/UALClasses.h -o $(ITMWORK)/runatester/UALClasses.h.gch
	
clean:
	rm *.a *.o test/*.a test/*.o fortran/*.a fortran/*.o  #test/*.bin
