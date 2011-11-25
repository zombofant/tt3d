#include "Geometry.hpp"
#include <cmath>

namespace tt3d {
namespace Geometry {
    
/* tt3d::Geometry::Vector2 */

Vector2::Vector2() {
    x = 0.;
    y = 0.;
}

Vector2::Vector2(double X, double Y) {
    x = X;
    y = Y;
}

Vector2::Vector2(Vector2f vec2f) {
    x = vec2f.x;
    y = vec2f.y;
}

VectorFloat Vector2::length() {
    return sqrt(x * x + y * y);
}

Vector2 Vector2::normalized() {
    return *this / length();
}

void Vector2::normalize() {
    VectorFloat len = length();
    if (len == 0.)
        return;
    x = x / len;
    y = y / len;
}

Vector2f Vector2::toVector2f() {
    Vector2f result;
    result.x = x;
    result.y = y;
    return result;
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

VectorFloat Vector3::length() {
    return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() {
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

Vector3f Vector3::toVector3f() {
    Vector3f result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
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

VectorFloat Vector4::length() {
    return sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::normalized() {
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

Vector4f Vector4::toVector4f() {
    Vector4f result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

/* Vector2 operators */

Vector2 operator+ (Vector2 a, Vector2 b) {
    return Vector2(a.x + b.x, a.y + b.y);
}

Vector2 operator+ (Vector2 a, VectorFloat b) {
    return Vector2(a.x + b, a.y + b);
}

Vector2 operator+ (VectorFloat a, Vector2 b) {
    return b + a;
}

Vector2 operator- (Vector2 a) {
    return Vector2(-a.x, -a.y);
}

Vector2 operator- (Vector2 a, Vector2 b) {
    return Vector2(a.x - b.x, a.y - b.y);
}

Vector2 operator- (Vector2 a, VectorFloat b) {
    return Vector2(a.x - b, a.y - b);
}

VectorFloat operator* (Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

Vector2 operator* (Vector2 a, VectorFloat b) {
    return Vector2(a.x * b, a.y * b);
}

Vector2 operator/ (Vector2 a, VectorFloat b) {
    return Vector2(a.x / b, a.y / b);
}

bool operator== (Vector2 a, Vector2 b) {
    return (a.x == b.x) && (a.y == b.y);
}

/* Vector3 operators */

Vector3 operator+ (Vector3 a, Vector3 b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 operator+ (Vector3 a, VectorFloat b) {
    return Vector3(a.x + b, a.y + b, a.z + b);
}

Vector3 operator+ (VectorFloat a, Vector3 b) {
    return b + a;
}

Vector3 operator- (Vector3 a) {
    return Vector3(-a.x, -a.y, -a.z);
}

Vector3 operator- (Vector3 a, Vector3 b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 operator- (Vector3 a, VectorFloat b) {
    return Vector3(a.x - b, a.y - b, a.z - b);
}
    
VectorFloat operator* (Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 operator* (Vector3 a, VectorFloat b) {
    return Vector3(a.x * b, a.y * b, a.z * b);
}

Vector3 operator% (Vector3 a, Vector3 b) {
    return Vector3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

Vector3 operator/ (Vector3 a, VectorFloat b) {
    return Vector3(a.x / b, a.y / b, a.z / b);
}

bool operator== (Vector3 a, Vector3 b) {
    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

/* Vector4 operators */

Vector4 operator+ (Vector4 a, Vector4 b) {
    return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vector4 operator+ (Vector4 a, VectorFloat b) {
    return Vector4(a.x + b, a.y + b, a.z + b, a.w + b);
}

Vector4 operator+ (VectorFloat a, Vector4 b) {
    return b + a;
}

Vector4 operator- (Vector4 a) {
    return Vector4(-a.x, -a.y, -a.z, -a.w);
}

Vector4 operator- (Vector4 a, Vector4 b) {
    return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
    
Vector4 operator- (Vector4 a, VectorFloat b) {
    return Vector4(a.x - b, a.y - b, a.z - b, a.w - b);
}

VectorFloat operator* (Vector4 a, Vector4 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
    
Vector4 operator* (Vector4 a, VectorFloat b) {
    return Vector4(a.x * b, a.y * b, a.z * b, a.w * b);
}

Vector4 operator/ (Vector4 a, VectorFloat b) {
    return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
}

bool operator== (Vector4 a, Vector4 b) {
    return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w));
}


}
}
