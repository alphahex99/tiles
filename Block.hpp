#ifndef _BLOCK_HPP
#define _BLOCK_HPP

#include "BlockDef.hpp"

struct Block
{
    unsigned int height;

    block_id_t id; // TODO: block_idx_t ?
};

#endif /* _BLOCK_HPP */
