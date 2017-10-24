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

if [ "$PIN_ROOT" == "" ]; then
	echo "Warning: PIN_ROOT not set!"
	echo -e "\tA permanent solution is to enter this in sudoers file (sudo visudo)"
	echo -e "\tDefaults env_keep +=\"PIN_ROOT\""
	echo "For a one time run, specify PIN_ROOT then press [ENTER]:"
	read pin_root
fi

set -e

# Build the pin tool
if [ "$pin_root" != "" ]; then
	if [ "$1" == "debug" ]; then
		make PIN_ROOT=$pin_root debug
	else
		make PIN_ROOT=$pin_root
	fi
else
	if [ "$1" == "debug" ]; then
		make debug
	else
		make
	fi
fi
