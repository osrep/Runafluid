 # Makefile
 #
 #  Created on: Jul 4, 2011
 #      Author: Edmondo Giovannozzi
 #     Company: ENEA
 
CXX=g++
LD=g++
CXXFLAGS=-g  -O0 -fPIC -pthread
LDFLAGS= -g -pthread

OBJECTCODE=$(ITM_INTEL_OBJECTCODE)

INCDIR2= ${shell eval-pkg-config --cflags blitz --cflags ual-cpp-gnu --cflags xmllib-2.5-$(OBJECTCODE)}
LIBSCPP2= ${shell eval-pkg-config --libs blitz hdf5-cpp-gnu ual-cpp-gnu xmllib-2.5-$(OBJECTCODE)} -L$(MDSPLUS_DIR)/lib64 -L$(MDSPLUS_DIR)/lib -lTreeShr -lTdiShr -lMdsShr -lMdsIpShr -lXTreeShr 


INCDIR= ${shell eval-pkg-config --cflags blitz --cflags ual-cpp-gnu --cflags xmllib-$(OBJECTCODE)}
LIBSCPP= ${shell eval-pkg-config --libs blitz hdf5-cpp-gnu ual-cpp-gnu xmllib-$(OBJECTCODE)} -L$(MDSPLUS_DIR)/lib64 -L$(MDSPLUS_DIR)/lib -lTreeShr -lTdiShr -lMdsShr -lMdsIpShr -lXTreeShr 
INCDIR+= -I$(ITM_XMLLIB_DIR)/$(ITM_XMLLIB_VERSION)/$(DATAVERSION)/include/


reader1 :reader1.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCDIR) $(LIBSCPP)

clean :
	rm -f *.o
	rm -f mydriver
