#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "My_Types.h"
#include "Camera.h"

class Player
{
public:
    Player();
    void process_input();
    void get_camera_vectors(vec3f32* position, vec3f32* front, vec3f32* world_up);
    void update_camera();
private:

    vec3f32 m_position;
    Camera m_camera;
    float m_camera_height; // 20.12 fixed point
};

#endif /* _PLAYER_H_ */