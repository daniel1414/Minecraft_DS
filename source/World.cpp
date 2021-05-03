#include "World.h"

#include "stdlib.h"
#include "nds/arm9/videoGL.h"

#include "NoiseGenerator.h"
#include "Log.h"

struct TextureCoords
{
    Vec2 coords[3];
    operator Vec2*()
    {
        return coords;
    }
} textureCoords;

struct FaceOpacities
{
    bool opacities[7];
    operator bool*()
    {
        return opacities;
    }
} faceOpacities;

World::World()
{
    loadCubeInformation();
    initChunks();
}

World::~World()
{
    for(int i = 0; i < CUBE_TYPE_COUNT; ++i)
    {
        delete m_cubeInstances[i];
    }

    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            delete m_chunks[z * WORLD_SIZE_X + x];
        }
    }
}

void World::plantOakTree(const Vec3& position)
{
    Vec3 chunkIndex = {((position.x >> 12) + (WORLD_SIZE_X / 2) * CHUNK_SIZE_X) / CHUNK_SIZE_X, 0, ((position.z >> 12) + (WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) / CHUNK_SIZE_Z};
    m_chunks[chunkIndex.z * WORLD_SIZE_X + chunkIndex.x]->plantOakTree(position);
}

void World::destroyCube(const Vec3& cameraPosition, const Vec3& cameraDirection)
{
    Vec3 chunkIndex = {((cameraPosition.x >> 12) + (WORLD_SIZE_X / 2) * CHUNK_SIZE_X) / CHUNK_SIZE_X, 0, ((cameraPosition.z >> 12) + (WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) / CHUNK_SIZE_Z};
    m_chunks[chunkIndex.z * WORLD_SIZE_X + chunkIndex.x]->destroyCube(cameraPosition, cameraDirection, CUBE_DESTRUCTION_RADIUS);
}

void World::drawTerrain(Camera* camera) const
{
    bool isInFrustum;
    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            if(((camera->getPosition().x >> 12) - ((x - WORLD_SIZE_X / 2) * CHUNK_SIZE_X) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X)
                && ((((x + 1) - WORLD_SIZE_X / 2) * CHUNK_SIZE_X) - (camera->getPosition().x >> 12) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X)
                && ((camera->getPosition().z >> 12) - ((z - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X)
                && ((((z + 1) - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) - (camera->getPosition().z >> 12) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X))
            {
                /* check if any chunk corner is in view frustum */
                isInFrustum = true;
                /* Vec3 position = {inttof32((x - WORLD_SIZE_X / 2) * CHUNK_SIZE_X), inttof32(0), inttof32((z - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z)};
                int32 radius = inttof32(3);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.x += inttof32(CHUNK_SIZE_X);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.z += inttof32(CHUNK_SIZE_Z);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.x -= inttof32(CHUNK_SIZE_X);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.y += inttof32(CHUNK_SIZE_Y);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.z -= inttof32(CHUNK_SIZE_Z);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.x += inttof32(CHUNK_SIZE_X);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;
                position.z += inttof32(CHUNK_SIZE_Z);
                if(camera->isInFrustum(&position, radius))
                    isInFrustum = true;  */

                if(isInFrustum)
                {
                    m_chunks[z * WORLD_SIZE_X + x]->drawTerrain(camera);
                }
            }
        }
    }
}

void World::drawPlants(Camera* camera) const
{
    
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE | (1 << 11) | POLY_ID(0) | POLY_FORMAT_LIGHT0);

    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            if(((camera->getPosition().x >> 12) - ((x - WORLD_SIZE_X / 2) * CHUNK_SIZE_X) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X)
                && ((((x + 1) - WORLD_SIZE_X / 2) * CHUNK_SIZE_X) - (camera->getPosition().x >> 12) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X)
                && ((camera->getPosition().z >> 12) - ((z - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X)
                && ((((z + 1) - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) - (camera->getPosition().z >> 12) < VISIBLE_CHUNK_DISTANCE * CHUNK_SIZE_X))
            {
                m_chunks[z * WORLD_SIZE_X + x]->drawPlants(camera);
            }
        }
    }
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | (1 << 11) | POLY_ID(0) | POLY_FOG);
}

Chunk* World::getChunk(const Vec3& position)
{
    Vec3 chunkIndex = {((position.x >> 12) + (WORLD_SIZE_X / 2) * CHUNK_SIZE_X) / CHUNK_SIZE_X, 0, ((position.z >> 12) + (WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) / CHUNK_SIZE_Z};
    if(chunkIndex.z < WORLD_SIZE_Z && chunkIndex.x < WORLD_SIZE_X)
        return m_chunks[chunkIndex.z * WORLD_SIZE_X + chunkIndex.x];
    else
        return nullptr;
}

Cube* World::getCube(const Vec3& position)
{
    Chunk* chunk = getChunk(position);
    if(chunk != nullptr)
        return chunk->getCube(position);
    else
        return nullptr;
}

void World::loadCubeInformation()
{
    /* air cube */
    textureCoords = {Vec2{-1, -1}, Vec2{-1, -1}, Vec2{-1, -1}};
    faceOpacities = {false, false, false, false, false, false, false};
    m_cubeInstances[CUBE_TYPE_OFFSET_AIR] = new Cube(textureCoords, faceOpacities, false);
    /* grass cube */
    textureCoords = {Vec2{0, 0}, Vec2{16, 0}, Vec2{32, 0}};
    faceOpacities = {true, true, true, true, true, true, true};
    m_cubeInstances[CUBE_TYPE_OFFSET_GRASS] = new Cube(textureCoords, faceOpacities);
    /* dirt cube */
    textureCoords = {Vec2{0, 0}, Vec2{0, 0}, Vec2{0, 0}};
    m_cubeInstances[CUBE_TYPE_OFFSET_DIRT] = new Cube(textureCoords, faceOpacities);
    /* stone cube */
    textureCoords = {Vec2{48, 0}, Vec2{48, 0}, Vec2{48, 0}};
    m_cubeInstances[CUBE_TYPE_OFFSET_STONE] = new Cube(textureCoords, faceOpacities);
    /* cobblestone cube */
    textureCoords = {Vec2{64, 0}, Vec2{64, 0}, Vec2{64, 0}};
    m_cubeInstances[CUBE_TYPE_OFFSET_COBBLESTONE] = new Cube(textureCoords, faceOpacities);
    /* oak wood cube */
    textureCoords = {Vec2{0, 16}, Vec2{96, 0}, Vec2{0, 16}};
    m_cubeInstances[CUBE_TYPE_OFFSET_OAK_WOOD] = new Cube(textureCoords, faceOpacities);
    /* oak leaves cube */
    textureCoords = {Vec2{16, 16}, Vec2{16, 16}, Vec2{16, 16}};
    faceOpacities = {false, false, false, false, false, false, false};
    m_cubeInstances[CUBE_TYPE_OFFSET_OAK_LEAVES] = new Cube(textureCoords, faceOpacities);
    /* grass plant */
    textureCoords = {Vec2{32, 16}, Vec2{32, 16}, Vec2{32, 16}};
    m_cubeInstances[CUBE_TYPE_OFFSET_GRASS_PLANT] = new PlantCube(textureCoords, faceOpacities);
    /* rose plant */
    textureCoords = {Vec2{80, 0}, Vec2{80, 0}, Vec2{80, 0}};
    m_cubeInstances[CUBE_TYPE_OFFSET_ROSE] = new PlantCube(textureCoords, faceOpacities);
    /* daffodil plant */
    textureCoords = {Vec2{48, 16}, Vec2{48, 16}, Vec2{48, 16}};
    m_cubeInstances[CUBE_TYPE_OFFSET_DAFFODIL] = new PlantCube(textureCoords, faceOpacities);
    
    /* add cubes and their properties here */
}

void World::initChunks()
{
    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            m_chunks[z * WORLD_SIZE_X + x] = new Chunk({((x - WORLD_SIZE_X / 2) * CHUNK_SIZE_X) << 12, ((z - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) << 12}, m_cubeInstances);
        }
    }

    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            if(z != 0) // add back neighbour
            {
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_BACK, m_chunks[(z - 1) * WORLD_SIZE_X + x]);
                if(x != 0)
                    m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_BACK_LEFT, m_chunks[(z - 1) * WORLD_SIZE_X + x - 1]);
                if(x != WORLD_SIZE_X - 1)
                    m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_BACK_RIGHT, m_chunks[(z - 1) * WORLD_SIZE_X + x + 1]);
            }
            if(z != WORLD_SIZE_Z - 1)
            {
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_FRONT, m_chunks[(z + 1) * WORLD_SIZE_X + x]);
                if(x != 0)
                    m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_FRONT_LEFT, m_chunks[(z + 1) * WORLD_SIZE_X + x - 1]);
                if(x != WORLD_SIZE_X - 1)
                    m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_FRONT_RIGHT, m_chunks[(z + 1) * WORLD_SIZE_X + x + 1]);
            }
            if(x != 0)
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_LEFT, m_chunks[z * WORLD_SIZE_X + x - 1]);
            if(x != WORLD_SIZE_X - 1)
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_RIGHT, m_chunks[z * WORLD_SIZE_X + x + 1]);
        }
    }
    /* plant some plant */
    plantInitialPlants();

    updateChunks();
}

void World::updateChunks()
{
    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            m_chunks[z * WORLD_SIZE_X + x]->updateDrawList();
        }
    }
}

void World::plantInitialPlants()
{
    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            m_chunks[z * WORLD_SIZE_X + x]->plantInitialPlants();
        }
    }
}