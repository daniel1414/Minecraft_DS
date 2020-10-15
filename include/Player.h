#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "My_Types.h"
#include "Camera.h"

class Player
{
public:
    Player();
    void process_input();

    void update_camera() const;

    inline vec3f32 get_position() const { return m_position; }
private:

    vec3f32 m_position;
    Camera m_camera;
    int m_camera_height; 

    touchPosition m_last_touch;
};

#endif /* _PLAYER_H_ */