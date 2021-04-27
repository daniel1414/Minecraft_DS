#include "Random.h"

std::random_device Random::rd;
std::mt19937 Random::mt(rd());

int Random::getInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}