#  This file is part of the program: Architecture Independent Profiler (AIP).
#
#  AIP is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  AIP is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with AIP.  If not, see <http://www.gnu.org/licenses/>.
#
#  Authors: Sam Van den Steen, Ghent University
#  Copyright: 2016, Ghent University

#############################################################
#																														#
#												Build rules													#
#																														#
#############################################################

# Checked by build_profiler script, so should be okay
#ifeq ($(PIN_ROOT), )
# can't start with a tab if there's no variable assigned, yay makefiles
#$(error PIN_ROOT is not set)
#endif

# Pin version 71313 in Linux shows up as 71293. Check for a bigger version than an older release
PIN_BUILD := $(shell expr `../pinversion.sh | cut -d. -f3` \> 67254)
#$(info $$PIN_BUILD is [${PIN_BUILD}])
ifeq ($(PIN_BUILD), 1)
	INCLUDES = -I$(PIN_ROOT)/extras/xed-intel64/include
	LIBS = -L$(PIN_ROOT)/extras/xed-intel64/lib
else
	INCLUDES = -I$(PIN_ROOT)/extras/xed2-intel64/include
	LIBS = -L$(PIN_ROOT)/extras/xed2-intel64/lib
endif

CPP_FILES := $(wildcard *.cc)
OBJ_FILES = $(patsubst %.cc,%.o,$(CPP_FILES))

all: libid.a

libid.a: $(OBJ_FILES)
	ar rcs $@ $(^:%.h=)

%.o: %.cc
	g++ $(COMPILE_FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	@rm -f *.o *.a
