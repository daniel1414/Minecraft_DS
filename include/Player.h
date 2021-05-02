#pragma once

#include "Camera.h"
#include <nds/arm9/math.h>

#define PLAYER_HEIGHT floattof32(1.5f)
#define PLAYER_FRONT_DIST floattof32(0.2f)
#define PLAYER_SIDE_DIST floattof32(0.3f)

class Player
{
public:

    Player();

    void processKeyInput(uint32 input, uint32 timeStep) { m_camera->processKeyInput(input, timeStep); }
    void processTouchInput(const touchPosition& input) { m_camera->processTouchInput(input); }

    Camera* getCamera() const { return m_camera; }
    const Vec3 getPosition() const { return m_camera->getPosition(); }
private:
    Camera* m_camera;

};