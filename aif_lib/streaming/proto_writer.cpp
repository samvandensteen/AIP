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

#include "proto_writer.h"

Proto_Writer::Proto_Writer(const string& filename, const bool compress) : filename(filename), file_counter(0), compression(compress){
  open_file();
}

void Proto_Writer::open_file(){
  filename_counter.str("");
  filename_counter << filename << "." << file_counter;
  ofs.open(filename_counter.str(), ios::binary);
	if(ofs.fail()){
		throw ios_base::failure("Could not open the sample file " + filename_counter.str());
	}

	oos = new OstreamOutputStream(&ofs);

	if(compression){
		// use default options except for the compression level
		GzipOutputStream::Options opts = GzipOutputStream::Options();
		// put the compression level at the lowest, higher levels take more time and even generate slightly bigger files (bigger dictionary?)
		opts.compression_level = 1;
		gos = new GzipOutputStream(oos, opts);
	}
}

void Proto_Writer::check_file_size(){
  if(ofs.tellp() > MAX_FILE_SIZE){
    close_file();
    open_file();
  }
}

void Proto_Writer::write_streaming(MessageLite *message, bool check_size){
	bool ret;
	if(compression){
		ret = write_delimited_to(message, gos);
    if(check_size){
      check_file_size();
    }
	}
	else{
		ret = write_delimited_to(message, oos);
    if(check_size){
      check_file_size();
    }
	}
	if(!ret){
		throw ios_base::failure("Output error on file: " + filename);
	}
}

void Proto_Writer::close_file(void){
  if(compression){
  	delete gos;
	}
	delete oos;
	ofs.close();
  file_counter++;
}

Proto_Writer::~Proto_Writer(void){
  if(ofs.is_open()){
    close_file();
  }
}

bool Proto_Writer::write_delimited_to(MessageLite *message, ZeroCopyOutputStream* rawOutput){
	// We create a new coded stream for each message.  Don't worry, this is fast.
	CodedOutputStream output(rawOutput);

	// Write the size.
	const int size = message->ByteSize();
	output.WriteVarint32(size);

	uint8_t* buffer = output.GetDirectBufferForNBytesAndAdvance(size);
	if (buffer != NULL){
		// Optimization:  The message fits in one buffer, so use the faster
		// direct-to-array serialization path.
		message->SerializeWithCachedSizesToArray(buffer);
	}
	else{
		// Slightly-slower path when the message is multiple buffers.
		message->SerializeWithCachedSizes(&output);
		if (output.HadError()){
			return false;
		}
	}

	return true;
}
