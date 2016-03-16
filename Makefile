CXX=g++

CXXFLAGS = -pthread -g -fPIC -I$(UAL)/include -I$(UAL)/lowlevel -I$(UAL)/cppinterface/
CXXFLAGS += -I/afs/ipp-garching.mpg.de/itm/switm/blitz/0.10/include/
#CXXFLAGS += -I$(ITMLIBDIR)/itmconstants/include/

LDFLAGS = -L$(UAL)/lib -lUALCPPInterface -lUALLowLevel
LDFLAGS += -L/afs/ipp-garching.mpg.de/itm/switm/blitz/0.10/lib -lblitz

all: librunafluid.a

librunafluid.a: runafluid.o cpo_utils.o critical_field.o dreicer.o avalanche_rate.o
	ar -rvs $@ $^
	
test/test.o: test/test.cpp
	$(CXX) -include UALClasses.h $(CXXFLAGS) -I$(ITMWORK)/gtest-1.7.0/include/ -c -o $@ $^

test: runin.o cpo_utils.o critical_field.o dreicer.o avalanche_rate.o test/test.o
	$(CXX) $(LDFLAGS) -L$(ITMWORK)/gtest-1.7.0/ -lgtest_main $^ -lgtest -o test.bin

.o: .cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
	
ual:
	$(CXX) $(CXXFLAGS) $(UAL)/include/UALClasses.h -o $(ITMWORK)/runin/UALClasses.h.gch
	
clean:
	rm *.a *.o test/*.o test.bin
