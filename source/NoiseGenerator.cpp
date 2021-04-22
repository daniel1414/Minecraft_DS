#include "NoiseGenerator.h"

#include <random>

int32 NoiseGenerator::seed[SEED_SIZE] = {floattof32(0.3f), floattof32(0.9f), floattof32(-0.5f), floattof32(-0.3f), floattof32(0.9f), 
    floattof32(-0.1f), floattof32(0.1f), floattof32(-0.1f), floattof32(0.2f), floattof32(0.4f)};

int32 NoiseGenerator::fade(int32 t)
{
    return mulf32(inttof32(6), mulf32(t, mulf32(t, mulf32(t, mulf32(t, t))))) - mulf32(15, mulf32(t, mulf32(t, mulf32(t, t)))) + mulf32(inttof32(10), mulf32(t, mulf32(t, t)));
}

int32 NoiseGenerator::fadeXY(const Vec2& offset)
{
    return mulf32(NoiseGenerator::fade(offset.x), NoiseGenerator::fade(offset.y));
}

int32 NoiseGenerator::fadeXY(int32 x, int32 y)
{
    return mulf32(NoiseGenerator::fade(x), NoiseGenerator::fade(y));
}

Vec2 NoiseGenerator::grad(int x, int y)
{
    return {NoiseGenerator::seed[(x * 12) % SEED_SIZE], NoiseGenerator::seed[(y * 14) % SEED_SIZE]};
}

int32 NoiseGenerator::noise2D(const Vec2& offset)
{
    int32 x0 = ((offset.x >> 12) << 12);
    int32 x1 = (((x0 >> 12) + 1) % SEED_SIZE << 12);
    int32 y0 = ((offset.y >> 12) << 12);
    int32 y1 = (((y0 >> 12) + 1) % SEED_SIZE << 12);

    int32 xx = offset.x - x0;
    int32 yy = offset.y - y0;

    Vec3 v00 = {offset.x - x0, offset.y - y0, 0};
    Vec3 v10 = {offset.x - x1, offset.y - y0, 0};
    Vec3 v01 = {offset.x - x0, offset.y - y1, 0};
    Vec3 v11 = {offset.x - x1, offset.y - y1, 0};
    
    Vec3 g00 = {grad(x0, y0).x, grad(x0, y0).y, 0};
    Vec3 g10 = {grad(x1, y0).x, grad(x1, y0).y, 0};
    Vec3 g01 = {grad(x0, y1).x, grad(x0, y1).y, 0};
    Vec3 g11 = {grad(x1, y1).x, grad(x1, y1).y, 0};


    int32 sig00 = dotf32(&v00.x, &g00.x);
    int32 sig10 = dotf32(&v10.x, &g10.x);
    int32 sig01 = dotf32(&v01.x, &g01.x);
    int32 sig11 = dotf32(&v11.x, &g11.x);

    return mulf32(fadeXY(1 - xx, 1 - yy), sig00) + mulf32(fadeXY(xx, 1 - yy), sig10) + 
            mulf32(fadeXY(1 - xx, yy), sig01) + mulf32(fadeXY(xx, yy), sig11);  
}