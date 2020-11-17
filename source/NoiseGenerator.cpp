#include "NoiseGenerator.h"

#include <random>

NoiseGenerator* NoiseGenerator::s_NoiseGenerator = new NoiseGenerator;
float NoiseGenerator::seed[SEED_SIZE] = {0.3, 0.9, 0.5, 0.3, 0.9, 0.1, 0.1, 0.1, 0.2, 0.4};

float NoiseGenerator::fade(float t)
{
    return 6 * powf(t, 5) - 15 * powf(t, 4) + 10 * powf(t, 3);
}

float NoiseGenerator::fadeXY(const vec2f& offset)
{
    return NoiseGenerator::fade(offset.x) * NoiseGenerator::fade(offset.y);
}

float NoiseGenerator::fadeXY(float x, float y)
{
    return NoiseGenerator::fade(x) * NoiseGenerator::fade(y);
}

vec2f NoiseGenerator::grad(int x, int y)
{
    return {(x * 12) % SEED_SIZE, (y * 14) % SEED_SIZE};
}

float NoiseGenerator::noise2D(const vec2f& offset)
{
    int x0 = floor(offset.x);
    int x1 = (x0 + 1) % SEED_SIZE;
    int y0 = floor(offset.y);
    int y1 = (y0 + 1) % SEED_SIZE;

    float xx = offset.x - x0;
    float yy = offset.y - y0;

    vec2f v00 = {offset.x - x0, offset.y - y0};
    vec2f v10 = {offset.x - x1, offset.y - y0};
    vec2f v01 = {offset.x - x0, offset.y - y1};
    vec2f v11 = {offset.x - x1, offset.y - y1};
    
    vec2f g00 = grad(x0, y0);
    vec2f g10 = grad(x1, y0);
    vec2f g01 = grad(x0, y1);
    vec2f g11 = grad(x1, y1);

    float sig00 = dot_vec2f(v00, g00);
    float sig10 = dot_vec2f(v10, g10);
    float sig01 = dot_vec2f(v01, g01);
    float sig11 = dot_vec2f(v11, g11);

    return fadeXY(1 - xx, 1 - yy) * sig00 + fadeXY(xx, 1 - yy) * sig10 + fadeXY(1 - xx, yy) * sig01 + fadeXY(xx, yy) * sig11;  
}