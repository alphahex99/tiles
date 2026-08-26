#ifndef _ATLAS_HPP
#define _ATLAS_HPP

#include <cmath>
#include "flat_hash_map.hpp"
#include "raylib.h"
// #include "stb_rect_pack.h" // Will become useful when it's not all squares, TODO

#include "BlockDef.hpp"

class Atlas
{
  public:
    ~Atlas()
    {
        UnloadImage(mImage);
        UnloadTexture(mTexture);
    }

    void Generate(ska::flat_hash_map<block_id_t, BlockDef> &blockDefs)
    {
        int count = blockDefs.size();
        int cols = static_cast<int>(std::ceil(std::sqrt(static_cast<float>(count))));
        int rows = (count + cols - 1) / cols;

        int imgWidth = cols * BLOCK_PX_SIZE;
        int imgHeight = rows * BLOCK_PX_SIZE;

        mImage = GenImageColor(imgWidth, imgHeight, BLANK);

        int i = 0;
        for (auto &[id, blockDef] : blockDefs)
        {
            Image blockImage = LoadImage(blockDef.texture.c_str());

            float x = static_cast<float>(i % cols * BLOCK_PX_SIZE);
            float y = static_cast<float>(i / cols * BLOCK_PX_SIZE);
            i++;

            Rectangle srcRect = {0.0f, 0.0f, static_cast<float>(blockImage.width),
                                 static_cast<float>(blockImage.height)};
            Rectangle dstRect = {x, y, BLOCK_PX_SIZE, BLOCK_PX_SIZE};

            ImageDraw(&mImage, blockImage, srcRect, dstRect, WHITE);
            UnloadImage(blockImage);

            blockDef.atlasSource = dstRect;
        }

        mTexture = LoadTextureFromImage(mImage);
        SetTextureFilter(mTexture, TEXTURE_FILTER_POINT);

        BlockDef::atlasTexture = mTexture;
    }

    const Texture2D &GetTexture() const
    {
        return mTexture;
    }

  private:
    Image mImage{};
    Texture2D mTexture{};
};

#endif /* _ATLAS_HPP */
