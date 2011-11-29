#ifndef _TT3D_GL_CAMERA_H
#define _TT3D_GL_CAMERA_H

#include "Viewport.hpp"
#include "modules/geometry/Vectors.hpp"
#include "modules/geometry/Matrices.hpp"
#include <boost/shared_ptr.hpp>

namespace tt3d {
namespace GL {
    
using namespace tt3d::Geometry;

class Camera {
    public:
        Camera();
    private:
        bool modelViewInvalidated, projectionInvalidated;
    protected:
        Matrix4 modelView, projection;
        ViewportHandle viewport;
    protected:
        void deinvalidateModelView();
        void deinvalidateProjection();
        void doMoved();
        virtual void doViewportChanged();
        void invalidateModelView();
        void invalidateProjection();
        virtual void recalculateModelView();
        virtual void recalculateProjection();
    public:
        Matrix4 getOneMatrix();
        void load();
        void loadAsOne();
        void mult();
        void multAsOne();
        virtual void update(const double interval);
        void validate();
        void viewportChanged();
    public:
        const ViewportHandle getViewport() const { return viewport; };
        void setViewport(const ViewportHandle aValue);
};

typedef boost::shared_ptr<Camera> CameraHandle;

class CameraPerspective: public Camera {
    public:
        CameraPerspective();
    protected:
        VectorFloat _fov, _nearZ, _farZ;
    protected:
        virtual void doViewportChanged();
        virtual void recalculateProjection();
    public:
        VectorFloat getFarZ() { return _farZ; };
        VectorFloat getFieldOfView() { return _fov; };
        VectorFloat getNearZ() { return _nearZ; };
        void setFarZ(const VectorFloat aValue);
        void setFieldOfView(const VectorFloat aValue);
        void setNearZ(const VectorFloat aValue);
};

class CameraFree: public CameraPerspective {
    public:
        CameraFree();
    protected:
        Vector3 position;
        Vector2 rotation;
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
        Vector3 getFlatFront();
        Vector3 getFlatRight();
        Vector3 getFront() { return front; }
        Vector3 getPosition() { return position; }
        Vector3 getRight() { return right; }
        Vector2 getRotation() { return rotation; }
        Vector3 getTransformedPos();
        Vector3 getUp() { return up; }
        VectorFloat getZoom() { return zoom; }
        
        void setPosition(const Vector3 &aValue);
        void setRotation(const Vector2 &aValue);
        void setZoom(const VectorFloat &aValue);
};

class CameraFreeSmooth: public CameraFree {
    public:
        CameraFreeSmooth();
    private:
        Vector2 accel, rotAccel;
        Vector2 velocity, rotVelocity;
        VectorFloat zoomVelocity, zoomAccel;
        
        bool moving, rotating;
        Vector2 posTarget, rotTarget;
    public:
        void addAccel(const Vector2 by) { accel += by; };
        void addAccelRot(const Vector2 by) { rotAccel += by; };
        void addAccelZoom(const VectorFloat by) { zoomAccel += by; };
        void issueMoveTo(const Vector2 target);
        void issueRotTo(const Vector2 target);
        void stopMove(const bool resetAccel);
        void stopRot(const bool resetAccel);
        void stopZoom(const bool resetAccel);
        void update(const double interval);
    public:
        Vector2 getAccel() { return accel; }
        Vector2 getVelocity() { return velocity; }
        Vector2 getRotAccel() { return rotAccel; }
        Vector2 getRotVelocity() { return rotVelocity; }
        VectorFloat getZoomAccel() { return zoomAccel; }
        VectorFloat getZoomVelocity() { return zoomVelocity; }
        
        void setAccel(const Vector2 aValue) { accel = aValue; };
        void setVelocity(const Vector2 aValue) { velocity = aValue; };
        void setRotAccel(const Vector2 aValue) { rotAccel = aValue; };
        void setRotVelocity(const Vector2 aValue) { rotVelocity = aValue; };
        void setZoomAccel(const VectorFloat aValue) { zoomAccel = aValue; };
        void setZoomVelocity(const VectorFloat aValue) { zoomVelocity = aValue; };
};

}
}

#endif
