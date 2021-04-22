#include "Camera.h"

#include <nds/arm9/math.h>
#include <nds/arm9/videoGL.h>

#include "Renderer.h"

Camera::Camera(const Vec3& position) : m_position(position) 
{   
    recalculateVectors();
};
/**
 * provide timeStep as f32
 */
void Camera::processKeyInput(uint32 input, int32 timeStep)
{
    Vec3 xAxis = {inttof32(1), 0, 0};
    Vec3 zAxis = {0, 0, inttof32(1)};
    Vec3 frontdotX = xAxis * dotf32(&m_front.x, &xAxis.x);
    Vec3 frontdotZ = zAxis * dotf32(&m_front.x, &zAxis.x);

    Vec3 horizontalFront = frontdotX + frontdotZ;
    normalizef32(&horizontalFront.x);

    if(input & KEY_LEFT)
    {
        m_position.x -= mulf32(mulf32(m_speed, timeStep), m_right.x);
        m_position.z -= mulf32(mulf32(m_speed, timeStep), m_right.z);
    }
    if(input & KEY_RIGHT)
    {
        m_position.x += mulf32(mulf32(m_speed, timeStep), m_right.x);
        m_position.z += mulf32(mulf32(m_speed, timeStep), m_right.z);
    }
    if(input & KEY_UP)
    {
        m_position.x -= mulf32(mulf32(m_speed, timeStep), horizontalFront.x);
        m_position.z -= mulf32(mulf32(m_speed, timeStep), horizontalFront.z);
    }
    if(input & KEY_DOWN)
    {
        m_position.x += mulf32(mulf32(m_speed, timeStep), horizontalFront.x);
        m_position.z += mulf32(mulf32(m_speed, timeStep), horizontalFront.z);
    }
    if(input & KEY_R)
    {
        m_position.y += mulf32(m_speed, timeStep);
    }
    if(input & KEY_L)
    {
        m_position.y -= mulf32(m_speed, timeStep);
    }
    //LOG("camPos (%d,%d,%d)", m_position.x >> 12, m_position.y >> 12, m_position.z >> 12);
}

void Camera::processTouchInput(const touchPosition& input)
{
    if(input.px != 0 || input.py != 0) // there is touch
    {
        if(m_lastX != 0 || m_lastY != 0) // no touch before
        {
            int32 offsetX = input.px - m_lastX;
            int32 offsetY = input.py - m_lastY;
            
            int8 signX = offsetX > 0 ? 1 : -1;
            int8 signY = offsetY > 0 ? 1 : -1;

            m_yaw += signX * offsetX * offsetX * m_touchSensitivity;
            m_pitch -= signY * offsetY * offsetY * m_touchSensitivity;
            
            if(m_pitch < degreesToAngle(-88))
                m_pitch = degreesToAngle(-88);
            else if(m_pitch > degreesToAngle(88))
                m_pitch = degreesToAngle(88);

            recalculateVectors();
        }
    }
    m_lastX = input.px;
    m_lastY = input.py;
}

void Camera::recalculateVectors()
{
    m_front.x = mulf32(cosLerp(-m_yaw), cosLerp(m_pitch));
    m_front.y = sinLerp(m_pitch);
    m_front.z = mulf32(sinLerp(-m_yaw), cosLerp(m_pitch));
    
    normalizef32(&m_front.x);

    Vec3 world_up = {0, inttof32(1), 0};
    crossf32(&world_up.x, &m_front.x, &m_right.x);
    normalizef32(&m_right.x);

    crossf32(&m_front.x, &m_right.x, &m_up.x);
    normalizef32(&m_up.x);
}

/**
 * Perspective Camera
 */
PerspectiveCamera::PerspectiveCamera(const Vec3& position, int fovYDeg, int32 aspectRatio, int32 near, int32 far)
    : Camera(position), m_fovY(degreesToAngle(fovYDeg)), m_asp(aspectRatio), m_near(near), m_far(far)
{ 
    // calculate fovX
    int32 ymax = mulf32(m_near, tanLerp(m_fovY >> 1)); // y/2
    int32 xmax = mulf32(m_asp, ymax); // x/2
    int32 d = sqrtf32(mulf32(m_near, m_near) + mulf32(xmax, xmax));
    m_fovX = mulf32(acosLerp(divf32(m_near, d)), inttof32(2));

    setGlStuff();
}

void PerspectiveCamera::processKeyInput(uint32 input, int32 timeStep)
{
    Camera::processKeyInput(input, timeStep);
    if(cullFrustum)
        recalculateNormals();
    setGlStuff();
}

bool PerspectiveCamera::isInFrustum(Vec3* position, int32 radius)
{
    for(int i = 0; i < 6; ++i)
    {
        int32 c = dotf32((int32*)position, &m_planeNormals[i].x) + m_planeDistances[i] + radius;
        if(c < 0){
            return false;
        }
    }
    return true;
}

void PerspectiveCamera::recalculateNormals()
{
    /* for matrix rotating and component retrieving */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // bottom plane normal
    s16 angle = degreesToAngle(-90) + (m_fovY >> 1);
    m_planeNormals[0] = calculateNormalVector(angle, m_right);
    m_planeDistances[0] = -dotf32(&m_position.x, &m_planeNormals[0].x);

    // top plane normal
    angle = degreesToAngle(90) - (m_fovY >> 1);
    m_planeNormals[2] = calculateNormalVector(angle, m_right);
    m_planeDistances[2] = -dotf32(&m_position.x, &m_planeNormals[2].x);

    // right plane normal
    angle = degreesToAngle(-90) + (m_fovX >> 1);
    m_planeNormals[1] = calculateNormalVector(angle, m_up);
    m_planeDistances[1] = -dotf32(&m_position.x, &m_planeNormals[1].x);

    // left plane normal
    angle = degreesToAngle(90) - (m_fovX >> 1);
    m_planeNormals[3] = calculateNormalVector(angle, m_up);
    m_planeDistances[3] = -dotf32(&m_position.x, &m_planeNormals[3].x);

    // front plane normal
    m_planeNormals[4] = {-m_front.x, -m_front.y, -m_front.z};
    Vec3 point = {m_position.x - mulf32(m_near, m_front.x), m_position.y - mulf32(m_near, m_front.y), m_position.z - mulf32(m_near, m_front.z)};
    m_planeDistances[4] = -dotf32(&point.x, &m_planeNormals[4].x);

    // back plane normal
    m_planeNormals[5] = m_front;
    point = {m_position.x - mulf32(m_far, m_front.x), m_position.y - mulf32(m_far, m_front.y), m_position.z - mulf32(m_far, m_front.z)};
    m_planeDistances[5] = -dotf32(&point.x, &m_planeNormals[5].x);

    glPopMatrix(1);
}

Vec3 PerspectiveCamera::calculateNormalVector(int32 angle, const Vec3& rotationAxis)
{
    glLoadIdentity();
    glRotatef32i(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    while(GFX_BUSY);
    Mat3 matrix = {MATRIX_READ_VECTOR[0], MATRIX_READ_VECTOR[1], MATRIX_READ_VECTOR[2],
	    MATRIX_READ_VECTOR[3], MATRIX_READ_VECTOR[4], MATRIX_READ_VECTOR[5],
	    MATRIX_READ_VECTOR[6], MATRIX_READ_VECTOR[7], MATRIX_READ_VECTOR[8] };
    return matrix * Vec3(-m_front.x, -m_front.y, -m_front.z);
}

void PerspectiveCamera::setGlStuff()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // TODO: implement on my own
    gluPerspectivef32(m_fovY, m_asp, m_near, m_far);
    // gluLookAt()
    GFX_FIFO = 0x19;
    GFX_FIFO = m_right.x;
	GFX_FIFO = m_up.x;
	GFX_FIFO = m_front.x;
	GFX_FIFO = m_right.y;
	GFX_FIFO = m_up.y;
	GFX_FIFO = m_front.y;
	GFX_FIFO = m_right.z;
	GFX_FIFO = m_up.z;
	GFX_FIFO = m_front.z;
	GFX_FIFO = -dotf32(&m_position.x, &m_right.x);
	GFX_FIFO = -dotf32(&m_position.x, &m_up.x);
	GFX_FIFO = -dotf32(&m_position.x, &m_front.x);

}

/**
 * don't you dare use this shit
 */
void OrthographicCamera::processKeyInput(uint32 input, int32 timeStep)
{
    Camera::processKeyInput(input, timeStep);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrthof32(m_position.x - m_halfWidth, m_position.x + m_halfWidth,
        m_position.y - m_halfHeight, m_position.y + m_halfHeight,
        m_position.z + m_frontDistance, m_position.z + m_size + m_frontDistance);
    
    glRotatef32i(m_yaw, 0, inttof32(1), 0);
    glRotatef32i(m_pitch, m_right.x, m_right.y, m_right.z);
}

