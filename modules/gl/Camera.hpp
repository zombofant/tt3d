#include "Viewport.hpp"
#include "modules/geometry/Vectors.hpp"
#include "modules/geometry/Matrices.hpp"

namespace tt3d {
namespace GL {

class GLCamera {
    public:
        GLCamera();
        ~GLCamera();
    private:
        bool modelViewInvalidated, projectionInvalidated;
    protected:
        Matrix4f modelView, projection;
        const GLViewport *viewport;
    protected:
        void deinvalidateModelView();
        void deinvalidateProjection();
        void doMoved();
        virtual doViewportChanged();
        void invalidateModelView();
        void invalidateProjection();
        virtual void recalculateModelView();
        virtual void recalculateProjection();
    public:
        Matrix4f getOneMatrix();
        void load();
        void loadAsOne();
        void mult();
        void multAsOne();
        virtual void update(const double interval);
        void validate();
    public:
        const GLViewport *getViewport();
        void setViewport(const GLViewport *viewport);
};

class GLCameraPerspective: public GLCamera {
    public:
        GLCameraPerspective();
    private:
        VectorFloat fov, nearZ, farZ;
    protected:
        virtual void doViewportChanged();
        virtual void recalculateProjection();
    public:
        VectorFloat getFarZ() { return farZ; };
        VectorFloat getFieldOfView() { return fov; };
        VectorFloat getNearZ() { return nearZ; };
        void setFarZ(const VectorFloat aFarZ);
        void setFieldOfView(const VectorFloat aFOV);
        void setNearZ(const VectorFloat aNearZ);
};

class GLCameraFree: public GLCameraPerspective {
    public:
        GLCameraFree();
    private:
        Matrix4 modelViewHD, invModelViewHD;
        Vector3 position, rotation;
        VectorFloat zoom;
        Vector3 right, up, front, flatRight, flatFront;
        Vector3 transformedPos;
        bool flatRightFrontInvalidated, transformedPosInvalidated;
    protected:
        void deinvalidateFlatRightFront();
        void deinvalidateTransformedPos();
        void invalidateFlatRightFront();
        void invalidateTransformedPos();
        virtual void recalculateFlatRightFront();
        void recalculateModelView();
        void recalculateTransformedPos();
    public:
        Vector3 getFlatRight();
        Vector3 getFlatFront();
        Vector3 getFront() { return front; }
        Vector3 getPosition() { return position; }
        Vector3 getRight() { return right; }
        Vector3 getRotation() { return rotation; }
        Vector3 getTransformedPos();
        Vector3 getUp() { return up; }
        VectorFloat getZoom() { return zoom; }
}

class GLCameraFreeSmooth: public GLCameraFree {
    public:
        GLCameraFreeSmooth();
    private:
        Vector2 accel, rotAccel;
        Vector2 velocity, rotVelocity;
        VectorFloat zoomVelocity, zoomAccel;
        
        bool moving, rotating;
        Vector2 posTarget, rotTarget;
    public:
        void accel(const Vector2 by) { accel += by; };
        void accelRot(const Vector2 by) { rotAccel += by; };
        void accelZoom(const VectorFloat by) { zoomAccel += by; };
        void issueMoveTo(const Vector2 target);
        void issueRotTo(const Vector2 target);
        void stopMove(const bool resetAcceleration);
        void stopRot(const bool resetAcceleration);
        void stopZoom(const bool resetAcceleration);
        void update(const double interval);
    public:
        Vector2 getAccel() { return accel; }
        Vector2 getVelocity() { return velocity; }
        Vector2 getRotAccel() { return rotAccel; }
        Vector2 getRotVelocity() { return rotVelocity; }
        VectorFloat getZoomAccel() { return zoomAccel; }
        VectorFloat getZoomVelocity() { return zoomVelocity; }
        
        void setAccel(const Vector2 aValue) { accel = aValue; };
        void setVelocity(const Vector2 aValue) { vel = aValue; };
        void setRotAccel(const Vector2 aValue) { rotAccel = aValue; };
        void setRotVelocity(const Vector2 aValue) { rotVelocity = aValue; };
        void setZoomAccel(const VectorFloat aValue) { zoomAccel = aValue; };
        void setZoomVelocity(const VectorFloat aValue) { zoomVelocity = aValue; };
}

}
}
