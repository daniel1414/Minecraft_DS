#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include "nds.h"

#include "World.h"


class GameSystem
{
public:
    GameSystem();
    ~GameSystem();
    void run();

private:
    static void FPSTimerCallback();
    // initialization functions
    void init_hardware();
    void load_textures();

private:
    World m_world;
    Player m_player;
    int* m_textures;
    static u8 m_frames;
};

#endif
