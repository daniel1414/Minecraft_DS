#include "Player.h"

#include <nds.h>
#include <nds/arm9/input.h>
#include <nds/arm9/videoGL.h>
#include <nds/arm9/background.h>
#include <nds/arm9/sprite.h>

#include "Log.h"

#include "center_cross.h"

Player::Player() : m_position({0, floattof32(1.0f), floattof32(2.0f)}), m_camera_height(floattof32(1.5f))
{
}

void Player::init()
{
    // camera 
    m_camera.init();
    m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);

    // pause backgroung
    m_pause_bg_sub = bgInitSub(2, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    dmaCopy(player_pause_optionsBitmap, bgGetGfxPtr(m_pause_bg_sub), player_pause_optionsBitmapLen);
    dmaCopy(player_pause_optionsPal, BG_PALETTE_SUB, player_pause_optionsPalLen);
    
    // center cross
    m_cross_gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_16Color);
    dmaCopy(center_crossTiles, m_cross_gfx, center_crossTilesLen);
    dmaCopy(center_crossPal, SPRITE_PALETTE, center_crossPalLen);
    oamSet(&oamMain, 0, 120, 88, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color, m_cross_gfx, -1, false, true, false, false, false);
}

void Player::process_input(int keys, const touchPosition& touch)
{
    process_key_input(keys);
    process_touch_input(touch);
}

void Player::start_playing()
{
    oamSet(&oamMain, 0, 120, 88, 0, 0, SpriteSize_16x16, SpriteColorFormat_16Color, m_cross_gfx, -1, false, false, false, false, false);
    m_return_status = PLAYER_RETURN_NO_RETURN;
    m_state = PLAYER_STATE_WALKING;
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
        case PLAYER_STATE_WALKING: // while just walking
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
                if(keys & KEY_START)
                {
                    m_state = PLAYER_STATE_PAUSE;
                    load_pause();
                }
                m_camera.movef32(m_position.x, m_position.y + m_camera_height, m_position.z);
            }
            break;
        }
        case PLAYER_STATE_EQUIPMENT:
        {
            break;
        }
        case PLAYER_STATE_CRAFTING:
        {
            break;
        }
        case PLAYER_STATE_PAUSE:
        {
            break;
        }
        default:
            break;
    }
}

void Player::process_touch_input(const touchPosition& touch)
{
    switch(m_state)
    {
        case PLAYER_STATE_WALKING:
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
        case PLAYER_STATE_EQUIPMENT:
        {
            break;
        }
        case PLAYER_STATE_CRAFTING:
        {
            break;
        }
        case PLAYER_STATE_PAUSE:
        {
            if(touch.px > 48 && touch.px < 208 && touch.py > 32 && touch.py < 64) // resume
            {
                m_state = PLAYER_STATE_WALKING;
                close_pause();
            }
            if(touch.px > 48 && touch.px < 208 && touch.py > 80 && touch.py < 112) // return to menu
            {
                oamSetHidden(&oamMain, 0, true);
                m_return_status = PLAYER_RETURN_MENU;
                close_pause();
            }
            if(touch.px > 48 && touch.px < 208 && touch.py > 128 && touch.py < 160)
            {
                oamSetHidden(&oamMain, 0, true);
                m_return_status = PLAYER_RETURN_EXIT;
            }
            break;
        }
        default:
            break;
    }
}

void Player::load_pause()
{
    // lcd main on top
    lcdMainOnTop();
    dmaCopy(player_pause_optionsBitmap, bgGetGfxPtr(m_pause_bg_sub), player_pause_optionsBitmapLen);
    dmaCopy(player_pause_optionsPal, BG_PALETTE_SUB, player_pause_optionsPalLen);

    videoBgEnableSub(2);
}

void Player::close_pause()
{
    // lcd main on bottom
    lcdMainOnBottom();
    videoBgDisableSub(2);
}