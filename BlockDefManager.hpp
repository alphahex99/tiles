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

    const BlockDef &GetBlockDef(block_id_t id) const
    {
        auto it = mBlockDefs.find(id);
        assert(it != mBlockDefs.end());
        return it->second;
    }

    block_id_t GetRandomBlock(Random &random) const
    {
        assert(!mBlockDefs.empty());
        auto it = mBlockDefs.begin();

        while (true)
        { // there is a NON-ZERO chance this will hang until the heat death of the universe :P
            unsigned int i = random.uint_rand(0, mBlockDefs.size() - 1);

            it = std::next(mBlockDefs.begin(), i);
            if (!it->second.debug)
            {
                break;
            }
        };

        return it->first;
    }

    void Load()
    {
        fs::path path = "assets/blocks/";
        assert(fs::exists(path));

        json::ondemand::parser parser;

        for (const fs::directory_entry &dir_entry : fs::recursive_directory_iterator(path))
        {
            if (dir_entry.is_regular_file() && (dir_entry.path().filename() == "BlockDef.json"))
            {
                LoadBlockDef(mBlockDefs, parser, dir_entry.path());
            }
        }

        mTextureAtlas.Generate(mBlockDefs);
    }

  private:
    ska::flat_hash_map<block_id_t, BlockDef> mBlockDefs;
    TextureAtlas mTextureAtlas{};
};

#endif /* _BLOCKDEFMANAGER_HPP */
