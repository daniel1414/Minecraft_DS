#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "My_Types.h"

class Camera
{
public:
    Camera();
    void movef32(int x, int y, int z);
    void movef32(vec3f32 position);
    void get_vectors(vec3f32* position, vec3f32* front, vec3f32* world_up);
    inline vec3f32 get_front() const {return m_front;}
    inline vec3f32 get_right() const {return m_right;}
    inline vec3f32 get_up() const {return m_up;}
    
    void update();
private:
    void recalculate_vectors();

    vec3f32 m_front;
    vec3f32 m_right;
    vec3f32 m_up;

    vec3f32 m_position;
    vec3f32 m_world_up;
    // fixed point 20.12
    int  m_yaw, m_pitch;
};

#endif /* _CAMERA_H_ */