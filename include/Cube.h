#ifndef _CUBE_H_
#define _CUBE_H_

#include <nds.h>


typedef struct world_positionf32
{
    int x, y, z; // fixed point format 20.12
} world_positionf32;

class Cube
{
public:
    Cube(world_positionf32 position, int textures[3]);
    void draw() const;
private:
    world_positionf32 m_position;
    int m_textures[3];
};

#endif /* _CUBE_H_ */