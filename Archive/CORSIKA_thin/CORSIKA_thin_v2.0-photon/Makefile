#
# General Makefile for the OfflineUser package, modified to build
# the ADST analysis examples
#
# If the environment variable AUGEROFFLINEROOT is not set
# AND the executable 'auger-offline-config' is not in your PATH
# the definition of the following variable is required
#
# AUGEROFFLINEROOT = /remote/hillas/bdawson/ApeInstalled/offline/2.6.2-AsterixObelix
#
USER_SRCS = $(wildcard *.cc)
#
# Executable names come from the .cc sources in this directory.
# Replace the wildcard expression with .cc file list if you do
# not want to compile all .cc files in this directory
#
EXE = $(patsubst %.cc,%, $(wildcard *.cc))
#
#############################################################

## You should not need to change anything below this line ###

# Authors: T. Paul, S. Argiro, L. Nellen, D. Veberic
# $Id$
# Send bug reports to http://www.auger.unam.mx/bugzilla/

.PHONY: all depend clean

ifdef AUGEROFFLINEROOT
  AUGEROFFLINECONFIG = $(AUGEROFFLINEROOT)/bin/auger-offline-config
else
  AUGEROFFLINECONFIG = auger-offline-config
endif

CPPFLAGS    = $(shell $(AUGEROFFLINECONFIG) --cppflags)
CXXFLAGS    = $(shell $(AUGEROFFLINECONFIG) --cxxflags)
LDFLAGS     = $(shell $(AUGEROFFLINECONFIG) --ldflags)

all: $(EXE)

%: %.cc
	$(CXX) -o $@ $^ $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -lMinuit -lAnalysisKG

#############################################################
# gcc can generate the dependency list

depend: Make-depend

Make-depend: $(USER_SRCS)
	$(CPP) $(CPPFLAGS) -MM $^ > $@

clean:
	- rm -f *.o  *.so *.ps core Make-depend $(EXE)

-include Make-depend
