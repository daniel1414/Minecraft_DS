#include "Menu.h"

#include "nds/arm9/background.h"

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
                m_return_status = RETURN_NEW_WORLD;
            }
        }
    }
}

void Menu::init(void* bitmap_ptr, unsigned int bitmap_len, void* palette_ptr, unsigned int palette_len,
                void* sub_bitmap_ptr, unsigned int sub_bitmap_len, void* sub_palette_ptr, unsigned int sub_palette_len)
{
    m_bg = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);
    m_bg_sub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0, 0);

    m_bmp_ptr = bitmap_ptr;
    m_bmp_len = bitmap_len;
    m_pal_ptr = palette_ptr;
    m_pal_len = palette_len;

    m_sub_bmp_ptr = sub_bitmap_ptr;
    m_sub_bmp_len = sub_bitmap_len;
    m_sub_pal_ptr = sub_palette_ptr;
    m_sub_pal_len = sub_palette_len;
}

void Menu::show()
{
	videoBgDisable(0);
	videoBgDisableSub(0);

    videoBgEnable(3);
    dmaCopy(m_bmp_ptr, bgGetGfxPtr(m_bg), m_bmp_len);
    dmaCopy(m_pal_ptr, BG_PALETTE, m_pal_len);

    videoBgEnableSub(3);
    dmaCopy(m_sub_bmp_ptr, bgGetGfxPtr(m_bg_sub), m_sub_bmp_len);
    dmaCopy(m_sub_pal_ptr, BG_PALETTE_SUB, m_sub_pal_len);

}

void Menu::hide()
{
    vramSetBankA(VRAM_A_TEXTURE_SLOT0);

    videoBgDisable(3);
    videoBgDisableSub(3);
    videoBgEnable(0);
    videoBgEnableSub(0);
}