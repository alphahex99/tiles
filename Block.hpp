#ifndef _BLOCK_HPP
#define _BLOCK_HPP

#include "BlockDef.hpp"

enum class BlockState
{
    EMPTY = 0,
    FLOOR,
    WALL
};

struct Block
{
    BlockState blockState;
    block_id_t id; // TODO: block_idx_t ?
};

#endif /* _BLOCK_HPP */