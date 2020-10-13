#ifndef _CUBE_H_
#define _CUBE_H_

#include <nds.h>

#include "My_Types.h"

class Cube
{
public:
    Cube() = default;
    Cube(vec3f32 position, int textures[3]);
    void move(vec3f32 destination);
    void load_textures(int textures[3]);
    void draw() const;
private:
    vec3f32 m_position;
    int m_textures[3];
};

#endif /* _CUBE_H_ */