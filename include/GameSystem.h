#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include "nds.h"

#include "World.h"
#include "Player.h"

class GameSystem
{
public:
    GameSystem();
    ~GameSystem();
    void run();
    int* get_textures() const {return m_textures;}
private:
    static void FPSTimerCallback();
    // initialization functions
    void init_hardware();
    void load_textures();

private:
    World m_world;
    Player m_player;
    static u8 m_frames;
    int* m_textures;
};

#endif
