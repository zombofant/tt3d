#include "BufferMap.hpp"

namespace tt3d {
namespace Utils {

/* tt3d::Utils::BufferMapOffset */

BufferMapOffset::BufferMapOffset(const size_t offset):
    _offset(offset)
{
    
}

size_t BufferMapOffset::map(const size_t index) {
    return index + _offset;
}

}
}
