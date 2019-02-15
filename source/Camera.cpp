#include <radix/Camera.hpp>

#include <cmath>

namespace radix {

Camera::Camera(const float fovy, const float aspect, const float zNear, const float zFar) {
  this->fovy = fovy;
  this->aspect = aspect;
  this->zNear = zNear;
  this->zFar = zFar;
  calcProj();
}

void Camera::calcProj() {
  Matrix4f &m = projMatrix;
  m.setIdentity();
  if (perspective) {
    m[0] = (float) (1 / tan(fovy / 2)) / aspect;
    m[5] = (float) (1 / tan(fovy / 2));
    m[10] = (zNear + zFar) / (zNear - zFar);
    m[11] = -1;
    m[14] = (2 * zNear * zFar) / (zNear - zFar);
    m[15] = 0;
  } else {
    m[0] = 2 / (right - left);
    m[5] = 2 / (top - bottom);
    m[10] = -2 / (zFar - zNear);
    m[12] = (left - right) / (right - left);
    m[13] = (bottom - top) / (top - bottom);
    m[14] = (zNear - zFar) / (zFar - zNear);
  }
}


void Camera::calcView() {
  Matrix4f &m = viewMatrix;
  m = inverse(orientation.toMatrix());
  m.translate(-position);
  invViewMatrix = inverse(viewMatrix);
}

void Camera::getProjMatrix(Matrix4f &m) const {
  m = projMatrix;
}

void Camera::setProjMatrix(const Matrix4f &m) {
  projMatrix = m;
}

void Camera::getViewMatrix(Matrix4f &m) const {
  m = viewMatrix;
}

void Camera::setViewMatrix(const Matrix4f &m) {
  viewMatrix = m;
  invViewMatrix = inverse(viewMatrix);
}

void Camera::getInvViewMatrix(Matrix4f &m) const {
  m = invViewMatrix;
}

void Camera::setFovy(const float fovy) {
  this->fovy = fovy;
  calcProj();
}

float Camera::getFovy() const {
  return fovy;
}

void Camera::setAspect(const float aspect) {
  this->aspect = aspect;
  calcProj();
}

float Camera::getAspect() const {
  return aspect;
}

void Camera::setZNear(const float zNear) {
  this->zNear = zNear;
  calcProj();
}

float Camera::getZNear() const {
  return zNear;
}

void Camera::setZFar(const float zFar) {
  this->zFar = zFar;
  calcProj();
}

float Camera::getZFar() const {
  return zFar;
}

void Camera::setPerspective() {
  this->perspective = true;
  calcProj();
}

void Camera::setOrthographic() {
  this->perspective = false;
  calcProj();
}

void Camera::setBounds(const float left, const float right, const float bottom, const float top) {
  this->left = left;
  this->right = right;
  this->bottom = bottom;
  this->top = top;
  calcProj();
}

Vector3f Camera::getPosition() const {
  return position;
}

void Camera::setPosition(const Vector3f &p) {
  position = p;
  calcView();
}

Quaternion Camera::getOrientation() const {
  return orientation;
}

void Camera::setOrientation(const Quaternion &o) {
  orientation = o;
  calcView();
}

} /* namespace radix */
