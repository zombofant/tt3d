#ifndef _TT3D_UTILS_BUFFER_MAP_H
#define _TT3D_UTILS_BUFFER_MAP_H

#include <cstddef>
#include <boost/shared_ptr.hpp>

namespace tt3d {
namespace Utils {

class BufferMap {
    public:
        BufferMap():
            _offset(0) {};
    private:
        size_t _offset;
    public:
        size_t getOffset() const { return _offset; }
        void setOffset(const size_t offset) { _offset = offset; }
    public:
        virtual size_t map(const size_t index) { return index+_offset; }; 
};

typedef boost::shared_ptr<BufferMap> BufferMapHandle;

}
}

#endif
