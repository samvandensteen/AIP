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

all: knobs.o extended_uop_type_enum.o globals.o

knobs.o: knobs.cpp
	$(CXX) $(COMPILE_PIN_FLAGS) -c -o $@ $<

extended_uop_type_enum.o: extended_uop_type_enum.cpp
	$(CXX) $(COMPILE_PIN_FLAGS) -c -o $@ $<

globals.o: globals.cpp
	$(CXX) $(COMPILE_PIN_FLAGS) -c -o $@ $<

clean:
	@rm -f *.o
