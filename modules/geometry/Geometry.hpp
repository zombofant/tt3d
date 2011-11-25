
namespace tt3d {
namespace Geometry {
    
typedef double VectorFloat;
    
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
        Vector2(double X, double Y);
        
        VectorFloat length();
        Vector2 normalized();
        void normalize();
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
        
        VectorFloat length();
        Vector3 normalized();
        void normalize();
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
        
        VectorFloat length();
        Vector4 normalized();
        void normalize();
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
