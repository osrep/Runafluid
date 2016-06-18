CXX=g++

CXXFLAGS = -pthread -g -fPIC 
CXXFLAGS+=$(shell eval-pkg-config --cflags ual-cpp-gnu)
LDFLAGS=$(shell eval-pkg-config --libs ual-cpp-gnu)
#CXXFLAGS += -I$(ITMLIBDIR)/itmconstants/include/

all: libRunafluid.a libDistInit.a  

test: libRunafluid.a libDistInit.a  test/libEfieldEdit.a test/libTeEdit.a test/libNeEdit.a test/libDAin.a test/libDAout.a test/libCPin.a test/libMycpp.a
 
#dreicer.o avalanche.o
libRunafluid.a: runafluid.o  control.o  cpo_utils.o
	ar -rvs $@ $^	
 	
libDistInit.a: distinit.o
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
	
ual:
	$(CXX) $(CXXFLAGS) $(UAL)/include/UALClasses.h -o $(ITMWORK)/runatester/UALClasses.h.gch
	
clean:
	rm *.a *.o test/*.o test/*.a #test/*.bin
