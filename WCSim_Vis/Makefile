## T.J. Corona, 2005
## based on makefile by M. Fechner, 2003

## -lNew must be included first if ROOT is to be used
## with STL containers; otherwise runtime errors occur with
## cryptic messages about failure in operator new, cf roottalk. 

# should test if Fedora Core 4 ; use 3.2 versions if FC4 unless
# you compiled ROOT with g++ 4.0 

ISFC = $(shell if [ -e /etc/fedora-release ]; then \
	cat /etc/fedora-release; \
	fi;)

CXX=g++
CC=gcc

ROOTCFLAGS   = $(shell root-config --cflags)
ROOTMAIN     = $(shell root-config --libs)
ROOTGMAIN     = $(shell root-config --glibs)
ROOTLIBS     = -L$(ROOTSYS)/lib $(ROOTMAIN) $(ROOTGMAIN) -lMinuit
CERNLIB = `cernlib pawlib graflib packlib mathlib kernlib` -lg2c -lm
WCSIMDIR = ../WCSim_WbLS
WCSIMLIB = -L$(WCSIMDIR) -lWCSimRoot
LDLIBS +=  $(JHF2KMLIB) $(ROOTLIBS)

ODIR = ./objects

$(ODIR)/%.o: %.cc
	$(CXX) $(ROOTCFLAGS) -fPIC -I$(WCSIMDIR)/include -c -o $@ $<

$(ODIR)/%.o: %.C
	$(CXX) $(ROOTCFLAGS) -fPIC -c  -o $@ $<

OBJS = $(ODIR)/MCEvent.o  $(ODIR)/WCMapDict.o $(ODIR)/WCSim_Draw.o

OBJSMAIN = 

SRC = MCEvent.cc MCEvent.h  WCSim_Draw.cc WCSim_Draw.h WCMapLinkDef.h 

SO = libWCMap.so

standard: rootcint shared

rootcint: $(SRC)
	rootcint -f WCMapDict.cc -c -I. -I$(WCSIMDIR)/include MCEvent.h WCSim_Draw.h WCMapLinkDef.h

shared: $(SRC) $(OBJS)
	$(CXX) -shared -O $(OBJS) -o $(SO) $(WCSIMLIB) $(ROOTLIBS)

clean:
	rm -f $(ODIR)/*.o *.exe *~ *.d 

realclean: clean
	rm -f *Dict.cc *Dict.h


all:   	rootcint shared 
