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

#ifndef PROTO_WRITER_H
#define PROTO_WRITER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cerrno>
#include <cstring>

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/coded_stream.h>

#include "define_includes.h"

using std::string;
using std::stringstream;
using std::ofstream;
using std::ios;
using std::ios_base;
using std::strerror;

using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::io::OstreamOutputStream;
using google::protobuf::io::GzipOutputStream;
using google::protobuf::MessageLite;

class Proto_Writer{
  private:
	  string filename;
    uint32_t file_counter;
	  bool compression;
	  ofstream ofs;
	  OstreamOutputStream* oos;
	  GzipOutputStream* gos;

    stringstream filename_counter;

    bool write_delimited_to(MessageLite *message, ZeroCopyOutputStream* rawOutput);

  public:
	  Proto_Writer(const string& filename, const bool compress);
	  Proto_Writer(const Proto_Writer&) = delete;
	  Proto_Writer& operator=(const Proto_Writer&) = delete;
	  ~Proto_Writer();

	  void write_streaming(MessageLite *message, bool write_streaming);
	  void open_file(void);
	  void close_file(void);
    void check_file_size(void);
};
#endif /*PROTO_WRITER_H*/
