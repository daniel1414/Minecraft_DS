#pragma once

#include <nds.h>

#include "Chunk.h"

#define VISIBLE_CHUNK_DISTANCE 3

class World
{
public:
    World();
    ~World();

    //void generate_terrain(int* textures);

    void destroyCube(const Vec3& cameraPosition, const Vec3& cameraDirection);

    void drawTerrain(Camera* camera) const;
private:
    Cube* m_cubeInstances[CUBE_TYPE_COUNT];
    Chunk* m_chunks[WORLD_SIZE_Z * WORLD_SIZE_X];

private:
    void loadCubeInformation();
    void initChunks();
};
