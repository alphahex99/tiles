#ifndef _ISOMETRIC_HPP
#define _ISOMETRIC_HPP

#include "raylib.h"

namespace iso
{

// Advances X on isometric grid:
// \
//  x
template <int BLOCK_PX_SIZE>
constexpr Vector2 StepX()
{
    constexpr float x = 0.5f * BLOCK_PX_SIZE;
    constexpr float y = 0.25f * BLOCK_PX_SIZE;

    return {x, y};
}

// Advances Y on isometric grid:
//  /
// y
template <int BLOCK_PX_SIZE>
constexpr Vector2 StepY()
{
    constexpr float x = -0.5f * BLOCK_PX_SIZE;
    constexpr float y = 0.25f * BLOCK_PX_SIZE;

    return {x, y};
}

// Rotates isometric grid
//   /\
//  /  \
// y    x
//
// to square grid.
//  ------ x
// |
// |
// y
//
template <int BLOCK_PX_SIZE>
constexpr Vector2 IsoToSquare(Vector2 position)
{
    constexpr float BLOCK_PX_SIZE_INV = 1.0f / BLOCK_PX_SIZE;

    float x = (position.x + 2.0f * position.y) * BLOCK_PX_SIZE_INV;
    float y = (2.0f * position.y - position.x) * BLOCK_PX_SIZE_INV;

    return {x, y};
}

// Rotates square grid
//  ------ x
// |
// |
// y
//
// to isometric grid.
//   /\
//  /  \
// y    x
//
template <int BLOCK_PX_SIZE>
constexpr Vector2 SquareToIso(Vector2 position)
{
    constexpr Vector2 stepX = StepX<BLOCK_PX_SIZE>();
    constexpr Vector2 stepY = StepY<BLOCK_PX_SIZE>();

    // We can't just:
    // 'return x * position.x + y * position.y;'
    // because raylib.h doesn't support constexpr.
    float x = stepX.x * position.x + stepY.x * position.y;
    float y = stepX.y * position.x + stepY.y * position.y;

    return {x, y};
}

} // namespace iso

#endif /* _ISOMETRIC_HPP */
