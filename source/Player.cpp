#include "Player.h"

#include "Chunk.h"

Player::Player()
{
    m_camera = new PerspectiveCamera({inttof32(0), inttof32(CHUNK_SIZE_Y), inttof32(3)}, 60, floattof32(256.0f / 192.0f), floattof32(0.1f), inttof32(50));
}