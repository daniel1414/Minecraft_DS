#pragma once

#include "Renderer.h"

#define CUBE_SIZE (int32)inttof32(1)
#define CUBE_SIZE_HALF (int32)floattof32(0.5f)
#define CUBE_RADIUS (int32)floattof32(0.8f)
#define TEXTURE_SIZE 16

#define CUBE_TYPE_COUNT 7

#define CUBE_TYPE_OFFSET_AIR 0
#define CUBE_TYPE_OFFSET_GRASS 1
#define CUBE_TYPE_OFFSET_DIRT 2
#define CUBE_TYPE_OFFSET_STONE 3
#define CUBE_TYPE_OFFSET_COBBLESTONE 4
#define CUBE_TYPE_OFFSET_OAK_WOOD 5 
#define CUBE_TYPE_OFFSET_OAK_LEAVES 6


enum CUBE_FACES
{
    CUBE_FACE_BOTTOM = 0,
    CUBE_FACE_FRONT,
    CUBE_FACE_RIGHT,
    CUBE_FACE_BACK,
    CUBE_FACE_LEFT,
    CUBE_FACE_TOP,
    CUBE_FACE_NONE
};

enum CUBE_TEXTURE_COORDS
{
    CUBE_TEXCOORD_BOTTOM,
    CUBE_TEXCOORD_SIDE,
    CUBE_TEXCOORD_TOP
};

class Cube
{
public:
    Cube(Vec2 texCoords[], bool faceOpacities[]);
    
    void drawFace(const Vec3& position, CUBE_FACES face) const;
    void draw(const Vec3& position) const;

    bool isOpaque(CUBE_FACES face) const { return m_faceOpaque[face]; }
private:
    Vec2 m_texCoords[3]; // according to CUBE_TEXTURE_COORDS
    bool m_faceOpaque[7]; // according to CUBE_FACES enum
    // add some other stuff according to every cube in the universe
};

struct CubeNode
{
    CubeNode();
    CubeNode(const Vec3& Position);

    CubeNode* next = nullptr;
    Cube* cube = nullptr;
    Vec3 position;
    uint8 visibleFaces = 0; // LSB - bottom, MSB - top
private:
    void init();
};