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

/* void World::generate_terrain(int* textures)
{
    // the top-most layer
    vec2f offset = {0.0, 0.0};
    for(int z = 0; z < m_width; z++)
	{
        for(int x = 0; x < m_width; x++)
        {
            float n = NoiseGenerator::noise2D(offset);
            Logger::Log("noise in (%d, %d) = %f", x, z, n);
            m_cubes[z * m_width + x].move({inttof32(x), inttof32((int)(n * 10)), inttof32(z)});
            m_cubes[z * m_width + x].load_textures(textures);

            offset.x += 0.05;

        }
        offset.x = 0.0;
        offset.y += 0.05;
	}
} */

void World::destroyCube(const Vec3& cameraPosition, const Vec3& cameraDirection)
{
    Vec3 chunkIndex = {((cameraPosition.x >> 12) + (WORLD_SIZE_X / 2) * CHUNK_SIZE_X) / CHUNK_SIZE_X, 0, ((cameraPosition.z >> 12) + (WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z) / CHUNK_SIZE_Z};
    LOG("chunk idx (%d, %d, %d)", chunkIndex.x, chunkIndex.y, chunkIndex.z);
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
                    m_chunks[z * WORLD_SIZE_X + x]->draw();
                }
            }
        }
    }
}

void World::loadCubeInformation()
{
    /* for(int i = 0; i < CUBE_TYPE_COUNT; ++i)
    {
        m_cubeInstances = (Cube*)malloc(CUBE_TYPE_COUNT * sizeof(Cube));
    } */

    /* air cube */
    textureCoords = {Vec2{-1, -1}, Vec2{-1, -1}, Vec2{-1, -1}};
    faceOpacities = {false, false, false, false, false, false, true};
    m_cubeInstances[CUBE_TYPE_OFFSET_AIR] = new Cube(textureCoords, faceOpacities);
    /* grass cube */
    textureCoords = {Vec2{0, 0}, Vec2{16, 0}, Vec2{32, 0}};
    faceOpacities = {true, true, true, true, true, true, false};
    m_cubeInstances[CUBE_TYPE_OFFSET_GRASS] = new Cube(textureCoords, faceOpacities);
    /* dirt cube */
    textureCoords = {Vec2{0, 0}, Vec2{0, 0}, Vec2{0, 0}};
    m_cubeInstances[CUBE_TYPE_OFFSET_DIRT] = new Cube(textureCoords, faceOpacities);

    /* add cubes and their properties here */
}

void World::initChunks()
{
    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            m_chunks[z * WORLD_SIZE_X + x] = new Chunk({inttof32((x - WORLD_SIZE_X / 2) * CHUNK_SIZE_X), inttof32((z - WORLD_SIZE_Z / 2) * CHUNK_SIZE_Z)}, m_cubeInstances);
        }
    }

    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            if(z != 0) // add back neighbour
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_BACK, m_chunks[(z - 1) * WORLD_SIZE_X + x]);
            if(z != WORLD_SIZE_Z - 1)
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_FRONT, m_chunks[(z + 1) * WORLD_SIZE_X + x]);
            if(x != 0)
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_LEFT, m_chunks[z * WORLD_SIZE_X + x - 1]);
            if(x != WORLD_SIZE_X - 1)
                m_chunks[z * WORLD_SIZE_X + x]->setNeighbour(CHUNK_SIDE_RIGHT, m_chunks[z * WORLD_SIZE_X + x + 1]);
        }
    }

    for(int z = 0; z < WORLD_SIZE_Z; ++z)
    {
        for(int x = 0; x < WORLD_SIZE_X; ++x)
        {
            m_chunks[z * WORLD_SIZE_X + x]->updateDrawList();
        }
    }
    
}