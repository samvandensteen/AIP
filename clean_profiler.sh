#!/bin/bash

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

PROFILER_DIR=$(dirname $(readlink -f "${BASH_SOURCE[0]}"))
cd "${PROFILER_DIR}"


#####################################
#      Clean all object files       #
#####################################
make -C branch clean
make -C includes clean
make -C instruction_decoder clean
make -C aif_lib clean
make -C statstack clean
make -C utrace clean

#####################################

#####################################
#          Remove pintool           #
#####################################
cd "${PROFILER_DIR}"
rm -f -r obj-intel64

#####################################
