#ifndef _NOISE_GENERATOR_H_
#define _NOISE_GENERATOR_H_

#define SEED_SIZE 10

#include <nds.h>

#include "My_Types.h"

class NoiseGenerator
{
public:
    static float fade(float t);
    static float fadeXY(const vec2f& offset);
    static float fadeXY(float x, float y);
    static vec2f grad(int x, int y);
    static float noise2D(const vec2f& offset);
private:
    static NoiseGenerator* s_NoiseGenerator;
    static float seed[SEED_SIZE];
};

#endif /* _NOISE_GENERATOR_H_ */