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

#ifndef PROTO_READER_H
#define PROTO_READER_H

#include <fstream>
#include <string>
#include <sstream>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/coded_stream.h>

using std::string;
using std::ifstream;
using std::ios;
using std::ios_base;

using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::IstreamInputStream;
using google::protobuf::io::GzipInputStream;
using google::protobuf::MessageLite;

class Proto_Reader{
  private:
	  string filename;
	  bool compression, finalized;
	  ifstream ifs;
	  IstreamInputStream* iis;
	  GzipInputStream* gis;

    bool read_delimited_from(ZeroCopyInputStream* rawInput, MessageLite* message, bool* clean_eof);

  public:
	  Proto_Reader(const string& filename, const bool compress);
	  Proto_Reader(const Proto_Reader&) = delete;
	  Proto_Reader& operator=(const Proto_Reader&) = delete;
	  ~Proto_Reader();

	  bool read_streaming(MessageLite *message);
};
#endif /*PROTO_READER_H*/
