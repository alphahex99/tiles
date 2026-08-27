#ifndef _BLOCKDEFMANAGER_HPP
#define _BLOCKDEFMANAGER_HPP

#include <cassert>
#include <filesystem>
#include "flat_hash_map.hpp"
#include <vector>

#include "BlockDef.hpp"
#include "Jsonc.hpp"
#include "Random.hpp"
#include "TextureAtlas.hpp"

class BlockDefManager
{
  public:
    void DrawTextureAtlas() const
    {
        mTextureAtlas.Draw();
    }

    block_idx_t GetBlockIndex(block_id_t id) const
    {
        auto it = mBlockDefsById.find(id);
        assert(it != mBlockDefsById.end());
        return it->second.index;
    }

    const BlockDef &GetBlockDef(block_idx_t index) const
    {
        return *mBlockDefsByIdx[index];
    }

    /// @todo !!! REMOVE !!! I wrote this commit while drunk
    const BlockDef &GetBlockDef(block_id_t id) const
    {
        return *mBlockDefsByIdx[GetBlockIndex(id)];
    }

    block_idx_t GetRandomBlock(Random &random) const
    {
        assert(!mBlockDefsById.empty());
        auto it = mBlockDefsById.begin();

        while (true)
        { // there is a NON-ZERO chance this will hang until the heat death of the universe :P
            unsigned int i = random.uint_rand(0, mBlockDefsById.size() - 1);

            it = std::next(mBlockDefsById.begin(), i);
            if (!it->second.debug)
            {
                break;
            }
        };

        return GetBlockIndex(it->first);
    }

    void Load()
    {
        fs::path path = "assets/blocks/";
        assert(fs::exists(path));

        block_idx_t nextIndex = 0;
        json::ondemand::parser parser;

        for (const fs::directory_entry &dir_entry : fs::recursive_directory_iterator(path))
        {
            if (dir_entry.is_regular_file() && (dir_entry.path().filename() == "BlockDef.json"))
            {
                BlockDef::Load(mBlockDefsById, nextIndex, parser, dir_entry.path());
            }
        }

        mBlockDefsByIdx.resize(nextIndex);
        for (auto &[id, blockDef] : mBlockDefsById)
        {
            // assert(blockDef.index < nextIndex);
            mBlockDefsByIdx[blockDef.index] = &blockDef;
        }

        mTextureAtlas.Generate(mBlockDefsById);
    }

  private:
    ska::flat_hash_map<block_id_t, BlockDef> mBlockDefsById;
    std::vector<BlockDef *> mBlockDefsByIdx;

    TextureAtlas mTextureAtlas{};
};

#endif /* _BLOCKDEFMANAGER_HPP */
