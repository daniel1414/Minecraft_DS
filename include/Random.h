#pragma once

#include <random>

class Random
{
public:
    static int getInt(int min, int max);
private:
    static std::random_device rd;
    static std::mt19937 mt;
};