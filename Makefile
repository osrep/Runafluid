CXX=g++

CXXFLAGS = -pthread -g -fPIC -I$(UAL)/include -I$(UAL)/lowlevel -I$(UAL)/cppinterface/
CXXFLAGS += -I/afs/ipp-garching.mpg.de/itm/switm/blitz/0.10/include/
#CXXFLAGS += -I$(ITMLIBDIR)/itmconstants/include/

LDFLAGS = -L$(UAL)/lib -lUALCPPInterface -lUALLowLevel
LDFLAGS += -L/afs/ipp-garching.mpg.de/itm/switm/blitz/0.10/lib -lblitz

all: librunafluid.a libdistread.a test/libiotest.a test/libdistredit.a test/libefieldedit.a

#dreicer.o avalanche.o
librunafluid.a: runafluid.o  init.o  cpo_utils.o 
	ar -rvs $@ $^
	
libdistread.a: distread.o cpo_utils.o 
	ar -rvs $@ $^
	
		
test/libiotest.a: test/iotest.o

test/libdistredit.a: test/distredit.o init.o cpo_utils.o 
	ar -rvs $@ $^	
	
	
test/libefieldedit.a: test/efieldedit.o init.o cpo_utils.o 
	ar -rvs $@ $^
	
#test/test.o: test/test.cpp
#	$(CXX) -include UALClasses.h $(CXXFLAGS) -I$(ITMWORK)/gtest-1.7.0/include/ -c -o $@ $^

#test: runafluid.o init.o cpo_utils.o critical_field.o dreicer.o avalanche.o test/test.o
#	$(CXX) $(LDFLAGS) -L$(ITMWORK)/gtest-1.7.0/ -lgtest_main $^ -lgtest -o test.bin

.o: .cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
	
ual:
	$(CXX) $(CXXFLAGS) $(UAL)/include/UALClasses.h -o $(ITMWORK)/runatester/UALClasses.h.gch
	
clean:
	rm *.a *.o test/*.o test/*.a #test/*.bin
