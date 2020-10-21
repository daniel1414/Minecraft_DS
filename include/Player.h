#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "My_Types.h"
#include "Camera.h"

enum GAMESTATE
{
    WALKING,
    EQUIPMENT,
    CRAFTING,
    RETURN
};

class Player
{
public:
    Player();
    void init_camera();

    /* returns true if back to main menu */
    bool process_input(int keys, const touchPosition& touch);
    void update_camera() const;

    inline const vec3f32& get_position() const { return m_position; }
    inline const vec3f32& get_camera_front() const { return m_camera.get_front(); }
private:
    void process_key_input(int keys);
    bool process_touch_input(const touchPosition& touch);

    vec3f32 m_position;
    Camera m_camera;
    int m_camera_height; 

    touchPosition m_last_touch;
    GAMESTATE m_state;
};

#endif /* _PLAYER_H_ */