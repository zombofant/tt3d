#ifndef _TT3D_UTILS_EXCEPTION_H
#define _TT3D_UTILS_EXCEPTION_H

#include <exception>
#include <string>

namespace tt3d {
namespace Utils {
 
class Exception: std::exception {
    public:
        Exception(const std::string message);
        Exception(const char *message);
        ~Exception() throw() {};
    private:
        const std::string _message;
    public:
        virtual const char *what() {
            return _message.c_str();
        }
};

}
}

#endif
