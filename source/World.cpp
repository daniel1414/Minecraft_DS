#include "World.h"

#include "stdlib.h"
#include "nds/arm9/videoGL.h"

#include "NoiseGenerator.h"
#include "Log.h"

World::World()
{
    m_cubes = (Cube*)malloc(m_width * m_width * sizeof(Cube));
}

World::~World()
{
    free(m_cubes);
}

void World::generate_terrain(int* textures)
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
            /* for(int z = 0; z < m_width; z++)
            {
                m_cubes[x * m_width + z].move({inttof32(x - m_width / 2), 0, inttof32(-z + m_width / 2)});
                m_cubes[x * m_width + z].load_textures(textures);
            } */

        }
        offset.x = 0.0;
        offset.y += 0.05;
	}
}

void World::render_cubes(const vec3f32& player_position, const vec3f32& player_direction) const
{
    for(unsigned int i = 0; i < m_width * m_width; i++)
    {
        m_cubes[i].draw_face(CUBE_TOP);
        m_cubes[i].draw_face(CUBE_BOTTOM);
        m_cubes[i].draw_face(CUBE_FRONT);
        m_cubes[i].draw_face(CUBE_BACK);
        m_cubes[i].draw_face(CUBE_LEFT);
        m_cubes[i].draw_face(CUBE_RIGHT);
    }
}