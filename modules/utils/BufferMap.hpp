#ifndef _TT3D_UTILS_BUFFER_MAP_H
#define _TT3D_UTILS_BUFFER_MAP_H

#include <cstddef>

namespace tt3d {
namespace Utils {

class BufferMap {
    public:
        virtual size_t map(const size_t index) = 0; 
};

class BufferMapOffset: public BufferMap {
    public:
        BufferMapOffset(const size_t offset);
    private:
        const size_t _offset;
    public:
        virtual size_t map(const size_t index);
};

}
}

#endif
