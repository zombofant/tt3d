/**********************************************************************
File name: Stream.hpp
This file is part of: tt3d — Freeform transport simulation

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about tt3d please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/

#include <cstdint>
#include "modules/utils/Int.hpp"
#include "modules/utils/Exception.hpp"

namespace tt3d {
namespace IO {
    
using namespace tt3d::Utils;
    
class StreamError: public Exception {
    public:
        StreamError(const std::string message):
            Exception(message) {};
        StreamError(const char *message):
            Exception(message) {};
        virtual ~StreamError() throw() {};
};

class StreamNotSupportedError: public StreamError {
    public:
        StreamNotSupportedError(const std::string message):
            StreamError(message) {};
        StreamNotSupportedError(const char *message):
            StreamError(message) {};
        virtual ~StreamNotSupportedError() throw() {};
};

class StreamReadError: public StreamError {
    public:
        StreamReadError(const std::string message):
            StreamError(message) {};
        StreamReadError(const char *message):
            StreamError(message) {};
        virtual ~StreamReadError() throw() {};
};

class StreamWriteError: public StreamError {
    public:
        StreamWriteError(const std::string message):
            StreamError(message) {};
        StreamWriteError(const char *message):
            StreamError(message) {};
        virtual ~StreamWriteError() throw() {};
};

/** 
 * Class to replace the std::[io]?[f]?stream classes. The rationale for
 * that can be... uh... requested at some of the developers.
 */
class Stream {
    public:
        sizeuint read(void *data, const sizeuint length);
        virtual sizeuint read(char *data, const sizeuint length);
        virtual sizeuint seek(const int whence, const sizeint offset);
        sizeuint write(const void *data, const sizeuint length);
        virtual sizeuint write(const char *data, const sizeuint length);
    protected:
        void raiseReadError(const sizeuint read, const sizeuint required);
        void raiseWriteError(const sizeuint written, const sizeuint required);
        template <class _T> _T readInt();
        template <class _T> void writeInt(const _T value);
    public:
        int8 readInt8();
        int16 readInt16();
        int32 readInt32();
        int64 readInt64();
        std::string readString(const sizeuint length);
        uint8 readUInt8();
        uint16 readUInt16();
        uint32 readUInt32();
        uint64 readUInt64();
        
        void writeInt8(const int8 value);
        void writeInt16(const int16 value);
        void writeInt32(const int32 value);
        void writeInt64(const int64 value);
        void writeString(const std::string &value);
        void writeUInt8(const uint8 value);
        void writeUInt16(const uint16 value);
        void writeUInt32(const uint32 value);
        void writeUInt64(const uint64 value);
        
};

#include "includes/StreamOperators.hpp.inc"

}
}
