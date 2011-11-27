#ifndef _TT3D_GEOMETRY_H
#define _TT3D_GEOMETRY_H

namespace tt3d {
namespace Geometry {
    
typedef double VectorFloat;

struct Vector2f {
    union {
        struct {
            float x, y;
        };
        float as_array[2];
    };
};

struct Vector3f {
    union {
        struct {
            float x, y, z;
        };
        float as_array[3];
    };
};

struct Vector4f {
    union {
        struct {
            float x, y, z, w;
        };
        float as_array[4];
    };
};
    
struct Vector2 {
    public:
        union {
            struct {
                union {
                    double x;
                    double u;
                };
                union {
                    double y;
                    double v;
                };
            };
            double as_array[2];
        };
        
        Vector2();
        Vector2(const Vector2 &vec2);
        Vector2(VectorFloat X, VectorFloat Y);
        Vector2(Vector2f vec2f);
        Vector2(VectorFloat angle);
        
        VectorFloat length() const;
        Vector2 normalized() const;
        void normalize();
        Vector2f toVector2f() const;
        
        Vector2 &operator+= (const Vector2 b);
        Vector2 &operator/= (const VectorFloat b);
        Vector2 &operator*= (const VectorFloat b);
};

struct Vector3 {
    public:
        union {
            struct {
                union {
                    double x;
                    double r;
                };
                union {
                    double y;
                    double g;
                };
                union {
                    double z;
                    double b;
                };
            };
            double as_array[3];
        };
        
        Vector3();
        Vector3(double X, double Y, double Z);
        Vector3(Vector2 vec2, double Z = 0.);
        Vector3(Vector3f vec3f);
        
        Vector2 vec2() const;
        
        VectorFloat length() const;
        Vector3 normalized() const;
        void normalize();
        Vector3f toVector3f() const;
        
        Vector3 &operator+= (const Vector3 b);
        Vector3 &operator/= (const VectorFloat b);
};

struct Vector4 {
    public:
        union {
            struct {
                union {
                    double x;
                    double r;
                };
                union {
                    double y;
                    double g;
                };
                union {
                    double z;
                    double b;
                };
                union {
                    double w;
                    double a;
                };
            };
            double as_array[4];
        };
        
        Vector4();
        Vector4(double X, double Y, double Z, double W);
        Vector4(Vector2 vec2, double Z = 0., double W = 0.);
        Vector4(Vector3 vec3, double W = 0.);
        Vector4(Vector4f vec4f);
        
        Vector2 vec2() const;
        Vector3 vec3() const;
        
        VectorFloat length() const;
        Vector4 normalized() const;
        void normalize();
        Vector4f toVector4f() const;
};

Vector2 operator+ (Vector2 a, Vector2 b);
Vector2 operator+ (Vector2 a, VectorFloat b);
Vector2 operator+ (VectorFloat a, Vector2 b);

Vector2 operator- (Vector2 a);
Vector2 operator- (Vector2 a, Vector2 b);
Vector2 operator- (Vector2 a, VectorFloat b);

VectorFloat operator* (Vector2 a, Vector2 b);
Vector2 operator* (Vector2 a, VectorFloat b);

Vector2 operator/ (Vector2 a, VectorFloat b);

bool operator== (Vector2 a, Vector2 b);


Vector3 operator+ (Vector3 a, Vector3 b);
Vector3 operator+ (Vector3 a, VectorFloat b);
Vector3 operator+ (VectorFloat a, Vector3 b);

Vector3 operator- (Vector3 a);
Vector3 operator- (Vector3 a, Vector3 b);
Vector3 operator- (Vector3 a, VectorFloat b);

VectorFloat operator* (Vector3 a, Vector3 b);
Vector3 operator* (Vector3 a, VectorFloat b);

Vector3 operator% (Vector3 a, Vector3 b);

Vector3 operator/ (Vector3 a, VectorFloat b);

bool operator== (Vector3 a, Vector3 b);


Vector4 operator+ (Vector4 a, Vector4 b);
Vector4 operator+ (Vector4 a, VectorFloat b);
Vector4 operator+ (VectorFloat a, Vector4 b);

Vector4 operator- (Vector4 a);
Vector4 operator- (Vector4 a, Vector4 b);
Vector4 operator- (Vector4 a, VectorFloat b);

VectorFloat operator* (Vector4 a, Vector4 b);
Vector4 operator* (Vector4 a, VectorFloat b);

Vector4 operator/ (Vector4 a, VectorFloat b);

bool operator== (Vector4 a, Vector4 b);

}
}

#endif
