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

ifndef COMPILE_FLAGS
	COMPILE_FLAGS := -g -std=c++0x -O3 -Wall -Wno-unknown-pragmas -fno-stack-protector -fomit-frame-pointer -fno-strict-aliasing -DTARGET_IA32E -DHOST_IA32E -fPIC
endif

ifndef INCLUDE_DIR
	INCLUDE_DIR := ../../includes
endif

INCLUDES := -I../aif_proto  -I../util -I../streaming

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES = $(patsubst %.cpp,%,$(CPP_FILES))

LIBS = ../libaif.a -pthread -lprotobuf -lpthread

all: $(OBJ_FILES)

%: %.cpp
	$(CXX) -I$(INCLUDE_DIR) $(INCLUDES) $(COMPILE_FLAGS) -o $@ $< $(LIBS)

clean:
	@rm -f *_reader

.PHONY: all clean
