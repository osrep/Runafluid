# for C++
CXX=g++

CXXFLAGS = -pthread -g -fPIC 
CXXFLAGS+=$(shell eval-pkg-config --cflags ual-cpp-gnu)
LDFLAGS=$(shell eval-pkg-config --libs ual-cpp-gnu)
#CXXFLAGS += -I$(ITMLIBDIR)/itmconstants/include/

# for Fortran
F90=ifort
F90COPTS = -g -O0 -assume no2underscore -fPIC -shared-intel
F90LIBS =   -L$(UAL)/lib  -lUALFORTRANInterface_ifort
F90INCLUDES = -I$(UAL)/include/amd64_ifort

# all files
all:   libRunafluid.a libEfieldEdit.a

# test files
t:     libRunafluid.a libEfieldEdit.a  test/libnewdist.a test/libNewDistSlice.a test/libTeEdit.a test/libNeEdit.a
test:  libRunafluid.a libEfieldEdit.a  test/libnewdist.a test/libNewDistSlice.a test/libTeEdit.a test/libNeEdit.a
 
# Runafluid actor
libRunafluid.a: runafluid.o distinit.o  cpo_utils.o  critical_field.o  control.o  dreicer.o  avalanche.o
	ar -rvs $@ $^	
	
libEfieldEdit.a: efieldedit.o  cpo_utils.o  critical_field.o
	ar -rvs $@ $^	
	

# test C++ files 		
	
test/libTeEdit.a: test/te_edit.o  cpo_utils.o 
	ar -rvs $@ $^
	
test/libNeEdit.a: test/ne_edit.o  cpo_utils.o 
	ar -rvs $@ $^
	
# compile C++ files
.o: .cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^


# new distribution CPO array (Fortran)	
test/libnewdist.a: test/newdist.o
	ar -rvs $@ $^

test/newdist.o: test/newdist.f90
	$(F90) $(F90COPTS) -c -o $@ $^ ${F90INCLUDES} $(F90LIBS)	
	
	
# new distribution CPO slice (Fortran)	
test/libNewDistSlice.a: test/newdist_slice.o
	ar -rvs $@ $^

test/newdist_slice.o: test/newdist_slice.f90
	$(F90) $(F90COPTS) -c -o $@ $^ ${F90INCLUDES} $(F90LIBS)	


#test/test.o: test/test.cpp
#	$(CXX) -include UALClasses.h $(CXXFLAGS) -I$(ITMWORK)/gtest-1.7.0/include/ -c -o $@ $^

#test: runafluid.o init.o cpo_utils.o critical_field.o dreicer.o avalanche.o test/test.o
#	$(CXX) $(LDFLAGS) -L$(ITMWORK)/gtest-1.7.0/ -lgtest_main $^ -lgtest -o test.bin

	
ual:
	$(CXX) $(CXXFLAGS) $(UAL)/include/UALClasses.h -o $(ITMWORK)/runafluid/UALClasses.h.gch
	
# delete old files 	
clean:
	rm *.a *.o test/*.a test/*.o  #test/*.bin
