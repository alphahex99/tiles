#ifndef _BLOCKDEF_HPP
#define _BLOCKDEF_HPP

#include <cstdint>
#include "flat_hash_map.hpp"
#include "raylib.h"
#include <string>

#include "Jsonc.hpp"

using block_id_t = std::uint64_t;  // 0 - 18446744073709551615
using block_idx_t = std::uint16_t; // 0 - 65535

static constexpr block_id_t BLOCK_ID_DEBUG_NULL = 0ULL;
static constexpr block_id_t BLOCK_ID_DEBUG_SELECTION_ADD = 13935501760859369348ULL;
static constexpr block_id_t BLOCK_ID_DEBUG_SELECTION_REMOVE = 7963692563000193987ULL;

static constexpr int BLOCK_PX_SIZE = 64;

struct BlockDef
{
    std::string name;
    std::string texture;

    bool debug;

    const Image *atlasImage;
    const Texture2D *atlasTexture;
    Rectangle atlasSource;

    static void Load(ska::flat_hash_map<block_id_t, BlockDef> &out, json::ondemand::parser &parser,
                     const fs::path &path);
};

#endif /* _BLOCKDEF_HPP */
