/**********************************************************************
File name: Exception.cpp
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
#include "Exception.hpp"
#include <boost/format.hpp>

namespace tt3d {
namespace Utils {
    
/* tt3d::Utils::Exception */

Exception::Exception(const std::string message):
    _message(message)
{
    
}

Exception::Exception(const char *message):
    _message(message)
{
    
}

/* tt3d::Utils::IndexError */

IndexError::IndexError(const int given, const int min, const int max):
    Exception::Exception((boost::format("Index (%d) out of range (%d..%d).") % given % min % max).str())
{
    
}

IndexError::IndexError(const int given):
    Exception::Exception((boost::format("Index (%d) out of range.") % given).str())
{
    
}

}
}
