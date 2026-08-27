#ifndef _ISOMETRIC_HPP
#define _ISOMETRIC_HPP

#include "raylib.h"

namespace iso
{

constexpr Vector2 IsoToSquare(int BLOCK_PX_SIZE, Vector2 position)
{
    // Rotate isometric grid
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
    return {(1.0f / BLOCK_PX_SIZE) * (position.x + 2.0f * position.y),
            (1.0f / BLOCK_PX_SIZE) * (2.0f * position.y - position.x)};
}

constexpr Vector2 SquareToIso(int BLOCK_PX_SIZE, Vector2 position)
{
    // Rotate square grid
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
    return {(BLOCK_PX_SIZE / 2) * (position.x - position.y),
            (BLOCK_PX_SIZE / 2) * position.y + (BLOCK_PX_SIZE / 4) * (position.x - position.y)};
}

} // namespace iso

#endif /* _ISOMETRIC_HPP */
