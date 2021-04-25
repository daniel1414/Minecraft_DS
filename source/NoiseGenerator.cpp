#include "NoiseGenerator.h"

#include "Log.h"

int32 NoiseGenerator::seed[SEED_SIZE] = {floattof32(0.9f), floattof32(-0.4f), floattof32(0.6f), floattof32(-0.5f), floattof32(0.3f), 
    /* floattof32(-0.4f), floattof32(0.8f), floattof32(-0.2f), floattof32(0.2f), floattof32(0.7f) */};

int32 NoiseGenerator::fade(int32 t)
{
    //return mulf32(inttof32(6), mulf32(t, mulf32(t, mulf32(t, mulf32(t, t))))) - mulf32(15, mulf32(t, mulf32(t, mulf32(t, t)))) + mulf32(inttof32(10), mulf32(t, mulf32(t, t)));
    return mulf32(inttof32(3), mulf32(t, t)) - mulf32(inttof32(2), mulf32(t, mulf32(t,t)));
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

/**
 * for seed values from range (-1, 1) this function returns noise from range (-8, 8)
 */
int32 NoiseGenerator::noise2D(const Vec2& offset)
{
    Vec2 position = offset;

    position.x = (position.x & 0xFFF) | (((offset.x >> 12) % SEED_SIZE) << 12);
    position.y = (position.y & 0xFFF) | (((offset.y >> 12) % SEED_SIZE) << 12);

    int32 x0 = ((position.x >> 12) << 12);
    int32 x1 = x0 + inttof32(1);
    int32 y0 = ((position.y >> 12) << 12);
    int32 y1 = y0 + inttof32(1);
    
    int32 xx = position.x - x0;
    int32 yy = position.y - y0;

    Vec3 v00 = {position.x - x0, position.y - y0, 0};
    Vec3 v10 = {position.x - x1, position.y - y0, 0};
    Vec3 v01 = {position.x - x0, position.y - y1, 0};
    Vec3 v11 = {position.x - x1, position.y - y1, 0};
    
    Vec3 g00 = {grad(x0 >> 12, y0 >> 12).x, grad(x0 >> 12, y0 >> 12).y, 0};
    Vec3 g10 = {grad(x1 >> 12, y0 >> 12).x, grad(x1 >> 12, y0 >> 12).y, 0};
    Vec3 g01 = {grad(x0 >> 12, y1 >> 12).x, grad(x0 >> 12, y1 >> 12).y, 0};
    Vec3 g11 = {grad(x1 >> 12, y1 >> 12).x, grad(x1 >> 12, y1 >> 12).y, 0};

    int32 sig00 = dotf32(&v00.x, &g00.x);
    int32 sig10 = dotf32(&v10.x, &g10.x);
    int32 sig01 = dotf32(&v01.x, &g01.x);
    int32 sig11 = dotf32(&v11.x, &g11.x);

    return mulf32(fadeXY(inttof32(1) - xx, inttof32(1) - yy), sig00) + mulf32(fadeXY(xx, inttof32(1) - yy), sig10) + 
            mulf32(fadeXY(inttof32(1) - xx, yy), sig01) + mulf32(fadeXY(xx, yy), sig11);
}