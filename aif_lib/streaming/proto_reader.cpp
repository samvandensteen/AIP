/*
*  This file is part of the program: Architecture Independent Profiler (AIP).
*
*  AIP is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  AIP is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with AIP.  If not, see <http://www.gnu.org/licenses/>.
*
*  Authors: Sam Van den Steen, Ghent University
*           Moncef Mechri, Uppsala University
*  Copyright: 2016, Ghent University
*/

#include "proto_reader.h"

Proto_Reader::Proto_Reader(const string& filename, const bool compress) : filename(filename), compression(compress), finalized(false), ifs(filename, ios::binary){
	if (ifs.fail()){
		throw ios_base::failure("Could not open the sample file " + filename);
	}

	iis = new IstreamInputStream(&ifs);

	if(compression){
		gis = new GzipInputStream(iis);
	}
}

bool Proto_Reader::read_streaming(MessageLite *message){
	bool clean_eof;

	bool ret;
	if(compression){
		ret = read_delimited_from(gis, message, &clean_eof);
	}
	else{
		ret = read_delimited_from(iis, message, &clean_eof);
	}

	if (!ret && !clean_eof){
		throw ios_base::failure("Input error on file: " + filename);
	}

	return ret;
}

Proto_Reader::~Proto_Reader(void){
	if(compression){
		delete gis;
	}
	delete iis;
	ifs.close();
}

bool Proto_Reader::read_delimited_from(ZeroCopyInputStream* rawInput, MessageLite* message, bool* clean_eof){
	// We create a new coded stream for each message.  Don't worry, this is fast,
	// and it makes sure the 64MB total size limit is imposed per-message rather
	// than on the whole stream.  (See the CodedInputStream interface for more
	// info on this limit.)
	CodedInputStream input(rawInput);
	const int start = input.CurrentPosition();
	if (clean_eof){
		*clean_eof = false;
	}

	// Read the size.
	uint32_t size;
	if (!input.ReadVarint32(&size)){
		if (clean_eof){
			*clean_eof = input.CurrentPosition() == start;
		}
		return false;
	}
	// Tell the stream not to read beyond that size.
	CodedInputStream::Limit limit = input.PushLimit(size);

	// Parse the message.
	if (!message->MergeFromCodedStream(&input)){
		return false;
	}
	if (!input.ConsumedEntireMessage()){
		return false;
	}

	// Release the limit.
	input.PopLimit(limit);

	return true;
}
