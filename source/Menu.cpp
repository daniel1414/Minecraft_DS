#include "Menu.h"

#include "nds/arm9/background.h"

#include "main_bg.h"
#include "main_options.h"

void Menu::process_input(int keys_down, const touchPosition& touch)
{
    process_key_input(keys_down);
    process_touch_input(touch);
}

void Menu::process_key_input(int keys_down)
{
    // TODO button selection
}

void Menu::process_touch_input(const touchPosition& touch)
{
    switch(m_state)
    {
        case MENU_START:
        {
            if(touch.px > 48 && touch.px < 208 && touch.py > 16 && touch.py < 48)
            {
                m_return_status = MENU_RETURN_NEW_WORLD;
            }
            else if(touch.px > 48 && touch.px < 208 && touch.py > 64 && touch.py < 96)
            {
                m_state = MENU_LOAD_WORLD;
                // TODO: how to choose a world stored somewhere
                m_return_status = MENU_RETURN_LOAD_WORLD;
            } 
            else if(touch.px > 48 && touch.px < 208 && touch.py > 112 && touch.py < 144)
            {
                m_state = MENU_OPTIONS;
            }
            else if(touch.px > 176 && touch.px < 240 && touch.py > 150 && touch.py < 176)
            {
                m_return_status = MENU_RETURN_EXIT;
            }
        }
    }
}

void Menu::init()
{
    m_bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    m_bg_sub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
}

void Menu::show()
{
    m_return_status = MENU_RETURN_NO_RETURN;
    vramSetBankA(VRAM_A_MAIN_BG);

	videoBgDisable(0);

    videoBgEnable(3);
    dmaCopy(main_optionsBitmap, bgGetGfxPtr(m_bg), main_optionsBitmapLen);
    dmaCopy(main_optionsPal, BG_PALETTE, main_optionsPalLen);

    videoBgEnableSub(3);
    dmaCopy(main_bgBitmap, bgGetGfxPtr(m_bg_sub), main_bgBitmapLen);
    dmaCopy(main_bgPal, BG_PALETTE_SUB, main_bgPalLen);

}

void Menu::hide()
{
    vramSetBankA(VRAM_A_TEXTURE_SLOT0);

    videoBgDisable(3);
    videoBgDisableSub(3);
    videoBgEnable(0);
}