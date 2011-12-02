#ifndef _TT3D_UTILS_EXCEPTION_H
#define _TT3D_UTILS_EXCEPTION_H

#include <exception>
#include <string>

namespace tt3d {
namespace Utils {
 
class Exception: public std::exception {
    public:
        Exception(const std::string message);
        Exception(const char *message);
        virtual ~Exception() throw() {};
    private:
        const std::string _message;
    public:
        virtual const char *what() {
            return _message.c_str();
        }
};

class IndexError: public Exception {
    public:
        IndexError(const int given, const int min, const int max);
        IndexError(const int given);
        virtual ~IndexError() throw() {};
};

}
}

#endif
