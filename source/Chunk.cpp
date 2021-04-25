#include "Chunk.h"
#include "NoiseGenerator.h"

Chunk::Chunk(const Vec2& position, Cube** cubeInstances) : m_position(position), m_cubeInstances(cubeInstances)
{
    init();
    updateDrawList();
}

Chunk::~Chunk()
{
    delete[] m_cubes;
    deleteDrawList();
}

void Chunk::updateDrawList()
{
    deleteDrawList();
    m_visibleCubes = new CubeNode();
    
    CubeNode* head = m_visibleCubes;

    for(int y = 0; y < CHUNK_SIZE_Y; ++y)
    {
        for(int z = 0; z < CHUNK_SIZE_Z; ++z)
        {
            for(int x = 0; x < CHUNK_SIZE_X; ++x)
            {
                if(!m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_NONE))
                {
                    Vec3 cubePosition = {m_position.x + inttof32(x), inttof32(y), m_position.y + inttof32(z)};
                    // check bottom (y - 1)
                    if(y != 0 && !m_cubes[(y - 1) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_TOP))
                    {
                        if(head->position.y == inttof32(-1)) 
                        {
                            head->position = cubePosition;
                            head->cube = m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x];
                            head->next = new CubeNode();
                        }
                        head->visibleFaces =  (head->visibleFaces | 1);
                    }
                    // check front (z + 1)
                    if((z == CHUNK_SIZE_Z - 1 && ((m_position.y >> 12) + CHUNK_SIZE_Z < WORLD_SIZE_Z * CHUNK_SIZE_Z / 2)
                        && (m_sideChunks[CHUNK_SIDE_FRONT] == nullptr || !m_sideChunks[CHUNK_SIDE_FRONT]->getCubes()[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + 0 * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_BACK))) 
                        || !m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + (z + 1) * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_BACK))
                    {
                        if(head->position.y == inttof32(-1)) 
                        {
                            head->position = cubePosition;
                            head->cube = m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x];
                            head->next = new CubeNode();
                        }
                        head->visibleFaces =  (head->visibleFaces | (1 << 1));
                    }
                    // check right (x + 1)
                    if((x == CHUNK_SIZE_X - 1 && ((m_position.x >> 12) + CHUNK_SIZE_X < WORLD_SIZE_X * CHUNK_SIZE_X / 2)
                        && (m_sideChunks[CHUNK_SIDE_RIGHT] == nullptr || !m_sideChunks[CHUNK_SIDE_RIGHT]->getCubes()[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + 0]->isOpaque(CUBE_FACE_LEFT))) 
                        || !m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x + 1]->isOpaque(CUBE_FACE_LEFT))
                    {
                        if(head->position.y == inttof32(-1)) 
                        {
                            head->position = cubePosition;
                            head->cube = m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x];
                            head->next = new CubeNode();
                        }
                        head->visibleFaces = (head->visibleFaces | (1 << 2));
                    }
                    // check back
                    if((z == 0 && ((m_position.y >> 12) + (WORLD_SIZE_X / 2) * CHUNK_SIZE_Z > 0)
                        && (m_sideChunks[CHUNK_SIDE_BACK] == nullptr || !m_sideChunks[CHUNK_SIDE_BACK]->getCubes()[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + (CHUNK_SIZE_Z - 1) * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_FRONT))) 
                        || !m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + (z - 1) * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_FRONT))
                    {
                        if(head->position.y == inttof32(-1)) 
                        {
                            head->position = cubePosition;
                            head->cube = m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x];
                            head->next = new CubeNode();
                        }
                        head->visibleFaces = (head->visibleFaces | (1 << 3));
                    }
                    // check left
                    if((x == 0 && ((m_position.x >> 12) + (WORLD_SIZE_X / 2) * CHUNK_SIZE_X > 0)
                        && (m_sideChunks[CHUNK_SIDE_LEFT] == nullptr || !m_sideChunks[CHUNK_SIDE_LEFT]->getCubes()[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + CHUNK_SIZE_X - 1]->isOpaque(CUBE_FACE_RIGHT))) 
                        || !m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x - 1]->isOpaque(CUBE_FACE_RIGHT))
                    {
                        if(head->position.y == inttof32(-1)) 
                        {
                            head->position = cubePosition;
                            head->cube = m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x];
                            head->next = new CubeNode();
                        }
                        head->visibleFaces = (head->visibleFaces |(1 << 4));
                    }
                    // check top
                    if(y == CHUNK_SIZE_Y - 1
                        || !m_cubes[(y + 1) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x]->isOpaque(CUBE_FACE_BOTTOM))
                    {
                        if(head->position.y == inttof32(-1))
                        {
                            head->position = cubePosition;
                            head->cube = m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x];
                            head->next = new CubeNode();
                        }
                        head->visibleFaces = (head->visibleFaces | (1 << 5));
                    }

                    if(head->next != nullptr)
                    {
                        head = head->next;
                    }
                }
            }
        }
    }
}

bool Chunk::destroyCube(const Vec3& cameraPosition, const Vec3& cameraFront, int32 distance)
{
    bool result = false;
    /* position in the chunk */
    Vec3 playerPosition = {cameraPosition.x - m_position.x, cameraPosition.y, cameraPosition.z - m_position.y};
    Vec3 rayPosition = playerPosition;
    Vec3 step = (Vec3)cameraFront * floattof32(0.05f);
    Vec3 lastRayPosition = rayPosition;
    while(!result) // add constraints
    {
        if((rayPosition.x >> 12) < 0)
            return m_sideChunks[CHUNK_SIDE_LEFT]->destroyCube({m_position.x + rayPosition.x, rayPosition.y, m_position.y + rayPosition.z}, cameraFront, distance - Vec3::dist(playerPosition, rayPosition));
        if((rayPosition.x >> 12) > CHUNK_SIZE_X - 1)
            return m_sideChunks[CHUNK_SIDE_RIGHT]->destroyCube({m_position.x + rayPosition.x, rayPosition.y, m_position.y + rayPosition.z}, cameraFront, distance - Vec3::dist(playerPosition, rayPosition));
        if((rayPosition.z >> 12) < 0)
            return m_sideChunks[CHUNK_SIDE_BACK]->destroyCube({m_position.x + rayPosition.x, rayPosition.y, m_position.y + rayPosition.z}, cameraFront, distance - Vec3::dist(playerPosition, rayPosition));
        if((rayPosition.z >> 12) > CHUNK_SIZE_Z - 1)
            return m_sideChunks[CHUNK_SIDE_FRONT]->destroyCube({m_position.x + rayPosition.x, rayPosition.y, m_position.y + rayPosition.z}, cameraFront, distance - Vec3::dist(playerPosition, rayPosition));

        if(((rayPosition.y >> 12) < CHUNK_SIZE_Y && (rayPosition.y >> 12) > -1) &&
            ((rayPosition.z >> 12) < CHUNK_SIZE_Z && (rayPosition.z >> 12) > -1) && 
            ((rayPosition.x >> 12) < CHUNK_SIZE_X && (rayPosition.x >> 12) > -1)) // check if position does not exceed the chunk limits
        {
            if(!m_cubes[(rayPosition.y >> 12) * CHUNK_SIZE_Z * CHUNK_SIZE_X + (rayPosition.z >> 12) * CHUNK_SIZE_X + (rayPosition.x >> 12)]->isOpaque(CUBE_FACE_NONE))
            {
                m_cubes[(rayPosition.y >> 12) * CHUNK_SIZE_Z * CHUNK_SIZE_X + (rayPosition.z >> 12) * CHUNK_SIZE_X + (rayPosition.x >> 12)] = m_cubeInstances[CUBE_TYPE_OFFSET_AIR];
                result = true;
                if((rayPosition.x >> 12) == 0)
                    m_sideChunks[CHUNK_SIDE_LEFT]->updateDrawList();
                else if((rayPosition.x >> 12 == CHUNK_SIZE_X - 1))
                    m_sideChunks[CHUNK_SIDE_RIGHT]->updateDrawList();
                if((rayPosition.z >> 12) == 0)
                    m_sideChunks[CHUNK_SIDE_BACK]->updateDrawList();
                else if((rayPosition.z >> 12) == CHUNK_SIZE_Z - 1)
                    m_sideChunks[CHUNK_SIDE_FRONT]->updateDrawList();
                updateDrawList();
            }
        }
        while(((rayPosition.x >> 12) == (lastRayPosition.x >> 12)) && ((rayPosition.y >> 12) == (lastRayPosition.y >> 12)) && ((rayPosition.z >> 12) == (lastRayPosition.z >> 12)) && !result)
        {
            lastRayPosition = rayPosition;
            rayPosition += step;
            if(Vec3::dist(rayPosition, playerPosition) > distance)
                result = true;
        }
        lastRayPosition = rayPosition;
        
    }
    return result;
}

void Chunk::draw(Camera* camera) const
{
    const Vec3& camPosition = camera->getPosition();
    CubeNode* head = m_visibleCubes;
    while(head->next != nullptr)
    {
        if(head->visibleFaces & 1) // bottom
        {
            if(camPosition.y < head->position.y)
                head->cube->drawFace(head->position, CUBE_FACE_BOTTOM);
        }
        if(head->visibleFaces & (1 << 5)) // top
        {
            if(camPosition.y > head->position.y + CUBE_SIZE)
                head->cube->drawFace(head->position, CUBE_FACE_TOP);
        }
        if(head->visibleFaces & (1 << 1)) // front
        {
            if(camPosition.z > head->position.z + CUBE_SIZE)
                head->cube->drawFace(head->position, CUBE_FACE_FRONT);
        }
        if(head->visibleFaces & (1 << 3)) // back
        {
            if(camPosition.z < head->position.z)
                head->cube->drawFace(head->position, CUBE_FACE_BACK);
        }
        if(head->visibleFaces & (1 << 2)) // right
        {
            if(camPosition.x > head->position.x + CUBE_SIZE)
                head->cube->drawFace(head->position, CUBE_FACE_RIGHT);
        }
        if(head->visibleFaces & (1 << 4)) // left
        {
            if(camPosition.x < head->position.x)
                head->cube->drawFace(head->position, CUBE_FACE_LEFT);
        }
        head = head->next;
    }
}

void Chunk::init()
{
    m_cubes = (Cube**)new Cube*[CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z];

    for(int y = 0; y < CHUNK_SIZE_Y; ++y)
    {
        for(int z = 0; z < CHUNK_SIZE_Z; ++z)
        {
            for(int x = 0; x < CHUNK_SIZE_X; ++x)
            {
                m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] = m_cubeInstances[CUBE_TYPE_OFFSET_AIR];
            }
        }
    }

    int32 step = floattof32(0.05f);
    Vec2 offset = {mulf32(divf32(m_position.x + inttof32(WORLD_SIZE_X / 2 * CHUNK_SIZE_X), inttof32(CHUNK_SIZE_X)), mulf32(inttof32(CHUNK_SIZE_X), step)),
        mulf32(divf32(m_position.y + inttof32(WORLD_SIZE_Z / 2 * CHUNK_SIZE_Z), inttof32(CHUNK_SIZE_Z)), mulf32(inttof32(CHUNK_SIZE_Z), step)),};

    for(int z = 0; z < CHUNK_SIZE_Z; ++z)
    {
        for(int x = 0; x < CHUNK_SIZE_X; ++x)
        {
            int32 y = NoiseGenerator::noise2D(offset);
            m_cubes[((mulf32(y, inttof32(10)) >> 12) + CHUNK_SIZE_Y - 10) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] = m_cubeInstances[CUBE_TYPE_OFFSET_GRASS];
            offset.x += step;
        }
        offset.x = mulf32(divf32(m_position.x + inttof32(WORLD_SIZE_X / 2 * CHUNK_SIZE_X), inttof32(CHUNK_SIZE_X)), mulf32(inttof32(CHUNK_SIZE_X), step));
        offset.y += step;
    }
    
    for(int y = CHUNK_SIZE_Y - 2; y > -1 ; --y)
    {
        for(int z = CHUNK_SIZE_Z - 1; z > -1; --z)
        {
            for(int x = CHUNK_SIZE_X; x > -1; --x)
            {
                if(m_cubes[(y + 1) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] == m_cubeInstances[CUBE_TYPE_OFFSET_GRASS]
                        || m_cubes[(y + 2) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] == m_cubeInstances[CUBE_TYPE_OFFSET_GRASS])
                    m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] = m_cubeInstances[CUBE_TYPE_OFFSET_DIRT];
                if(m_cubes[(y + 1) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] == m_cubeInstances[CUBE_TYPE_OFFSET_DIRT]
                        || m_cubes[(y + 1) * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] == m_cubeInstances[CUBE_TYPE_OFFSET_STONE])
                    m_cubes[y * CHUNK_SIZE_Z * CHUNK_SIZE_X + z * CHUNK_SIZE_X + x] = m_cubeInstances[CUBE_TYPE_OFFSET_STONE];
            }
        }
    }

    m_visibleCubes = new CubeNode();

    for(int i = 0; i < 4; ++i)
    {
        m_sideChunks[i] = nullptr;
    }

}

void Chunk::deleteDrawList()
{
    CubeNode* head = m_visibleCubes;
    while(head != nullptr)
    {
        CubeNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}