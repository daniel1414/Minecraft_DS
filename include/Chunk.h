#pragma once

#include "Cube.h"

#define WORLD_SIZE_X    6 /* in chunks */
#define WORLD_SIZE_Z    6 /* in chunks */

#define CHUNK_SIZE_X    8
#define CHUNK_SIZE_Z    8
#define CHUNK_SIZE_Y    32

#define VISIBLE_CHUNK_DISTANCE 3
#define CUBE_DESTRUCTION_RADIUS inttof32(5)

#define TREE_HEIGHT 5

enum CHUNK_SIDE
{
    CHUNK_SIDE_FRONT,
    CHUNK_SIDE_RIGHT,
    CHUNK_SIDE_BACK,
    CHUNK_SIDE_LEFT,
    CHUNK_SIDE_FRONT_RIGHT,
    CHUNK_SIDE_BACK_RIGHT,
    CHUNK_SIDE_BACK_LEFT,
    CHUNK_SIDE_FRONT_LEFT,
    CHUNK_SIDE_NONE
};

class Chunk
{
public:
    // position is the top left corner in the chunk (x, z)
    Chunk(const Vec2& position, Cube** cubeInstances);
    ~Chunk();

    void plantOakTree(const Vec3& position);
    void setBlock(const Vec3& position, Cube* cubeInstance);
    void plantInitialPlants();
    // returns true if operation is over
    bool destroyCube(const Vec3& cameraPosition, const Vec3& cameraFront, int32 distance);
    
    //void draw(Camera* camera) const;
    void drawTerrain(Camera* camera) const;
    void drawPlants(Camera* camera) const;

    const Cube** getCubes() { return (const Cube**)m_cubes; }
    void setNeighbour(CHUNK_SIDE side, Chunk* chunk) { m_sideChunks[side] = chunk; }
    void updateDrawList();
private:
    void init();
    void deleteDrawList();
private:
    Vec2 m_position; /* position in f32 */
    Cube** m_cubeInstances = nullptr;
    // m_cubes contain pointers to the actual cubes
    Cube** m_cubes = nullptr;
    CubeNode* m_visibleCubes = nullptr;
    CubeNode* m_plants = nullptr;
    Chunk* m_sideChunks[8]; // front(z+), right(x+), back(z-), left(x-)
};