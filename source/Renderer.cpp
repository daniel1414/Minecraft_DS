#include "Renderer.h"
#include "Log.h"

#include <nds/arm9/videoGL.h>

#define RENDERER_STORAGE_SIZE 30

#define MTX_MODE    0x10 // one parameter
#define MTX_PUSH    0x11 // no parameters
#define MTX_POP     0x12 // one parameter
#define MTX_TRANS   0x1C // 3 parameters
#define MTX_SCALE   0x1B // 3 parameters

struct RendererStorage
{
    v16 vertexPositions[3 * 12] = {
        // XY from front
        inttov16(0), inttov16(0), inttov16(0),
        inttov16(1), inttov16(0), inttov16(0),
        inttov16(1), inttov16(1), inttov16(0),
        inttov16(0), inttov16(1), inttov16(0),
        // XZ from top
        inttov16(0), inttov16(0), inttov16(1),
        inttov16(1), inttov16(0), inttov16(1),
        inttov16(1), inttov16(0), inttov16(0),
        inttov16(0), inttov16(0), inttov16(0),
        // YZ from right
        inttov16(0), inttov16(0), inttov16(1),
        inttov16(0), inttov16(0), inttov16(0),
        inttov16(0), inttov16(1), inttov16(0),
        inttov16(0), inttov16(1), inttov16(1)
    };
    uint8_t vertexIndices[4 * 6] = {
        0, 3, 6, 9, // XY front
        0, 9, 6, 3, // XY back
        12, 15, 18, 21, // XZ top
        12, 21, 18, 15, // XZ bottom
        24, 27, 30, 33, // YZ right
        24, 33, 30, 27  // YZ left
    };
    int texturePositions[2 * 4] = { 
        0, 0,
        0, 0,
        0, 0,
        0, 0
    };
    int textureIndices[3 * 4] = {
        0, 2, 4, 6,
        0, 6, 4, 2,
        6, 0, 2, 4
    };
    
    uint32_t* packedCommands = nullptr;
};

static RendererStorage storage;

void Renderer::init()
{
    if(!storage.packedCommands)
    {
        storage.packedCommands = new uint32_t[RENDERER_STORAGE_SIZE];
    }
}

void Renderer::beginScene()
{ 
    glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | (1 << 11) | POLY_ID(0));
}
void Renderer::endScene()
{
    glFlush(0);
}

void Renderer::drawQuad(const Vec3& position, const Vec3& size, const Vec2& texCoords, const Vec2& texSize, const Vec3& normal,
    uint16_t color, float angle, const Vec3& rotationAxis)
{
    storage.packedCommands[0] = 26;

    setTranslation(position);
    setScale(size);
    
    storage.packedCommands[9] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX16);
	storage.packedCommands[10] = GL_QUADS; 
	storage.packedCommands[11] = NORMAL_PACK(inttov10(normal.x), inttov10(normal.y), inttov10(normal.z));

    uint8_t textureIndexOffset = 0;

    storage.texturePositions[0] = texCoords.x + 1;  // bottom left
    storage.texturePositions[1] = texCoords.y + texSize.y - 1;
    storage.texturePositions[2] = texCoords.x + texSize.x - 1; // bottom right 
    storage.texturePositions[3] = texCoords.y + texSize.y - 1;
    storage.texturePositions[4] = texCoords.x + texSize.x - 1;  // top right
    storage.texturePositions[5] = texCoords.y + 1;
    storage.texturePositions[6] = texCoords.x + 1; // top left
    storage.texturePositions[7] = texCoords.y + 1;
    
    uint8_t vertexIndicesOffset = 0;
    if(normal.z == 1)
    {
        vertexIndicesOffset = 0;
        textureIndexOffset = 0;
    }
    else if(normal.z == -1)
    {
        vertexIndicesOffset = 4;
        textureIndexOffset = 4;
    }
    else if(normal.y == 1)
    {
        vertexIndicesOffset = 8;
    }
    else if(normal.y == -1)
    {
        vertexIndicesOffset = 12;
        textureIndexOffset = 8;
    }
    else if(normal.x == 1)
    {
        vertexIndicesOffset = 16;
    }
    else if(normal.x == -1)
    {
        vertexIndicesOffset = 20;
        textureIndexOffset = 4;
    }

	storage.packedCommands[12] = TEXTURE_PACK(inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset]]), inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset] + 1]));
	storage.packedCommands[13] = VERTEX_PACK(storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset]], storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset] + 1]);
    storage.packedCommands[14] = storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset] + 2];

	storage.packedCommands[15] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_TEX_COORD, FIFO_VERTEX16);
	storage.packedCommands[16] = TEXTURE_PACK(inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset + 1]]), inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset + 1] + 1]));
	storage.packedCommands[17] = VERTEX_PACK(storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 1]], storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 1] + 1]);
	storage.packedCommands[18] = storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 1] + 2];
	storage.packedCommands[19] = TEXTURE_PACK(inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset + 2]]), inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset + 2] + 1]));
	storage.packedCommands[20] = VERTEX_PACK(storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 2]], storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 2] + 1]);
    storage.packedCommands[21] = storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 2] + 2];

	storage.packedCommands[22] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_VERTEX16, MTX_POP, 0);
	storage.packedCommands[23] = TEXTURE_PACK(inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset + 3]]), inttot16(storage.texturePositions[storage.textureIndices[textureIndexOffset + 3] + 1]));
	storage.packedCommands[24] = VERTEX_PACK(storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 3]], storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 3] + 1]);
	storage.packedCommands[25] = storage.vertexPositions[storage.vertexIndices[vertexIndicesOffset + 3] + 2];
    storage.packedCommands[26] = 1; 

    sendToFifo();
}

void Renderer::drawTriangle(const Vec3& position, const Vec3& size, const Vec2& texCoords, const Vec2& texSize, const Vec3& normal,
    uint16_t color, float angle, const Vec3& rotationAxis)
{
    storage.packedCommands[0] = 23;

    setTranslation(position);
    setScale(size);

    storage.packedCommands[9] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NORMAL, FIFO_TEX_COORD, FIFO_VERTEX16);
	storage.packedCommands[10] = GL_TRIANGLES;
	storage.packedCommands[11] = NORMAL_PACK(normal.x, normal.y, normal.z); 
	storage.packedCommands[12] = TEXTURE_PACK(inttot16(0), inttot16(16));
	storage.packedCommands[13] = VERTEX_PACK(floattov16(-0.5f), floattov16(-0.5f));
    storage.packedCommands[14] = 0;

	storage.packedCommands[15] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_TEX_COORD, FIFO_VERTEX16);
	storage.packedCommands[16] = TEXTURE_PACK(inttot16(16), inttot16(16));
	storage.packedCommands[17] = VERTEX_PACK(floattov16(0.5f), floattov16(-0.5f));
	storage.packedCommands[18] = 0;
	storage.packedCommands[19] = TEXTURE_PACK(inttot16(16), 0);
	storage.packedCommands[20] = VERTEX_PACK(floattov16(0.0f), floattov16(0.5f));
    storage.packedCommands[21] = 0;

	storage.packedCommands[22] = MTX_POP;
    storage.packedCommands[23] = 1;

    sendToFifo();
}

void Renderer::drawArrow(const Vec3& position, const Vec3& direction, uint16 color)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    int32 scaleFactor = mulf32(sqrtf32( mulf32(direction.x, direction.x) + mulf32(direction.y, direction.y) + mulf32(direction.z, direction.z)), floattof32(0.2f));
    int32 signX = direction.x > 0 ? inttof32(1) : inttof32(-1);
    int32 signY = direction.y > 0 ? inttof32(1) : inttof32(-1);
    int32 signZ = direction.z > 0 ? inttof32(1) : inttof32(-1);
    glBegin(GL_TRIANGLES);
    glColor(color);
    glTranslatef32(position.x, position.y, position.z);
    glVertex3v16(inttov16(0), inttov16(0), inttov16(0));
    glVertex3v16(f32tov16(direction.x), f32tov16(direction.y), f32tov16(direction.z));
    glVertex3v16(f32tov16(direction.x), f32tov16(direction.y), f32tov16(direction.z));
    glVertex3v16(f32tov16(direction.x), f32tov16(direction.y), f32tov16(direction.z));
    glVertex3v16(f32tov16(direction.x - mulf32(scaleFactor, signX)), f32tov16(direction.y - mulf32(scaleFactor, signY)), f32tov16(direction.z - mulf32(scaleFactor, signZ)));
    glVertex3v16(f32tov16(direction.x - mulf32(scaleFactor, signX)), f32tov16(direction.y - mulf32(scaleFactor, signY)), f32tov16(direction.z - mulf32(scaleFactor, signZ)));
    glEnd();
    glPopMatrix(1);
}

void Renderer::setTranslation(const Vec3& translation)
{
    storage.packedCommands[1] = FIFO_COMMAND_PACK(MTX_MODE, MTX_PUSH, MTX_TRANS, MTX_SCALE);
    storage.packedCommands[2] = GL_MODELVIEW;
    storage.packedCommands[3] = translation.x;
    storage.packedCommands[4] = translation.y;
    storage.packedCommands[5] = translation.z;
    
}

void Renderer::setScale(const Vec3& scale)
{
    storage.packedCommands[6] = scale.x;
    storage.packedCommands[7] = scale.y;
    storage.packedCommands[8] = scale.z;
}

void Renderer::sendToFifo()
{
    glCallList(storage.packedCommands);
}