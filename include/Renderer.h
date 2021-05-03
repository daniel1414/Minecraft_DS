#pragma once

#include <nds.h>
#include <nds/arm9/math.h>

class Renderer
{
public:
    static void init();
    static void beginScene();
    static void endScene();
    
    static void drawQuad(const Vec3& position, const Vec3& size, const Vec2& texCoords, const Vec2& texSize, const Vec3& normal,
        uint16_t color = RGB15(31, 31, 31), int angleDeg = 0, const Vec3& rotationAxis = {0, 0, 0});
    
    static void drawTriangle(const Vec3& position, const Vec3& size, const Vec2& texCoords, const Vec2& texSize, const Vec3& normal,
        uint16_t color = RGB15(31, 31, 31), float angle = 0.0f, const Vec3& rotationAxis = {0, 0, 0});
    static void drawArrow(const Vec3& position, const Vec3& direction, uint16 color = RGB15(31, 0, 0));
private:
    static void setTranslation(const Vec3& translation);
    static void setScale(const Vec3& scale);
    static void sendToFifo();
};