#include "Player.h  "

Player::Player()
{
    m_camera = new PerspectiveCamera({inttof32(0), inttof32(CHUNK_SIZE_Y), inttof32(3)}, 60, floattof32(256.0f / 192.0f), floattof32(0.1f), inttof32(50));
    m_state = GAMEPLAY_STATES::EXPLORATION;
    m_mode = GAMEMODES::SURVIVAL;
}

void Player::processKeyInput(uint32 input, uint32 timeStep, World* world)
{
    switch(m_state)
    {
        case GAMEPLAY_STATES::EXPLORATION:
        {
            Vec3 xAxis = {inttof32(1), 0, 0};
            Vec3 zAxis = {0, 0, inttof32(1)};
            Vec3 front = m_camera->getFront();
            Vec3 frontdotX = xAxis * dotf32(&front.x, &xAxis.x);
            Vec3 frontdotZ = zAxis * dotf32(&front.x, &zAxis.x);

            Vec3 horizontalFront = frontdotX + frontdotZ;
            normalizef32(&horizontalFront.x);

            Vec3 camPosition = m_camera->getPosition();
            Vec3 camRight = m_camera->getRight();

            switch(m_mode)
            {
                case GAMEMODES::CREATIVE:
                {
                    if(input & KEY_LEFT)
                    {
                        camPosition.x -= mulf32(mulf32(m_speed, timeStep), camRight.x);
                        camPosition.z -= mulf32(mulf32(m_speed, timeStep), camRight.z);
                    }
                    if(input & KEY_RIGHT)
                    {
                        camPosition.x += mulf32(mulf32(m_speed, timeStep), camRight.x);
                        camPosition.z += mulf32(mulf32(m_speed, timeStep), camRight.z);
                    }
                    if(input & KEY_UP)
                    {
                        camPosition.x -= mulf32(mulf32(m_speed, timeStep), horizontalFront.x);
                        camPosition.z -= mulf32(mulf32(m_speed, timeStep), horizontalFront.z);
                    }
                    if(input & KEY_DOWN)
                    {
                        camPosition.x += mulf32(mulf32(m_speed, timeStep), horizontalFront.x);
                        camPosition.z += mulf32(mulf32(m_speed, timeStep), horizontalFront.z);
                    }
                    if(input & KEY_R)
                    {
                        camPosition.y += mulf32(m_speed, timeStep);
                    }
                    if(input & KEY_L)
                    {
                        camPosition.y -= mulf32(m_speed, timeStep);
                    }
                    break;
                }
                case GAMEMODES::SURVIVAL:
                {
                    Cube* cube1 = nullptr;
                    Cube* cube2 = nullptr;
                    Vec3 futurePosition;

                    if(input & KEY_LEFT)
                    {
                        futurePosition = {camPosition.x - mulf32(mulf32(m_speed, timeStep), camRight.x), camPosition.y, camPosition.z - mulf32(mulf32(m_speed, timeStep), camRight.z)};
                        cube1 = world->getCube({futurePosition.x, futurePosition.y - CUBE_SIZE, futurePosition.z});
                        cube2 = world->getCube(futurePosition);
                        if((cube1 == nullptr || !cube1->isSolid())
                            && (cube2 == nullptr || !cube2->isSolid()))
                        {
                            camPosition = futurePosition;
                        }
                    }
                    if(input & KEY_RIGHT)
                    {
                        futurePosition = {camPosition.x + mulf32(mulf32(m_speed, timeStep), camRight.x), camPosition.y, camPosition.z + mulf32(mulf32(m_speed, timeStep), camRight.z)};
                        cube1 = world->getCube({futurePosition.x, futurePosition.y - CUBE_SIZE, futurePosition.z});
                        cube2 = world->getCube(futurePosition);
                        if((cube1 == nullptr || !cube1->isSolid())
                            && (cube2 == nullptr || !cube2->isSolid()))
                        {
                            camPosition = futurePosition;
                        }
                    }
                    if(input & KEY_UP)
                    {
                        futurePosition = {camPosition.x - mulf32(mulf32(m_speed, timeStep), horizontalFront.x), camPosition.y, camPosition.z - mulf32(mulf32(m_speed, timeStep), horizontalFront.z)};
                        cube1 = world->getCube({futurePosition.x, futurePosition.y - CUBE_SIZE, futurePosition.z});
                        cube2 = world->getCube(futurePosition);
                        if((cube1 == nullptr || !cube1->isSolid())
                            && (cube2 == nullptr || !cube2->isSolid()))
                        {
                            camPosition = futurePosition;
                        }
                    }
                    if(input & KEY_DOWN)
                    {
                        futurePosition = {camPosition.x + mulf32(mulf32(m_speed, timeStep), horizontalFront.x), camPosition.y, camPosition.z + mulf32(mulf32(m_speed, timeStep), horizontalFront.z)};
                        cube1 = world->getCube({futurePosition.x, futurePosition.y - CUBE_SIZE, futurePosition.z});
                        cube2 = world->getCube(futurePosition);
                        if((cube1 == nullptr || !cube1->isSolid())
                            && (cube2 == nullptr || !cube2->isSolid()))
                        {
                            camPosition = futurePosition;
                        }
                    }
                    if(input & KEY_R)
                    {
                        camPosition.y += mulf32(m_speed, timeStep);
                    }
                    if(input & KEY_L)
                    {
                        camPosition.y -= mulf32(m_speed, timeStep);
                    }
                    break;
                }
            }
            
            m_camera->setPosition(camPosition);
            LOG("camPos (%d,%d,%d)", camPosition.x >> 12, camPosition.y >> 12, camPosition.z >> 12);

            break;
        }
        case GAMEPLAY_STATES::INVENTORY:

            break;
        default:
            break;
    }
}