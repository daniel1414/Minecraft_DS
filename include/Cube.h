#ifndef _CUBE_H_
#define _CUBE_H_

#include <nds.h>

#include "My_Types.h"

enum CUBE_FACES
{
    CUBE_TOP,
    CUBE_FRONT,
    CUBE_RIGHT,
    CUBE_BACK,
    CUBE_LEFT,
    CUBE_BOTTOM
};

class Cube
{
public:
    Cube() = default;
    Cube(vec3f32 position, int textures[3]);
    void move(vec3f32 destination);
    void shift(vec3f32 direction);
    void load_textures(int* textures);
    void draw_face(CUBE_FACES face) const;
    void draw() const;
private:
    vec3f32 m_position;
    int* m_textures; // 3 textures - top, side, bottom
};

#endif /* _CUBE_H_ */