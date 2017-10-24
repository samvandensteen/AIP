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

pin_version=`$PIN_ROOT/pin.sh -version`

while read -r line; do
	if [[ "$line" == Pin* ]]
	then
		pin_product_version_major=`echo $line | cut -d'.' -f 1 | cut -d' ' -f 2`
		pin_product_version_minor=`echo $line | cut -d'.' -f 2`
	elif  [[ "$line" == @CHARM-VERSION:* ]]
	then
		pin_build_number=`echo $line | cut -d' ' -f 3`
	fi
done <<< "$pin_version"

echo "$pin_product_version_major.$pin_product_version_minor.$pin_build_number"
