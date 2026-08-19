#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <vector>

#include "raylib.h"

#include "Random.hpp"

#define BLOCK_DEF()                                                                                                    \
    IT(ArmorHeavy, Gray)                                                                                               \
    IT(ArmorHeavy, Green)                                                                                              \
    IT(ArmorHeavy, Red)                                                                                                \
    IT(ArmorHeavy, Teal)                                                                                               \
    IT(ArmorHeavy, Violet)                                                                                             \
    IT(ArmorLight, Gray)                                                                                               \
    IT(ArmorLight, Green)                                                                                              \
    IT(ArmorLight, Red)                                                                                                \
    IT(ArmorLight, Teal)                                                                                               \
    IT(ArmorLight, Violet)

class Map
{
  public:
    enum class Block
    {
#define IT(type, color) type##color,
        BLOCK_DEF()

        Count
#undef IT
    };

    Map()
    {
        for (std::size_t i = 0; i < static_cast<int>(Block::Count); i++)
        {
            block_textures[i] = LoadTexture(block_texture_paths[i]);
            SetTextureFilter(block_textures[i], TEXTURE_FILTER_POINT);
        }
        Generate();
    }

    ~Map()
    {
        for (Texture2D texture : block_textures)
        {
            UnloadTexture(texture);
        }
    }

    void Draw() const
    {
        constexpr float tile_size = 64.0f;

        for (int y = 0; y < blocks.size(); y++)
        {
            for (int x = 0; x < blocks[y].size(); x++)
            {
                Block block = blocks[y][x];

                Texture2D texture = block_textures[static_cast<int>(block)];

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
                Vector2 position = {(tile_size / 2) * (x - y), (tile_size / 2) * y + (tile_size / 4) * (x - y)};

                DrawTextureV(texture, position, WHITE);
            }
        }
    }

  private:
    std::vector<std::vector<Block>> blocks;

    std::array<const char *, static_cast<int>(Block::Count)> block_texture_paths{
#define IT(type, color) "sprites/" #type "/" #color ".png",
        BLOCK_DEF()
#undef IT
    };

    std::array<Texture2D, static_cast<int>(Block::Count)> block_textures{};

    Random rand{};

    void Generate()
    {
        blocks.clear();

        for (std::size_t y = 0; y < 20; y++)
        {
            std::vector<Block> blocks_x;

            for (std::size_t x = 0; x < 20; x++)
            {
                Block block = static_cast<Block>(rand.int_rand(0, static_cast<int>(Block::Count) - 1));
#if 1
                if (x == 0 && y == 0)
                {
                    block = Block::ArmorHeavyViolet;
                }
                else if (y == 0)
                {
                    block = Block::ArmorHeavyRed;
                }
                else if (x == 0)
                {
                    block = Block::ArmorHeavyTeal;
                }
                else
                {
                    block = Block::ArmorLightGray;
                }
#endif
                blocks_x.push_back(block);
            }

            blocks.push_back(blocks_x);
        }
    }
};

#endif /* _MAP_HPP */
