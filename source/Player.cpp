#include "Player.h"

#include <nds.h>
#include <nds/arm9/input.h>
#include <nds/arm9/videoGL.h>

#include "Log.h"

Player::Player() : m_position({0, floattof32(1.0f), floattof32(2.0f)}), m_camera_height(floattof32(1.5f))
{
    m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);
}

void Player::init_camera()
{
    m_camera.init();
}

void Player::process_input()
{
    scanKeys();
    int keys = keysHeld();
    int speed = floattof32(0.2f);
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
    touchPosition touch;
    touchRead(&touch);
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
}

void Player::update_camera() const
{
    m_camera.update();
}
