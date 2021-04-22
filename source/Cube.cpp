#include "Cube.h"

Cube::Cube(Vec2 texCoords[], bool faceOpacities[])
{
    for(int i = 0; i < 3; ++i)
    {
        m_texCoords[i] = texCoords[i];
    }

    for(int i = 0; i < 7; ++i)
    {
        m_faceOpaque[i] = faceOpacities[i];
    }
}

void Cube::drawFace(const Vec3& position, CUBE_FACES face) const
{
    switch(face)
    {
        case CUBE_FACE_BOTTOM:
            Renderer::drawQuad({position.x, position.y, position.z}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, m_texCoords[CUBE_TEXCOORD_BOTTOM], {TEXTURE_SIZE, TEXTURE_SIZE}, {0, -1, 0});
            break;
        case CUBE_FACE_FRONT:
            Renderer::drawQuad({position.x, position.y, position.z + CUBE_SIZE}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, m_texCoords[CUBE_TEXCOORD_SIDE], {TEXTURE_SIZE, TEXTURE_SIZE}, {0, 0, 1});
            break;
        case CUBE_FACE_RIGHT:
            Renderer::drawQuad({position.x + CUBE_SIZE, position.y, position.z}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, m_texCoords[CUBE_TEXCOORD_SIDE], {TEXTURE_SIZE, TEXTURE_SIZE}, {1, 0, 0});
            break;
        case CUBE_FACE_BACK:
            Renderer::drawQuad({position.x, position.y, position.z}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, m_texCoords[CUBE_TEXCOORD_SIDE], {TEXTURE_SIZE, TEXTURE_SIZE}, {0, 0, -1});
            break;
        case CUBE_FACE_LEFT:
            Renderer::drawQuad({position.x, position.y, position.z}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, m_texCoords[CUBE_TEXCOORD_SIDE], {TEXTURE_SIZE, TEXTURE_SIZE}, {-1, 0, 0});
            break;
        case CUBE_FACE_TOP:
            Renderer::drawQuad({position.x, position.y + CUBE_SIZE, position.z}, {CUBE_SIZE, CUBE_SIZE, CUBE_SIZE}, m_texCoords[CUBE_TEXCOORD_TOP], {TEXTURE_SIZE, TEXTURE_SIZE}, {0, 1, 0});
            break;
        case CUBE_FACE_NONE:
            break;
        default:
            LOG("Invalid Cube face!", 0);
            break;
    }
}

void Cube::draw(const Vec3& position) const
{
    // bottom face
    drawFace(position, CUBE_FACE_BOTTOM);
    // front
    drawFace(position, CUBE_FACE_FRONT);
    // right
    drawFace(position, CUBE_FACE_RIGHT);
    // back
    drawFace(position, CUBE_FACE_BACK);
    // left
    drawFace(position, CUBE_FACE_LEFT);
    // top face
    drawFace(position, CUBE_FACE_TOP);
}

CubeNode::CubeNode() : position({0, inttof32(-1), 0})
{
    init();
}

CubeNode::CubeNode(const Vec3& Position) : position(Position)
{
    init();
}

void CubeNode::init()
{
    for(int i = 0; i < 6; ++i)
    {
        visibleFaces[i] = CUBE_FACE_NONE;
    }
}
