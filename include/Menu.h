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
    MENU_RETURN_NO_RETURN,
    MENU_RETURN_NEW_WORLD,
    MENU_RETURN_LOAD_WORLD,
    MENU_RETURN_EXIT
};

class Menu
{
public:
    void process_input(int keys_down, const touchPosition& touch);
    void process_key_input(int keys_down);
    void process_touch_input(const touchPosition& touch);

    inline MENU_RETURN_STATUS get_return_status() const { return m_return_status; }
    // is to be changed - no parameters, menu.h handles all pointers to bitmaps and palettes.
    void init();
    void show();
    void hide();
private:
    MENU_STATE m_state = MENU_START;
    MENU_RETURN_STATUS m_return_status = MENU_RETURN_NO_RETURN;

    int m_bg;
    int m_bg_sub;
    
};

#endif