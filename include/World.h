#ifndef _WORLD_H_
#define _WORLD_H_

#include "nds.h"
#include "Player.h"
#include "Cube.h"

class World
{
public:
    World();
    ~World();

    void generate_terrain(int* textures);
    void render_cubes() const;

private:
    Player m_player;
    Cube* m_cubes;
    u16 m_width = 50;
};

#endif /* _WORLD_H_ */