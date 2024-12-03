#include "camera.h"

#include "../common/constants.h"

Camera::Camera() : x(0), y(0), width(WINDOW_WIDTH), height(WINDOW_HEIGHT), zoom(1.0f){}

float Camera::getX() const {
    return x;
}

float Camera::getY() const {
    return y;
}

int Camera::getWidth() const {
    return static_cast<int>(width / zoom);
}

int Camera::getHeight() const {
    return static_cast<int>(height / zoom);
}

float Camera::getZoom() const {
    return zoom;
}

void Camera::setZoom(float zoom) {
    this->zoom = zoom;
}

void Camera::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void Camera::setView(float x, float y, int width, int height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}