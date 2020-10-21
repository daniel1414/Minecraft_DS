#include "Player.h"

#include <nds.h>
#include <nds/arm9/input.h>
#include <nds/arm9/videoGL.h>

#include "Log.h"

Player::Player() : m_position({0, floattof32(1.0f), floattof32(2.0f)}), m_camera_height(floattof32(1.5f)), m_state(WALKING)
{
    m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);
}

void Player::init_camera()
{
    m_camera.init();
}

bool Player::process_input(int keys, const touchPosition& touch)
{
    process_key_input(keys);
    bool return_to_main_menu = process_touch_input(touch);

    return return_to_main_menu;
}

void Player::update_camera() const
{
    m_camera.update();
}

void Player::process_key_input(int keys)
{
    int speed = floattof32(0.2f);
    switch(m_state)
    {
        case WALKING: // while just walking
        {
            if(keys)
            {
                if(keys & KEY_LEFT)
                {
                    m_position.x -= mulf32(m_camera.get_right().x, speed);
                    m_position.z -= mulf32(m_camera.get_right().z, speed);
                }
                if(keys & KEY_RIGHT)
                {
                    m_position.x += mulf32(m_camera.get_right().x, speed);
                    m_position.z += mulf32(m_camera.get_right().z, speed);
                }
                if(keys & KEY_UP)
                {
                    m_position.x += mulf32(m_camera.get_front().x, speed);
                    m_position.z += mulf32(m_camera.get_front().z, speed);
                }
                if(keys & KEY_DOWN)
                {
                    m_position.x -= mulf32(m_camera.get_front().x, speed);
                    m_position.z -= mulf32(m_camera.get_front().z, speed);
                }
                if(keys & KEY_L)
                {
                    m_position.y -= 0.5 * speed;
                }
                if(keys & KEY_R)
                {
                    m_position.y += 0.5 * speed;
                }
                m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);
            }
            break;
        }
        case EQUIPMENT:
        {

            break;
        }
        case CRAFTING:
        {
            break;
        }
        default:
            break;
    }

}

bool Player::process_touch_input(const touchPosition& touch)
{
    bool return_to_main_menu = false;
    switch(m_state)
    {
        case WALKING:
        {
            if(touch.px || touch.py)
            {
                if(m_last_touch.px == 0 || m_last_touch.py == 0)
                {
                    m_last_touch = touch;
                } 
                else
                {
                    s16 pxoffset = touch.px - m_last_touch.px;
                    s16 pyoffset = touch.py - m_last_touch.py;
                    m_last_touch = touch;
                    m_camera.rotate(pxoffset, pyoffset);
                }
                LOG("Touch: (%u, %u)", touch.px, touch.py);
            } else {
                m_last_touch = {0, 0, 0, 0, 0, 0};
            }
            break;
        }
        case EQUIPMENT:
        {
            break;
        }
        case CRAFTING:
        {
            break;
        }
        default:
            break;
    }

    return return_to_main_menu;
}