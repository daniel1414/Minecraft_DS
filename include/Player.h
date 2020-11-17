#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <nds.h>

#include "My_Types.h"
#include "Camera.h"

#include "player_pause_options.h"

enum PLAYER_STATE
{
    PLAYER_STATE_WALKING,
    PLAYER_STATE_EQUIPMENT,
    PLAYER_STATE_CRAFTING,
    PLAYER_STATE_PAUSE
};

enum PLAYER_RETURN_STATUS
{
    PLAYER_RETURN_NO_RETURN,
    PLAYER_RETURN_MENU,
    PLAYER_RETURN_EXIT
};

class Player
{
public:
    Player();
    void init();

    void process_input(int keys, const touchPosition& touch);
    void process_key_input(int keys);
    void process_touch_input(const touchPosition& touch);

    void update_camera() const;
    void start_playing();
    void load_pause();
    void close_pause();

    inline const vec3f32& get_position() const { return m_position; }
    inline const vec3f32& get_camera_front() const { return m_camera.get_front(); }
    inline PLAYER_RETURN_STATUS get_return_status() const { return m_return_status; } 
private:

    vec3f32 m_position;
    Camera m_camera;
    int m_camera_height; 

    touchPosition m_last_touch;
    PLAYER_STATE m_state = PLAYER_STATE_WALKING;
    PLAYER_RETURN_STATUS m_return_status = PLAYER_RETURN_NO_RETURN;

    int m_pause_bg_sub;
    u16* m_cross_gfx;
};

#endif /* _PLAYER_H_ */