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

MAKE_SUBDIRS := aif_proto streaming util
MAKE_TOOLS := tools
MAKE_TEST := test

CPP_FILES := $(wildcard aif_proto/*.proto  streaming/*.cpp  util/*.cpp)
OBJ_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))
OBJ_FILES := $(patsubst %.proto,%.o,$(OBJ_FILES))

# $(info $$CPP_FILES is [${CPP_FILES}])
# $(info $$OBJ_FILES is [${OBJ_FILES}])

MAKE_SUBDIRS_CLEAN := $(wildcard */.)
CLEAN_SUBDIRS = $(MAKE_SUBDIRS_CLEAN:%=clean-%)

all: $(MAKE_SUBDIRS) libaif.a $(MAKE_TOOLS) $(MAKE_TEST)

libaif.a: $(OBJ_FILES)
	ar rcs $@ $(^:%.h=)

$(MAKE_SUBDIRS):
	$(MAKE) -C $@

$(MAKE_TOOLS):
	$(MAKE) -C $@

$(MAKE_TEST):
	$(MAKE) -C $@

clean: $(CLEAN_SUBDIRS)
	@rm -f *.a

$(CLEAN_SUBDIRS):
	$(MAKE) -C $(@:clean-%=%) clean

.PHONY: all clean $(MAKE_SUBDIRS) $(MAKE_TOOLS) $(MAKE_TEST) $(MAKE_SUBDIRS_CLEAN) $(CLEAN_SUBDIRS)
