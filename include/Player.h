#pragma once

#include <nds/arm9/math.h>

#include "Camera.h"
#include "World.h"

#define PLAYER_HEIGHT floattof32(1.5f)
#define PLAYER_RADIUS floattof32(0.2f)

enum class GAMEMODES
{
    CREATIVE,
    SURVIVAL
};

enum class GAMEPLAY_STATES
{
    EXPLORATION,
    INVENTORY
};

class Player
{
public:

    Player();

    void processKeyInput(uint32 input, uint32 timeStep, World* world);
    void processTouchInput(const touchPosition& input) { m_camera->processTouchInput(input); }

    void jump();

    Camera* getCamera() const { return m_camera; }
    const Vec3 getPosition() const { return m_camera->getPosition(); }
private:
    Camera* m_camera;
    GAMEPLAY_STATES m_state;
    GAMEMODES m_mode;
    int32 m_speed = floattof32(0.6f);
};