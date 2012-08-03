/**********************************************************************
File name: StdStream.hpp
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

#ifndef _TT3D_IO_STDSTREAM_H
#define _TT3D_IO_STDSTREAM_H

#include <iostream>
#include "Stream.hpp"
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<std::istream> IStreamHandle;
typedef boost::shared_ptr<std::ostream> OStreamHandle;
typedef boost::shared_ptr<std::iostream> IOStreamHandle;

/**
 * Stream class to wrap std::istream instances. Will give read and seek
 * access and deny write access.
 */
class IStreamWrapper: public Stream {
    public:
        IStreamWrapper(std::istream &stream);
        IStreamWrapper(IStreamHandle stream);
    private:
        std::istream &_stream;
        IStreamHandle _streamHandle;
};

/**
 * Stream class to wrap std::ostream instances. Will give write and seek
 * access and deny read access.
 */
class OStreamWrapper: public Stream {
    public:
        OStreamWrapper(std::ostream &stream);
        OStreamWrapper(OStreamHandle stream);
    private:
        std::ostream &_stream;
        OStreamHandle _streamHandle;
};

/**
 * Stream class to wrap std::iostream instances. Will give full access
 * to read, write and seek functionality.
 */
class IOStreamWrapper: public Stream {
    public:
        IOStreamWrapper(std::iostream &stream);
        IOStreamWrapper(IOStreamHandle stream);
    private:
        std::iostream &_stream;
        IOStreamHandle _streamHandle;
};

#endif
