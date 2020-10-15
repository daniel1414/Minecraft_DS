#include "World.h"

#include "stdlib.h"
#include "nds/arm9/videoGL.h"

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
    for(int x = 0; x < m_width; x++)
	{
		for(int z = 0; z < m_width; z++)
		{
			m_cubes[x * m_width + z].move({inttof32(x - m_width / 2), 0, inttof32(-z + m_width / 2)});
            m_cubes[x * m_width + z].load_textures(textures);
		}
	}
}

void World::render_cubes() const
{
    for(unsigned int i = 0; i < m_width * m_width; i++)
    {
        m_cubes[i].draw_face(CUBE_TOP);
    }
}