#pragma once

#include <nds.h>

#include "Chunk.h"

class World
{
public:
    World();
    ~World();

    void plantOakTree(const Vec3& position);

    Cube* destroyCube(const Vec3& cameraPosition, const Vec3& cameraDirection);
    void placeCube(const Vec3& cameraPosition, const Vec3& cameraDirection, Cube* cube);

    void drawTerrain(Camera* camera) const;
    void drawPlants(Camera* camera) const;
    
    Chunk* getChunk(const Vec3& position);
    Cube* getCube(const Vec3& position);
    void updateChunks();
private:
    Cube* m_cubeInstances[CUBE_TYPE_COUNT];
    Chunk* m_chunks[WORLD_SIZE_Z * WORLD_SIZE_X];

private:
    void loadCubeInformation();
    void initChunks();
    void plantInitialPlants();
};
