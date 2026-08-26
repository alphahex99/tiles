#ifndef _MAPCHUNK_HPP
#define _MAPCHUNK_HPP

#include "raylib.h"
#include <vector>

#include "BlockDef.hpp"
#include "BlockDefManager.hpp"

class MapChunk
{
  public:
    static constexpr int CHUNK_SIZE = 16;

    void Draw() const
    {
    }

    void Update(const BlockDefManager &blockDefManager)
    {
        if (mImage.data != nullptr)
        {
            UnloadImage(mImage);
        }
        if (mTexture.id != 0)
        {
            UnloadTexture(mTexture);
        }

        constexpr int imageWidth = CHUNK_SIZE * BLOCK_PX_SIZE;
        constexpr int imageHeight = CHUNK_SIZE * BLOCK_PX_SIZE;

        mImage = GenImageColor(imageWidth, imageHeight, BLANK);

        int i = 0;
        for (block_id_t id : mBlocks)
        {
            if (id != BLOCK_ID_DEBUG_EMPTY)
            {
                const BlockDef &blockDef = blockDefManager.GetBlockDef(id);

                float x = static_cast<float>(i % CHUNK_SIZE * BLOCK_PX_SIZE);
                float y = static_cast<float>(i / CHUNK_SIZE * BLOCK_PX_SIZE);
                Rectangle dstRect = {x, y, BLOCK_PX_SIZE, BLOCK_PX_SIZE};

                ImageDraw(&mImage, *blockDef.atlasImage, blockDef.atlasSource, dstRect, WHITE);
            }

            i++;
        }

        mTexture = LoadTextureFromImage(mImage);
        SetTextureFilter(mTexture, TEXTURE_FILTER_POINT);
    }

    const Texture2D &GetTexture() const
    {
        return mTexture;
    }

  private:
    std::vector<block_id_t> mBlocks;
    Image mImage = {0};
    Texture2D mTexture = {0};
};

#endif /* _MAPCHUNK_HPP */
