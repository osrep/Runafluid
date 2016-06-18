CXXFLAGS=-g -fPIC -I$(UAL)/include -I$(UAL)/lowlevel -I$(UAL)/cppinterface/
-I$SWITMDIR/blitz/blitz-0.9/include/
LDFLAGS=-L$(UAL)/lib -lUALCPPInterface -lUALLowLevel -L/afs/efda-
itm.eu/project/switm/blitz/blitz-0.9/lib -lblitz
libMycpp.a: mycpp.o 
      ar -rvs libMycpp.a mycpp.o 
mycpp.o: mycpp.cpp
clean:
      rm mycpp.o libMycpp.a
