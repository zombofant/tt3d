#include <GL/glew.h>
#include "Camera.hpp"
#include <cmath>

namespace tt3d {
namespace GL {
    
#define ACCEL_EPSILON 0.001
#define VELOCITY_EPSILON 0.001
    
static inline double sqr(const double a) {
    return a * a;
}

// why is the compiler unable to find the declaration from cmath?
static inline VectorFloat abs(const VectorFloat v) {
    if (v > 0) {
        return v;
    } else {
        return -v;
    }
}

template <class T>
bool processVectorAccel(Vector2 &accel, Vector2 &velocity, T &value, const VectorFloat interval) 
{
    const VectorFloat intervalSqr = sqr(interval);
    
    if (!(velocity == T()) || !(accel == T())) {
        value += T(0.5 * accel * intervalSqr + velocity * interval);
        velocity += accel * interval;
        
        accel /= 180 * interval;
        if (abs(accel.x) <= ACCEL_EPSILON) {
            accel.x = 0.;
        }
        if (abs(accel.y) <= ACCEL_EPSILON) {
            accel.y = 0.;
        }
        
        velocity /= 110 * interval;
        if (abs(velocity.x) <= VELOCITY_EPSILON) {
            velocity.x = 0.;
        }
        if (abs(velocity.y) <= VELOCITY_EPSILON) {
            velocity.y = 0.;
        }
        return true;
    } else {
        return false;
    }
}
    
/* tt3d::GL::Camera */

Camera::Camera():
    modelViewInvalidated(false),
    projectionInvalidated(false),
    modelView(),
    projection(),
    viewport()
{
    
}

void Camera::deinvalidateModelView() {
    modelViewInvalidated = false;
}

void Camera::deinvalidateProjection() {
    projectionInvalidated = false;
}

void Camera::doMoved() {
    // TODO: call a callback here.
}

void Camera::doViewportChanged() {
    
}

void Camera::invalidateModelView() {
    modelViewInvalidated = true;
}

void Camera::invalidateProjection() {
    projectionInvalidated = true;
}

void Camera::recalculateModelView() {
    modelView = Matrix4();
}

void Camera::recalculateProjection() {
    projection = Matrix4();
}

Matrix4 Camera::getOneMatrix() {
    validate();
    return projection * modelView;
}

void Camera::load() {
    validate();
    /*projection.dump();
    modelView.dump();*/
    const Matrix4f proj = projection.toMatrix4f();
    const Matrix4f mv = modelView.toMatrix4f();
    
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(proj.coeff);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mv.coeff);
}

void Camera::loadAsOne() {
    validate();
    glLoadMatrixd(getOneMatrix().coeff);
}

void Camera::mult() {
    validate();
    glMatrixMode(GL_PROJECTION);
    glMultMatrixd(projection.coeff);
    glMatrixMode(GL_MODELVIEW);
    glMultMatrixd(modelView.coeff);
}

void Camera::multAsOne() {
    validate();
    glMultMatrixd(getOneMatrix().coeff);
}

void Camera::update(const double interval) {
    
}

void Camera::validate() {
    if (projectionInvalidated) {
        recalculateProjection();
    }
    if (modelViewInvalidated) {
        recalculateModelView();
    }
}

void Camera::viewportChanged() {
    doViewportChanged();
}

void Camera::setViewport(const ViewportHandle aValue) {
    viewport = aValue;
    doViewportChanged();
}

/* tt3d::GL::CameraPerspective */

CameraPerspective::CameraPerspective():
    _fov(60.),
    _nearZ(1.0),
    _farZ(100.0)
{
    
}

void CameraPerspective::doViewportChanged() {
    invalidateProjection();
}

void CameraPerspective::recalculateProjection() {
    const float f = 1. / tan(_fov / 360. * M_PI); // cotan(DegToRad(fov) / 2.)
    const float aspect = double(viewport->getWidth()) / (viewport->getHeight());
    
    projection = Matrix4();
    projection.coeff[0] = f / aspect;
    projection.coeff[5] = f;
    projection.coeff[10] = (_farZ - _nearZ) / (_nearZ - _farZ);
    projection.coeff[11] = -1.0;
    projection.coeff[14] = (2*_farZ*_nearZ) / (_nearZ - _farZ);
    projection.coeff[15] = 0.;
    deinvalidateProjection();
}

void CameraPerspective::setFarZ(const VectorFloat aValue) {
    if (_farZ == aValue)
        return;
    _farZ = aValue;
    invalidateProjection();
}

void CameraPerspective::setFieldOfView(const VectorFloat aValue) {
    if (_fov == aValue)
        return;
    _fov = aValue;
    invalidateProjection();
}

void CameraPerspective::setNearZ(const VectorFloat aValue) {
    if (_nearZ == aValue)
        return;
    _nearZ = aValue;
    invalidateProjection();
}

/* tt3d::GL::CameraFree */

CameraFree::CameraFree():
    position(0., 0., 0.),
    rotation(0., 0.),
    zoom(0.),
    right(),
    up(),
    front(),
    flatRight(),
    flatFront(),
    transformedPos(0., 0., 0.),
    flatRightFrontInvalidated(true),
    transformedPosInvalidated(true)
{
    
}

void CameraFree::deinvalidateFlatRightFront() {
    flatRightFrontInvalidated = false;
}

void CameraFree::deinvalidateTransformedPos() {
    transformedPosInvalidated = false;
}

void CameraFree::invalidateFlatRightFront() {
    flatRightFrontInvalidated = true;
}

void CameraFree::invalidateTransformedPos() {
    transformedPosInvalidated = true;
}

void CameraFree::recalculateFlatRightFront() {
    flatRight = Vector2(-rotation.y);
    flatFront = Vector2(-(rotation.y + M_PI / 2.));
    
    deinvalidateFlatRightFront();
}

void CameraFree::recalculateModelView() {
    /*std::cout << zoom << std::endl;
    position.dump();
    rotation.dump();*/
    modelView = 
        Matrix4::Translation(Vector3(0., 0., zoom)) * 
        Matrix4::RotationX(rotation.x) *
        Matrix4::RotationZ(rotation.y) *
        Matrix4::Translation(-position);
    /*modelView.dump();*/
    
    right = Vector3(modelView.coeff[0], modelView.coeff[4], modelView.coeff[8]);
    up = Vector3(modelView.coeff[1], modelView.coeff[5], modelView.coeff[9]);
    front = Vector3(modelView.coeff[2], modelView.coeff[6], modelView.coeff[10]);
    
    deinvalidateModelView();
}

void CameraFree::recalculateTransformedPos() {
    invModelView = 
        Matrix4::Translation(-position) * 
        Matrix4::RotationZ(rotation.y) *
        Matrix4::RotationX(rotation.x) *
        Matrix4::Translation(Vector3(0., 0., zoom));
    transformedPos = (invModelView * Vector4(0., 0., 0., 1.)).vec3();
}

Vector3 CameraFree::getFlatFront() {
    if (flatRightFrontInvalidated)
        recalculateFlatRightFront();
    return flatFront;
}

Vector3 CameraFree::getFlatRight() {
    if (flatRightFrontInvalidated)
        recalculateFlatRightFront();
    return flatRight;
}

Vector3 CameraFree::getTransformedPos() {
    if (transformedPosInvalidated)
        recalculateTransformedPos();
    return transformedPos;
}

void CameraFree::setPosition(const Vector3 &aValue) {
    if (position == aValue)
        return;
    
    position = Vector3(aValue);
    invalidateModelView();
    invalidateFlatRightFront();
    invalidateTransformedPos();
}

void CameraFree::setRotation(const Vector2 &aValue) {
    if (rotation == aValue)
        return;
    
    rotation = Vector2(aValue);
    invalidateModelView();
    invalidateFlatRightFront();
    invalidateTransformedPos();
}

void CameraFree::setZoom(const VectorFloat &aValue) {
    if (zoom == aValue)
        return;
    
    rotation = aValue;
    invalidateModelView();
    invalidateFlatRightFront();
    invalidateTransformedPos();
}

/* tt3d::GL::CameraFreeSmooth */

CameraFreeSmooth::CameraFreeSmooth():
    accel(0., 0.),
    rotAccel(0., 0.),
    velocity(0., 0.),
    rotVelocity(0., 0.),
    zoomVelocity(0.),
    zoomAccel(0.),
    moving(false),
    rotating(false),
    posTarget(0., 0.),
    rotTarget(0., 0.)
{
    
}

void CameraFreeSmooth::issueMoveTo(const Vector2 target) {
    moving = true;
    posTarget = Vector2(target);
}

void CameraFreeSmooth::issueRotTo(const Vector2 target) {
    rotating = true;
    rotTarget = Vector2(target);
}

void CameraFreeSmooth::stopMove(const bool resetAccel) {
    moving = false;
    if (resetAccel) {
        accel = Vector2(0., 0.);
    }
}

void CameraFreeSmooth::stopRot(const bool resetAccel) {
    rotating = false;
    if (resetAccel) {
        rotAccel = Vector2(0., 0.);
    }
}

void CameraFreeSmooth::stopZoom(const bool resetAccel) {
    if (resetAccel) {
        zoomAccel = 0.;
    }
}

void CameraFreeSmooth::update(const double interval) {
    /*std::cout << "update" << std::endl;
    
    std::cout << "rotation accel "; rotAccel.dump();
    std::cout << "position accel "; accel.dump();*/
    
    if (moving) {
        const Vector2 d = posTarget - position.vec2();
        const VectorFloat l = d.length();
        accel = d * 50.0;
        if (l < 0.05) {
            moving = false;
        }
    }
    
    if (rotating) {
        Vector2 d = rotTarget - rotation;
        if (abs(d.y) > M_PI) {
            d.y = -d.y;
        }
        const VectorFloat l = d.length();
        rotAccel = d * 7.5;
        if (l < 0.05 ) {
            rotating = false;
        }
    }
    
    const VectorFloat intervalSqr = sqr(interval);
    if (processVectorAccel<Vector3>(accel, velocity, position, interval)) {
        invalidateModelView();
        invalidateTransformedPos();
        doMoved();
    }
    
    if (processVectorAccel<Vector2>(rotAccel, rotVelocity, rotation, interval)) {
        invalidateModelView();
        invalidateFlatRightFront();
        invalidateTransformedPos();
        doMoved();
    }
    
    if ((zoomVelocity != 0.) || (zoomAccel != 0.)) {
        zoom += 0.5 * zoomAccel * intervalSqr + zoomVelocity * interval;
        zoomVelocity += zoomAccel * interval;
        
        zoomAccel /= 180 * interval;
        if (abs(zoomAccel) <= ACCEL_EPSILON) {
            zoomAccel = 0.;
        }
        
        zoomVelocity /= 110 * interval;
        if (abs(zoomVelocity) <= VELOCITY_EPSILON) {
            zoomVelocity = 0.;
        }
        
        invalidateModelView();
        invalidateTransformedPos();
        doMoved();
    }
    
    if (rotation.x < -(8*M_PI/18)) {
        stopRot(true);
        rotation.x = -(8*M_PI/18);
        invalidateModelView();
        invalidateFlatRightFront();
        invalidateTransformedPos();
    }
    
    if (rotation.x > -(M_PI/18)) {
        stopRot(true);
        rotation.x = -(M_PI/18);
        invalidateModelView();
        invalidateFlatRightFront();
        invalidateTransformedPos();
    }
    
    if (rotation.y > M_PI*2) {
        rotation.y -= M_PI*2;
    }
    if (rotation.y < 0.) {
        rotation.y += 0.;
    }
    
    if (zoom >= -2.3) {
        zoom = -2.3;
        stopZoom(true);
        invalidateModelView();
        invalidateTransformedPos();
    }
    
}

}
}
