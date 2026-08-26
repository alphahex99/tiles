#ifndef _MAPCHUNK_HPP
#define _MAPCHUNK_HPP

#include <vector>

#include "BlockDef.hpp"

class MapChunk // TODO
{
  public:
    static constexpr int CHUNK_SIZE = 16;

    void Draw() const
    {
    }

    void Update()
    {
    }

  private:
    std::vector<block_id_t> mBlocks;
};

#endif /* _MAPCHUNK_HPP */
