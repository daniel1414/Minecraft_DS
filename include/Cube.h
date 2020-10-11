#ifndef _CUBE_H_
#define _CUBE_H_

#include <nds.h>

#include "My_Types.h"

class Cube
{
public:
    Cube(vec3f32 position, int textures[3]);
    void draw(vec3f32* camera_position, vec3f32* camera_front, vec3f32* camera_world_up);
    void draw() const;
private:
    vec3f32 m_position;
    int m_textures[3];
};

#endif /* _CUBE_H_ */