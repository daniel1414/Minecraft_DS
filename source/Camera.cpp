#include "Camera.h"

#include <nds.h>
#include <nds/arm9/videoGL.h>

#include "Log.h"

Camera::Camera() : m_front({0, 0, floattof32(-0.97)}), m_right({floattof32(0.97f), 0, 0}), m_up({0, floattof32(0.97f), 0}),
    m_position({0, 0, floattof32(2.0f)}), m_world_up({0, floattof32(0.97f), 0}), m_yaw(0), m_pitch(90)
{
    MATRIX_CONTROL = GL_PROJECTION;
    glLoadIdentity();
    gluPerspective(70, (256.0f / 192.0f), 0.1, 40);

    LOG("Camera position initialized! (%d, %d, %d)", m_position.x >> 12, m_position.y >> 12, m_position.z >> 12);
}

void Camera::movef32(int x, int y, int z)
{
    // TODO: saturation
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
    //recalculate_vectors();
}

void Camera::movef32(vec3f32 position)
{
    m_position = position;
    recalculate_vectors();
}

void Camera::get_vectors(vec3f32* position, vec3f32* front, vec3f32* world_up)
{
    //LOG("position in get_vectors(%d, %d, %d)", position->x, position->y, position->z);
    position = &m_position;
    front = &m_front;
    world_up = &m_world_up;
    LOG("position in get_vectors(%d, %d, %d)", position->x >> 12, position->y >> 12, position->z >> 12);
}

// private functions

void Camera::recalculate_vectors()
{
    vec3f32 front = {0, 0, 0};
    // recalculate the front vector
    front.x = cosLerp(degreesToAngle(m_yaw * 3.14 / 180)) * cosLerp(degreesToAngle(m_pitch * 3.14 / 180));
    front.y = sinLerp(degreesToAngle(m_pitch * 3.14 / 180));
    front.z = sinLerp(degreesToAngle(m_yaw * 3.14 / 180)) * cosLerp(degreesToAngle(m_pitch * 3.14 / 180));
    LOG("front 1: (%d, %d, %d)", front.x, front.y, front.z);
    // fix the fixed notation
    front.x = floattof32(fixedToFloat(front.x, 24));
    front.y = floattof32(fixedToFloat(front.y, 12));
    front.z = floattof32(fixedToFloat(front.z, 24));
    LOG("front 2: (%d, %d, %d)", front.x, front.y, front.z);
    // normalize 
    normalize_vec3f32(front);
    LOG("front 3: (%d, %d, %d)", front.x, front.y, front.z);
    m_front = front;
    LOG("m_front 1: (%d, %d, %d)", m_front.x, m_front.y, m_front.z);
    // recalculate the right vector
    m_right = cross_vec3f32(m_front, m_world_up);
    normalize_vec3f32(m_right);
    // recalculate the up vector
    m_up = cross_vec3f32(m_right, m_front);
    normalize_vec3f32(m_up);
    
    LOG("new front (%d, %d, %d)", m_front.x, m_front.y, m_front.z);
    LOG("new right (%d, %d, %d)", m_right.x, m_right.y, m_right.z);
    LOG("new up (%d, %d, %d)", m_up.x, m_up.y, m_up.z);
    
}

void Camera::update()
{
    MATRIX_CONTROL = GL_MODELVIEW;
    MATRIX_PUSH = 1;
    //MATRIX_IDENTITY = 1;
    gluLookAtf32(   m_position.x, m_position.y, m_position.z,
                    m_position.x + m_front.x, m_position.y + m_front.y, m_position.z + m_front.z,
                    m_world_up.x, m_world_up.y, m_world_up.z);

    LOG("Camera pos: (%d, %d, %d)", m_position.x >> 12, m_position.y >> 12, m_position.z >> 12);
}
