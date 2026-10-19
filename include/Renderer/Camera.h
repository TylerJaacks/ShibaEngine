//
// Created by Tyler Jaacks on 9/28/26.
//

#ifndef SHIBAENGINE_CAMERA_H
#define SHIBAENGINE_CAMERA_H

class Camera {
public:
    Camera();
    ~Camera();

    void SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void SetPosition(float x, float y, float z);
    void SetRotation(float pitch, float yaw, float roll);

private:
    float m_fieldOfView;
    float m_aspectRatio;
    float m_nearClip;
    float m_farClip;

    float m_positionX;
    float m_positionY;
    float m_positionZ;

    float m_pitch;
    float m_yaw;
    float m_roll;
};

#endif //SHIBAENGINE_CAMERA_H