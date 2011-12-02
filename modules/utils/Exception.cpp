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
