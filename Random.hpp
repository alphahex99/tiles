#ifndef _RANDOM_HPP
#define _RANDOM_HPP

#include <ctime>
#include <math.h>
#include <stdlib.h>

#include "raylib.h"

class Random
{
  public:
    Random()
    {
        srand(std::time({}));
    }

    Random(int seed)
    {
        srand(seed);
    }

    bool bool_rand()
    {
        return (bool) (rand() % 2);
    }

    float float_rand()
    {
        return (float) (rand()) / (float) (RAND_MAX);
    }

    float float_rand(int a, int b)
    {
        if (a > b)
        {
            return float_rand(b, a);
        }
        if (a == b)
        {
            return a;
        }
        return (float) int_rand(a, b) + float_rand();
    }

    int int_rand()
    {
        return rand();
    }

    int int_rand(int a, int b)
    {
        if (a > b)
        {
            return int_rand(b, a);
        }
        if (a == b)
        {
            return a;
        }
        return a + (rand() % (b - a));
    }

    Vector2 vec2_rand()
    {
        float azimuth = float_rand(0, 2 * PI);

        float x = cosf(azimuth);
        float y = sinf(azimuth);

        return Vector2{x, y};
    }

    Vector3 vec3_rand()
    {
        float azimuth = float_rand(0.0f, 2 * PI);

        float z = float_rand(-1.0f, 1.0f);
        float radius = sqrtf(1.0f - z * z);

        float x = radius * cosf(azimuth);
        float y = radius * sinf(azimuth);

        return Vector3{x, y, z};
    }
};

#endif /* _SRC_RANDOM_HPP */
