CXX=g++
CXXFLAGS=-g -fPIC -I$(UAL)/include -I$(UAL)/lowlevel -I$(UAL)/cppinterface/ -I$SWITMDIR/blitz/blitz-0.9/include/
LDFLAGS=-L$(UAL)/lib -lUALCPPInterface -lUALLowLevel -L/afs/efda-itm.eu/project/switm/blitz/blitz-0.9/lib -lblitz

#CXXFLAGS = -pthread -g -fPIC 
#CXXFLAGS+=$(shell eval-pkg-config --cflags ual-cpp-gnu)
#LDFLAGS=$(shell eval-pkg-config --libs ual-cpp-gnu)


all: libMycpp.a 

libMycpp.a: runafluid.o  control.o  cpo_utils.o
	ar -rvs libMycpp.a mycpp.o 
	
mycpp.o: mycpp.cpp

clean:
	rm mycpp.o libMycpp.a 	
