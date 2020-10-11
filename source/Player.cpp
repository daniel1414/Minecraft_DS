#include "Player.h"

#include <nds.h>
#include <nds/arm9/input.h>
#include <nds/arm9/videoGL.h>

#include "Log.h"

Player::Player() : m_position({0, 0, floattof32(2.0f)}), m_camera_height(floattof32(1.5f))
{
    m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);
}
void Player::process_input()
{
    scanKeys();
    int keys = keysHeld();
    int speed = floattof32(0.5f);
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
        m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);
    }
}

void Player::get_camera_vectors(vec3f32* position, vec3f32* front, vec3f32* world_up)
{
    m_camera.get_vectors(position, front, world_up);
    LOG("Position in get_camera_vectors: (%d, %d, %d)", position->x >> 12, position->y >> 12, position->z >> 12);
    //LOG("Position in get_camera_vectors: (%d, %d, %d)", p, f, w);
}

void Player::update_camera()
{
    m_camera.update();
}
