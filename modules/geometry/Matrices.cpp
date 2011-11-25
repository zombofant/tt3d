#include "Matrices.hpp"
#include <cmath>
#include <string.h>

#include "MatrixTemplates.hpp"

namespace tt3d {
namespace Geometry {
    
static inline VectorFloat sqr(VectorFloat a) {
    return a * a;
}

/* tt3d::Geometry::Matrix3 */

Matrix3::Matrix3() {
    matrixIdentity<3>(coeff);
}

Matrix3f Matrix3::toMatrix3f() {
    Matrix3f result;
    for (int i=0; i<9; i++) {
        result.coeff[i] = coeff[i];
    }
}

Matrix3 Matrix3::Rotation(const Vector3 rawAxis, const VectorFloat angle) {
    const VectorFloat saveSin = sin(angle);
    const VectorFloat saveCos = cos(angle);
    const VectorFloat oneMinusCos = 1-saveCos;
    const VectorFloat len = rawAxis.length();
    
    if (len == 0.) {
        return Matrix3();
    }
    
    Matrix3 result;
    const Vector3 axis = rawAxis / len;
    
    result.coeff[0] = (oneMinusCos * sqr(axis.x)) + saveCos;
    result.coeff[1] = (oneMinusCos * axis.x * axis.y) - (axis.z * saveSin);
    result.coeff[2] = (oneMinusCos * axis.z * axis.x) + (axis.y * saveSin);

    result.coeff[3] = (oneMinusCos * axis.x * axis.y) + (axis.z * saveSin);
    result.coeff[4] = (oneMinusCos * sqr(axis.y)) + saveCos;
    result.coeff[5] = (oneMinusCos * axis.y * axis.z) - (axis.x * saveSin);

    result.coeff[6] = (oneMinusCos * axis.z * axis.x) - (axis.y * saveSin);
    result.coeff[7] = (oneMinusCos * axis.y * axis.z) + (axis.x * saveSin);
    result.coeff[8] = (oneMinusCos * sqr(axis.z)) + saveCos;
    
    return result;
}

/* tt3d::Geometry::Matrix4 */

Matrix4::Matrix4() {
    matrixIdentity<4>(coeff);
}

Matrix4f Matrix4::toMatrix4f() {
    Matrix4f result;
    for (int i=0; i<16; i++) {
        result.coeff[i] = coeff[i];
    }
}

Matrix4 Matrix4::Rotation(const Vector3 rawAxis, const VectorFloat angle) {
    return Matrix4::Rotation(Matrix3::Rotation(rawAxis, angle));
}

Matrix4 Matrix4::Rotation(const Matrix3 m3) {
    Matrix4 result;
    result.coeff[0] = m3.coeff[0];
    result.coeff[1] = m3.coeff[1];
    result.coeff[2] = m3.coeff[2];
    
    result.coeff[4] = m3.coeff[3];
    result.coeff[5] = m3.coeff[4];
    result.coeff[6] = m3.coeff[5];
    
    result.coeff[8] = m3.coeff[6];
    result.coeff[9] = m3.coeff[7];
    result.coeff[10] = m3.coeff[8];
}

/* matrix operations */

Vector3 operator* (Matrix3 a, Vector3 b) {
    Vector3 result;
    matrixVectorMult<3>(a.coeff, b.as_array, result.as_array);
}

Matrix3 operator* (Matrix3 a, Matrix3 b) {
    Matrix3 result;
    matrixMult<3>(a.coeff, b.coeff, result.coeff);
}

Matrix3 operator* (Matrix3 a, VectorFloat b) {
    Matrix3 result;
    matrixScalarMult<3>(a.coeff, b, result.coeff);
}


Vector4 operator* (Matrix4 a, Vector4 b) {
    Vector4 result;
    matrixVectorMult<3>(a.coeff, b.as_array, result.as_array);
}

Matrix4 operator* (Matrix4 a, Matrix4 b) {
    Matrix4 result;
    matrixMult<4>(a.coeff, b.coeff, result.coeff);
}

Matrix4 operator* (Matrix4 a, VectorFloat b) {
    Matrix4 result;
    matrixScalarMult<4>(a.coeff, b, result.coeff);
}

}
}
