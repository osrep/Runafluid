# for C++
CXX=g++
CXXFLAGS = -pthread -g -fPIC 
CXXFLAGS += $(shell eval-pkg-config --cflags ual-cpp-gnu)
CXXFLAGS += $(shell eval-pkg-config --cflags itmconstants)
LDFLAGS = $(shell eval-pkg-config --libs ual-cpp-gnu)

# for Fortran
F90=ifort
F90COPTS = -r8 -assume no2underscore  -fPIC $(shell eval-pkg-config --cflags ual-ifort) -shared-intel
F90LIBS = $(shell eval-pkg-config --libs ual-ifort) -lm
F90INCLUDES = $(shell eval-pkg-config --cflags ual-ifort)
ETSDIR = /marconi_work/eufus_gw/work/g2denka/ETS
F90ETSINCLUDES = $(shell eval-pkg-config --libs itmtypes-ifort)

# all files

ifeq ($(ITM_ENVIRONMENT_LOADED), yes)
    CXXFLAGS += $(shell eval-pkg-config --cflags ual-cpp-gnu --cflags itmconstants)
    CXXFLAGS +=-DITM_CONSTANTS
    LDFLAGS = $(shell eval-pkg-config --libs ual-cpp-gnu)    
    all:   libRunafluid.a libEfieldEdit.a
    # test developing files
    d:        libRunafluid.a libEfieldEdit.a  test/libnewdist.a test/libNewDistSlice.a test/libTeEdit.a test/libNeEdit.a test/libqeimpEdit.a test/libqeexpdecay.a test/libteexpdecay.a
    dev:      libRunafluid.a libEfieldEdit.a  test/libnewdist.a test/libNewDistSlice.a test/libTeEdit.a test/libNeEdit.a test/libqeimpEdit.a test/libqeexpdecay.a test/libteexpdecay.a
    devonly:  test/libnewdist.a test/libNewDistSlice.a test/libTeEdit.a test/libNeEdit.a test/libqeimpEdit.a test/libqeexpdecay.a test/libteexpdecay.a
    fortran:  test/libNewDistSlice.a     
    $(info *** Compiler set to ITM *** )
else
    CXXFLAGS += $(shell pkg-config --cflags imas-cpp blitz)
    LDFLAGS = $(shell pkg-config --libs imas-cpp blitz)    
    all: libRunafluid_imas.a
    d:  libRunafluid_imas.a libEfieldEdit_imas.a  test/libTeEdit_imas.a test/libNeEdit_imas.a     
    $(info *** Compiler set to IMAS *** )
endif

 
# Runafluid actor
libRunafluid.a: runafluid.o distinit.o  cpo_utils.o  critical_field.o  control.o  dreicer.o  avalanche.o
	ar -rvs $@ $^	
	
libRunafluid_imas.a: runafluid_imas.o distinit_imas.o  ids_utils.o  critical_field.o  control.o  dreicer.o  avalanche.o
	ar -rvs $@ $^	
	
libEfieldEdit.a: efieldedit.o  cpo_utils.o  critical_field.o
	ar -rvs $@ $^	
	
libEfieldEdit_imas.a: efieldedit.o  ids_utils.o  critical_field.o
	ar -rvs $@ $^	
	
# google test
test: runafluid.o distinit.o  cpo_utils.o  critical_field.o  control.o  dreicer.o  avalanche.o test/test.o
	$(CXX) $(LDFLAGS) -L$(ITMWORK)/gtest-1.7.0/ -lgtest_main $^ -lgtest -o test.bin
		
test/test.o: test/test.cpp
	$(CXX) -include UALClasses.h $(CXXFLAGS) -I$(ITMWORK)/gtest-1.7.0/include/ -c -o $@ $^




# test C++ files 		
test/libTeEdit.a: test/te_edit.o  cpo_utils.o 
	ar -rvs $@ $^
	
test/libNeEdit.a: test/ne_edit.o  cpo_utils.o 
	ar -rvs $@ $^

test/libqeimpEdit.a: test/qe_imp_edit.o  cpo_utils.o 
	ar -rvs $@ $^
	
test/libqeexpdecay.a: test/qe_expdecay.o  cpo_utils.o 
	ar -rvs $@ $^
	
test/libteexpdecay.a: test/te_expdecay.o  cpo_utils.o 
	ar -rvs $@ $^
	
	
# test C++ files 	for IMAS	
test/libTeEdit_imas.a: test/te_edit.o  ids_utils.o 
	ar -rvs $@ $^
	
test/libNeEdit_imas.a: test/ne_edit.o  ids_utils.o 
	ar -rvs $@ $^

test/libqeimpEdit_imas.a: test/qe_imp_edit.o  ids_utils.o 
	ar -rvs $@ $^
	
test/libqeexpdecay_imas.a: test/qe_expdecay.o  ids_utils.o 
	ar -rvs $@ $^
	
test/libteexpdecay_imas.a: test/te_expdecay.o  ids_utils.o 
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

	
# delete old files 	
clean:
	rm *.a *.o test/*.a test/*.o  #test/*.bin
