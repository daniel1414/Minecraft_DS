#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include "nds.h"

#include "World.h"
#include "Player.h"
#include "Menu.h"

enum GAME_STATES
{
    MENU,
    GAMEPLAY,
    EXIT
};

class GameSystem
{
public:
    GameSystem();
    ~GameSystem();
    void run();

private:
    void process_input();
    // initialization functions
    void init_hardware();
    void load_textures();
    static void FPSTimerCallback();

    void deinit_hardware();
private:
    World m_world;
    Player m_player;
    static u8 m_frames;
    int* m_textures;
    GAME_STATES m_state;
    Menu m_menu;
};

#endif
