#ifndef _RANDOM_HPP
#define _RANDOM_HPP

#include <cmath>
#include <random>
#include "raylib.h"
#include <type_traits>

class Random
{
  public:
    Random(unsigned int seed = std::random_device{}()) : mRandom(seed)
    {
    }

    bool bool_rand()
    {
        return (mRandom() & 1) != 0;
    }

    float float_rand(float a, float b)
    {
        return std::uniform_real_distribution<float>(a, b)(mRandom);
    }

    /// @returns INT_MIN to INT_MAX
    int int_rand()
    {
        return static_cast<int>(mRandom());
    }

    int int_rand(int a, int b)
    {
        return std::uniform_int_distribution<int>(a, b)(mRandom);
    }

    /// @returns 0 to UINT_MAX
    unsigned int uint_rand()
    {
        return mRandom();
    }

    unsigned int uint_rand(unsigned int a, unsigned int b)
    {
        return std::uniform_int_distribution<unsigned int>(a, b)(mRandom);
    }

    /// @returns 2D unit vector (length = 1.0f)
    Vector2 vec2_rand()
    {
        const float azimuth = float_rand(0.0f, 2.0f * PI);
#ifdef __GLIBC__
        float x, y;
        sincosf(azimuth, &y, &x); // tiny FSINCOS instruction optimization
#else
        const float x = cosf(azimuth);
        const float y = sinf(azimuth);
#endif
        return {x, y};
    }

    /// @returns 3D unit vector (length = 1.0f)
    Vector3 vec3_rand()
    {
        const float azimuth = float_rand(0.0f, 2.0f * PI);
        const float z = float_rand(-1.0f, 1.0f);
        const float r = sqrtf(1.0f - z * z);
#ifdef __GLIBC__
        float x, y;
        sincosf(azimuth, &y, &x); // tiny FSINCOS instruction optimization
#else
        const float x = cosf(azimuth);
        const float y = sinf(azimuth);
#endif
        return {r * x, r * y, z};
    }

  private:
    std::conditional_t<sizeof(int) == 4, std::mt19937, std::mt19937_64> mRandom; // 32B+64B support
};

#endif /* _RANDOM_HPP */
