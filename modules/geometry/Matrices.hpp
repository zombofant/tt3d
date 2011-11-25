#ifndef _TT3D_GEOMETRY_MATRICES_H
#define _TT3D_GEOMETRY_MATRICES_H

#include "Vectors.hpp"

namespace tt3d {
namespace Geometry {

struct Matrix3 {
    double coeff[9];
    
    Matrix3();
    
    static Matrix3 Rotation(const Vector3 axis, const VectorFloat angle);
};

struct Matrix4 {
    double coeff[16];
    
    Matrix4();
    
    static Matrix4 Rotation(const Vector3 axis, const VectorFloat angle);
    static Matrix4 Rotation(const Matrix3 m3);
    static Matrix4 Translation(const Vector3 by);
    static Matrix4 Scale(const Vector3 by);
};

Vector3 operator* (Matrix3 a, Vector3 b);
Matrix3 operator* (Matrix3 a, Matrix3 b);
Matrix3 operator* (Matrix3 a, VectorFloat b);

Vector4 operator* (Matrix4 a, Vector4 b);
Matrix4 operator* (Matrix4 a, Matrix4 b);
Matrix4 operator* (Matrix4 a, VectorFloat b);
    
}
}

#endif
