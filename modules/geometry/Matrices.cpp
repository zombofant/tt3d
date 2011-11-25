#include "Matrices.hpp"
#include <cmath>

namespace tt3d {
namespace Geometry {
    
static inline VectorFloat sqr(VectorFloat a) {
    return a * a;
}

/* tt3d::Geometry::Matrix3 */

Matrix3::Matrix3() {
    coeff = {1, 0, 0, 0, 1, 0, 0, 0, 1};
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
    coeff = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
}

Matrix4 Matrix4::Rotation(const Vector3 rawAxis, const VectorFloat angle) {
    return Matrix4::Rotation(Matrix3::Rotation(rawAxis, angle));
}

Matrix4 Matrix4::Rotation(const Matrix3 m3) {
    Matrix4 result;
    result[0] = m3[0];
    result[1] = m3[1];
    result[2] = m3[2];
    
    result[4] = m3[3];
    result[5] = m3[4];
    result[6] = m3[5];
    
    result[8] = m3[6];
    result[9] = m3[7];
    result[10] = m3[8];
}

/* matrix operations */

/*operator * (A: TMatrix3; B: TVector3): TVector3; inline;
begin
  Result.X := A[0] * B.X + A[3] * B.Y + A[6] * B.Z;
  Result.Y := A[1] * B.X + A[4] * B.Y + A[7] * B.Z;
  Result.Z := A[2] * B.X + A[5] * B.Z + A[8] * B.Z;
end;

operator * (A: TMatrix4; B: TVector4): TVector4; inline;
begin
  Result.X := A[0] * B.X + A[4] * B.Y + A[8] * B.Z + A[12] * B.W;
  Result.Y := A[1] * B.X + A[5] * B.Y + A[9] * B.Z + A[13] * B.W;
  Result.Z := A[2] * B.X + A[6] * B.Z + A[10] * B.Z + A[14] * B.W;
  Result.W := A[3] * B.X + A[7] * B.Z + A[11] * B.Z + A[15] * B.W;
end;*/

Vector3 operator* (Matrix3 a, Vector3 b) {
    return Vector3(
        a.coeff[0] * b.x + a.coeff[3] * b.y + a.coeff[6] * b.z,
        a.coeff[1] * b.x + a.coeff[4] * b.y + a.coeff[7] * b.z,
        a.coeff[2] * b.x + a.coeff[5] * b.z + a.coeff[8] * b.z
    );
}

Matrix3 operator* (Matrix3 a, Matrix3 b) {
    Matrix3 result;
    for (int i=0; i < 3; i++) {
        VectorFloat sum = 0;
        for (int k=0; k < 3; k++) {
            sum += a.coeff[i]*b.coeff[k];
        }
        result.coeff[i] = sum;
    }
}

Matrix3 operator* (Matrix3 a, VectorFloat b);

Vector4 operator* (Matrix4 a, Vector4 b) {
    return Vector4(
        a.coeff[0] * b.x + a.coeff[4] * b.y + a.coeff[8] * b.z + a.coeff[12] * b.w,
        a.coeff[1] * b.x + a.coeff[5] * b.y + a.coeff[9] * b.z + a.coeff[13] * b.w,
        a.coeff[2] * b.x + a.coeff[6] * b.y + a.coeff[10] * b.z + a.coeff[14] * b.w,
        a.coeff[3] * b.x + a.coeff[7] * b.y + a.coeff[11] * b.z + a.coeff[15] * b.w
    );
}

Matrix4 operator* (Matrix4 a, Matrix4 b);
Matrix4 operator* (Matrix4 a, VectorFloat b);

}
}
