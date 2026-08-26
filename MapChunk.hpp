#ifndef _CHUNK_HPP
#define _CHUNK_HPP

#include <cstddef>
#include <cstdint>

#include "flat_hash_map.hpp"
#include "raylib.h"

#include "BlockDef.hpp"

class Chunk // TODO
{
    using packed_xyz_t = std::uint16_t;

  public:
    static constexpr std::size_t X = 64;
    static constexpr std::size_t Y = 16;
    static constexpr std::size_t Z = 64;
    static_assert(X * Y * Z == 1 << (sizeof(packed_xyz_t) * 8));

    Vector2 GetWorld();

  private:
    ska::flat_hash_map<block_idx_t, std::vector<packed_xyz_t>> blocks;
}

#endif /* _CHUNK_HPP */
