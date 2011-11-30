#include "GeometryQuadsForTris.hpp"

namespace tt3d {
namespace GL {
    
using namespace tt3d;

/* tt3d::GL::GeometryQuadsForTris */

VertexAllocationHandle GeometryQuadsForTris::allocateVertices() {
    VertexAllocation *allocation = new VertexAllocation(_material->getGeometryBuffer()->allocateVertices(_quadCount * 4));
    VertexIndexList *indicies = new VertexIndexList();
    indicies->reserve(_quadCount * 6);
    for (int i = 0; i < _quadCount; i++) {
        const int allocIdx = i * 4;
        indicies->push_back((*allocation->bufferAllocation)[allocIdx]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+1]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+2]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+2]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+3]);
        
        /*indicies->push_back((*allocation->bufferAllocation)[allocIdx]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+1]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+2]);
        indicies->push_back((*allocation->bufferAllocation)[allocIdx+3]);*/
    }
    allocation->indicies = VertexIndexListHandle(indicies);
    allocation->staticHandle = _material->getStaticIndexBuffer()->add(allocation->indicies);
    return VertexAllocationHandle(allocation);
}

}
}
