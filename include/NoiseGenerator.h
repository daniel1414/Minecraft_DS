#ifndef _NOISE_GENERATOR_H_
#define _NOISE_GENERATOR_H_

#define NOISE_N 6
#define SEED_SIZE 10

#include <nds.h>

#include <nds/arm9/math.h>

class NoiseGenerator
{
public:
    static int32 fade(int32 t);
    static int32 fadeXY(const Vec2& offset);
    static int32 fadeXY(int32 x, int32 y);
    static Vec2 grad(int x, int y);
    static int32 noise2D(const Vec2& offset);
private:
    static int32 seed[SEED_SIZE];
};

#endif /* _NOISE_GENERATOR_H_ */