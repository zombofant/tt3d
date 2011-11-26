#include "Exception.hpp"

namespace tt3d {
namespace Utils {

Exception::Exception(const std::string message):
    _message(message)
{
    
}

Exception::Exception(const char *message):
    _message(message)
{
    
}

}
}
