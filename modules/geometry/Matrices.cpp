#include "Matrices.hpp"
#include <cmath>
#include <string.h>

namespace tt3d {
namespace Geometry {
    
static inline VectorFloat sqr(VectorFloat a) {
    return a * a;
}


/**
 * Write a column-wise n-dimensional identity matrix to the memory 
 * pointed to by result.
 * 
 * @param result    pointer to the memory where the resulting matrix 
 *                  will be written to.
 */
template <int n>
static inline void matrixIdentity(VectorFloat result[n*n]) {
    for (int i=0; i < n; i++) {
        for (int j=0; i < n; j++) {
            if (i == j)
                result[i + j*n] = 1.;
            else
                result[i + j*n] = 0.;
        }
    }
}

/**
 * Implements a generic matrix multiplication of two square matrices
 * with dimension dim. 
 * 
 * @param a         Pointer to the column-wise coefficients of the left 
 *                  operand.
 * @param b         Pointer to the column-wise coefficients of the right 
 *                  operand.
 * @param result    Pointer to a space where the column-wise 
 *                  coefficients of the resulting matrix will be written
 *                  to. This must be at least sqr(dim) in size.
 */
template <int dim> 
static inline void matrixMult(const VectorFloat a[dim*dim], 
    const VectorFloat b[dim*dim], 
    VectorFloat result[dim*dim]) 
{
    for (int i=0; i < dim; i++) {
        for (int j=0; j < dim; j++) {
            VectorFloat sum = 0;
            for (int k=0; k < dim; k++) {
                sum += a[i + k*dim] * b[k + j*dim];
            }
            result[i + j*dim] = sum;
        }
    }
}

/**
 * Implements a generic matrix-vector multiplication of a square matrix
 * with dimension dim and a vector with dimension dim.
 */
template <int dim>
static inline void matrixVectorMult(const VectorFloat matrix[dim*dim], 
    const VectorFloat vec[dim], 
    VectorFloat result[dim]) 
{
    // r_i = m_ij * v_j
    for (int i=0; i < dim; i++) {
        VectorFloat sum = 0.;
        for (int j=0; j < dim; j++) {
            sum += matrix[i + j*dim] * vec[j];
        }
        result[i] = sum;
    }
}

template <int dim>
static inline void matrixScalarMult(const VectorFloat matrix[dim*dim], 
    const VectorFloat factor, 
    VectorFloat result[dim*dim]) 
{
    for (int i=0; i < dim*dim; i++) {
        result[i] = matrix[i] * factor;
    }
}

/* tt3d::Geometry::Matrix3 */

Matrix3::Matrix3() {
    matrixIdentity<3>(coeff);
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
