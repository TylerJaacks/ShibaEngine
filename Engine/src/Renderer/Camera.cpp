#include <Renderer/Camera.h>

Camera::Camera() : m_fieldOfView(0),
                    m_aspectRatio(0),
                    m_nearClip(0),
                    m_farClip(0),
                    m_positionX(0),
                    m_positionY(0),
                    m_positionZ(0),
                    m_pitch(0), m_yaw(0),
                    m_roll(0) {
}

Camera::~Camera() {

}

void Camera::SetPerspective(const float fov, const float aspectRatio, const float nearPlane, const float farPlane) {
    m_fieldOfView = fov;
    m_aspectRatio = aspectRatio;
    m_nearClip = nearPlane;
    m_farClip = farPlane;
}

void Camera::SetPosition(const float x, const float y, const float z) {
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

void Camera::SetRotation(const float pitch, const float yaw, const float roll) {
    m_pitch = pitch;
    m_yaw = yaw;
    m_roll = roll;
}
