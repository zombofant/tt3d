#include "Vectors.hpp"
#include <cmath>

#include "VectorTemplates.hpp"

namespace tt3d {
namespace Geometry {

/* tt3d::Geometry::Vector2 */

Vector2::Vector2() {
    x = 0.;
    y = 0.;
}

Vector2::Vector2(const Vector2 &vec2) {
    x = vec2.x;
    y = vec2.y;
}

Vector2::Vector2(VectorFloat X, VectorFloat Y) {
    x = X;
    y = Y;
}

Vector2::Vector2(Vector2f vec2f) {
    x = vec2f.x;
    y = vec2f.y;
}

Vector2::Vector2(VectorFloat angle) {
    x = cos(angle);
    y = sin(angle);
}

VectorFloat Vector2::length() const {
    return sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() const {
    return *this / length();
}

void Vector2::normalize() {
    VectorFloat len = length();
    if (len == 0.)
        return;
    x = x / len;
    y = y / len;
}

Vector2f Vector2::toVector2f() const {
    Vector2f result;
    result.x = x;
    result.y = y;
    return result;
}

Vector2 &Vector2::operator+= (const Vector2 b) {
    vectorAdd<2>(as_array, b.as_array, as_array);
    return *this;
}

Vector2 &Vector2::operator/=(const VectorFloat b) {
    vectorScalarDiv<2>(as_array, b, as_array);
    return *this;
}

Vector2 &Vector2::operator*=(const VectorFloat b) {
    vectorScalarMult<2>(as_array, b, as_array);
    return *this;
}

/* tt3d::Geometry::Vector3 */

Vector3::Vector3() {
    x = 0.;
    y = 0.;
    z = 0.;
}

Vector3::Vector3(double X, double Y, double Z) {
    x = X;
    y = Y;
    z = Z;
}

Vector3::Vector3(Vector2 vec2, double Z) {
    x = vec2.x;
    y = vec2.y;
    z = Z;
}

Vector3::Vector3(Vector3f vec3f) {
    x = vec3f.x;
    y = vec3f.y;
    z = vec3f.z;
}

Vector2 Vector3::vec2() const {
    return Vector2(x, y);
}

VectorFloat Vector3::length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const {
    return *this / length();
}

void Vector3::normalize() {
    VectorFloat len = length();
    if (len == 0.)
        return;
    x = x / len;
    y = y / len;
    z = z / len;
}

Vector3f Vector3::toVector3f() const {
    Vector3f result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

Vector3 &Vector3::operator+=(const Vector3 b) {
    vectorAdd<3>(as_array, b.as_array, as_array);
    return *this;
}

Vector3 &Vector3::operator/=(const VectorFloat b) {
    vectorScalarDiv<3>(as_array, b, as_array);
    return *this;
}

/* tt3d::Geometry::Vector4 */

Vector4::Vector4() {
    x = 0.;
    y = 0.;
    z = 0.;
    w = 0.;
}

Vector4::Vector4(double X, double Y, double Z, double W) {
    x = X;
    y = Y;
    z = Z;
    w = W;
}

Vector4::Vector4(Vector2 vec2, double Z, double W) {
    x = vec2.x;
    y = vec2.y;
    z = Z;
    w = W;
}

Vector4::Vector4(Vector3 vec3, double W) {
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    w = W;
}

Vector4::Vector4(Vector4f vec4f) {
    x = vec4f.x;
    y = vec4f.y;
    z = vec4f.z;
    w = vec4f.w;
}

Vector2 Vector4::vec2() const {
    return Vector2(x, y);
}

Vector3 Vector4::vec3() const {
    return Vector3(x, y, z);
}

VectorFloat Vector4::length() const {
    return sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::normalized() const {
    return *this / length();
}

void Vector4::normalize() {
    VectorFloat len = length();
    if (len == 0.)
        return;
    x = x / len;
    y = y / len;
    z = z / len;
    w = w / len;
}

Vector4f Vector4::toVector4f() const {
    Vector4f result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

/* Vector2 operators */

Vector2 operator+ (Vector2 a, Vector2 b) {
    Vector2 result;
    vectorAdd<2>(a.as_array, b.as_array, result.as_array);
    return result;
}

Vector2 operator+ (Vector2 a, VectorFloat b) {
    Vector2 result;
    vectorScalarAdd<2>(a.as_array, b, result.as_array);
    return result;
}

Vector2 operator+ (VectorFloat a, Vector2 b) {
    Vector2 result;
    vectorScalarAdd<2>(b.as_array, a, result.as_array);
    return result;
}

Vector2 operator- (Vector2 a) {
    Vector2 result(a);
    vectorInvert<2>(a.as_array);
    return result;
}

Vector2 operator- (Vector2 a, Vector2 b) {
    Vector2 result;
    vectorSub<2>(a.as_array, b.as_array, result.as_array);
    return result;
}

Vector2 operator- (Vector2 a, VectorFloat b) {
    Vector2 result;
    vectorScalarSub<2>(a.as_array, b, result.as_array);
    return result;
}

VectorFloat operator* (Vector2 a, Vector2 b) {
    return vectorDotProduct<2>(a.as_array, b.as_array);
}

Vector2 operator* (Vector2 a, VectorFloat b) {
    Vector2 result;
    vectorScalarMult<2>(a.as_array, b, result.as_array);
    return result;
}

Vector2 operator/ (Vector2 a, VectorFloat b) {
    Vector2 result;
    vectorScalarDiv<2>(a.as_array, b, result.as_array);
    return result;
}

bool operator== (Vector2 a, Vector2 b) {
    return (a.x == b.x) && (a.y == b.y);
}

/* Vector3 operators */

Vector3 operator+ (Vector3 a, Vector3 b) {
    Vector3 result;
    vectorAdd<3>(a.as_array, b.as_array, result.as_array);
    return result;
}

Vector3 operator+ (Vector3 a, VectorFloat b) {
    Vector3 result;
    vectorScalarAdd<3>(a.as_array, b, result.as_array);
    return result;
}

Vector3 operator+ (VectorFloat a, Vector3 b) {
    Vector3 result;
    vectorScalarAdd<3>(b.as_array, a, result.as_array);
    return result;
}

Vector3 operator- (Vector3 a) {
    Vector3 result(a);
    vectorInvert<3>(a.as_array);
    return result;
}

Vector3 operator- (Vector3 a, Vector3 b) {
    Vector3 result;
    vectorSub<3>(a.as_array, b.as_array, result.as_array);
    return result;
}

Vector3 operator- (Vector3 a, VectorFloat b) {
    Vector3 result;
    vectorScalarSub<3>(a.as_array, b, result.as_array);
    return result;
}
    
VectorFloat operator* (Vector3 a, Vector3 b) {
    return vectorDotProduct<3>(a.as_array, b.as_array);
}

Vector3 operator* (Vector3 a, VectorFloat b) {
    Vector3 result;
    vectorScalarMult<3>(a.as_array, b, result.as_array);
    return result;
}

Vector3 operator% (Vector3 a, Vector3 b) {
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Vector3 operator/ (Vector3 a, VectorFloat b) {
    Vector3 result;
    vectorScalarDiv<3>(a.as_array, b, result.as_array);
    return result;
}

bool operator== (Vector3 a, Vector3 b) {
    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

/* Vector4 operators */

Vector4 operator+ (Vector4 a, Vector4 b) {
    Vector4 result;
    vectorAdd<4>(a.as_array, b.as_array, result.as_array);
    return result;
}

Vector4 operator+ (Vector4 a, VectorFloat b) {
    Vector4 result;
    vectorScalarAdd<4>(a.as_array, b, result.as_array);
    return result;
}

Vector4 operator+ (VectorFloat a, Vector4 b) {
    Vector4 result;
    vectorScalarAdd<4>(b.as_array, a, result.as_array);
    return result;
}

Vector4 operator- (Vector4 a) {
    Vector4 result(a);
    vectorInvert<4>(a.as_array);
    return result;
}

Vector4 operator- (Vector4 a, Vector4 b) {
    Vector4 result;
    vectorSub<4>(a.as_array, b.as_array, result.as_array);
    return result;
}
    
Vector4 operator- (Vector4 a, VectorFloat b) {
    Vector4 result;
    vectorScalarSub<4>(a.as_array, b, result.as_array);
    return result;
}

VectorFloat operator* (Vector4 a, Vector4 b) {
    return vectorDotProduct<4>(a.as_array, b.as_array);
}
    
Vector4 operator* (Vector4 a, VectorFloat b) {
    Vector4 result;
    vectorScalarMult<4>(a.as_array, b, result.as_array);
    return result;
}

Vector4 operator/ (Vector4 a, VectorFloat b) {
    Vector4 result;
    vectorScalarDiv<4>(a.as_array, b, result.as_array);
    return result;
}

bool operator== (Vector4 a, Vector4 b) {
    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
}


}
}
