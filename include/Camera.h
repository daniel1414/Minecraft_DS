#pragma once

#include <nds.h>

#include "Log.h"

extern bool cullFrustum;

class Camera
{
public:
    Camera(const Vec3& position);
    virtual ~Camera() = default;

    virtual void processKeyInput(uint32 input, int32 timeStep);
    virtual void processTouchInput(const touchPosition& input);

    virtual bool isInFrustum(Vec3* position, int32 radius) = 0;

    const Vec3& getPosition() const { return m_position; }
    const Vec3& getFront() const { return m_front; }
    virtual const Vec3* getPlaneNormals() const = 0;

private:
    void recalculateVectors();
protected:
    Vec3 m_position;

    Vec3 m_front; // facing outward the camera
    Vec3 m_up = {0, inttof32(1), 0};
    Vec3 m_right = {inttof32(1), 0, 0};

    int32 m_speed = floattof32(0.6f);

    // stored as libnds angle
    int16 m_yaw = degreesToAngle(-90), m_pitch = degreesToAngle(0);
    uint16 m_lastX = 0, m_lastY = 0;
    uint8 m_touchSensitivity = 2;
};



class PerspectiveCamera: public Camera
{
public:
    PerspectiveCamera(const Vec3& position, int fovYDeg, int32 aspectRatio, int32 near, int32 far);
    
    void processKeyInput(uint32 input, int32 timeStep) override;

    bool isInFrustum(Vec3* position, int32 radius) override;

    const Vec3* getPlaneNormals() const override { return m_planeNormals; }
private:
    Vec3 calculateNormalVector(int32 angle, const Vec3& rotationAxis);
    void recalculateNormals();
    void setGlStuff();
private:
    int32 m_fovY, m_fovX, m_asp, m_near, m_far;
    Vec3 m_planeNormals[6]; // bottom, right, top, left, front, back facing inward
    int32 m_planeDistances[6];

};



class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3& position, int32 width, int32 height, int32 size)
        : Camera(position), m_halfWidth(divf32(width, inttof32(2))), m_halfHeight(divf32(height, inttof32(2))), 
        m_size(size) {}
    
    void processKeyInput(uint32 input, int32 timeStep) override;
private:
    int32 m_halfWidth, m_halfHeight, m_size;
    int32 m_frontDistance = inttof32(1);
};

