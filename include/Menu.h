#ifndef _MENU_H_
#define _MENU_H_

#include "nds.h"
#include "nds/arm9/input.h"

enum MENU_STATE
{
    MENU_START,
    MENU_NEW_WORLD,
    MENU_LOAD_WORLD,
    MENU_OPTIONS
};

enum MENU_RETURN_STATUS
{
    RETURN_NO_RETURN,
    RETURN_NEW_WORLD,
    RETURN_LOAD_WORLD,
    RETURN_EXIT
};

class Menu
{
public:
    void process_input(int keys_down, const touchPosition& touch);
    void process_key_input(int keys_down);
    void process_touch_input(const touchPosition& touch);

    inline MENU_RETURN_STATUS get_return_status() const { return m_return_status; }
    // is to be changed - no parameters, menu.h handles all pointers to bitmaps and palettes.
    void init(void* bitmap_ptr, unsigned int bitmap_len, void* palette_ptr, unsigned int palette_len,
              void* sub_bitmap_ptr, unsigned int sub_bitmap_len, void* sub_palette_ptr, unsigned int sub_palette_len);
    void show();
    void hide();
private:
    MENU_STATE m_state = MENU_START;
    MENU_RETURN_STATUS m_return_status = RETURN_NO_RETURN;

    int m_bg;
    int m_bg_sub;
    
    void* m_bmp_ptr;
    unsigned int m_bmp_len;
    void* m_pal_ptr;
    unsigned int m_pal_len;

    void* m_sub_bmp_ptr;
    unsigned int m_sub_bmp_len;
    void* m_sub_pal_ptr;
    unsigned int m_sub_pal_len;
};

#endif