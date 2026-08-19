#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <vector>

#include "raylib.h"

#include "Random.hpp"

#define BLOCK_TYPE_DEF()                                                                                               \
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

static constexpr float TILE_SIZE = 64.0f; // TODO

static constexpr int TILES_WIDTH = 40;
static constexpr int TILES_HEIGHT = 60;

class Map
{
  public:
    enum class BlockType
    {
#define IT(blockType, color) blockType##color,
        BLOCK_TYPE_DEF()

        Count
#undef IT
    };
    struct Block
    {
        BlockType blockType;

        bool selected; // TODO
    };

    Map()
    {
        for (std::size_t i = 0; i < static_cast<int>(BlockType::Count); i++)
        {
            blockTypeTextures[i] = LoadTexture(blockTypeTexturePaths[i]);
            SetTextureFilter(blockTypeTextures[i], TEXTURE_FILTER_POINT);
        }
        Generate();
    }

    ~Map()
    {
        for (Texture2D texture : blockTypeTextures)
        {
            UnloadTexture(texture);
        }
    }

    void Draw() const
    {
        for (int y = 0; y < blocks.size(); y++)
        {
            for (int x = 0; x < blocks[y].size(); x++)
            {
                const Block &block = blocks[y][x];
                BlockType blockType = block.blockType;

                Texture2D texture = blockTypeTextures[static_cast<int>(blockType)];
                Vector2 position = GetWorldToScreen({static_cast<float>(x), static_cast<float>(y)});

                DrawTextureV(texture, position, block.selected ? PINK : WHITE);
            }
        }
    }

    void OnMouseButtonLeft(Vector2 mousePosition)
    {
        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(position.x);
        int y = static_cast<int>(position.y);

        if (y < 0 || y >= blocks.size())
        {
            return;
        }
        if (x < 0 || x >= blocks[y].size())
        {
            return;
        }

        blocks[y][x].selected = true;
    }

    void OnMouseButtonRight(Vector2 mousePosition)
    {
    }

  private:
    std::vector<std::vector<Block>> blocks;

    std::array<const char *, static_cast<int>(BlockType::Count)> blockTypeTexturePaths{
#define IT(blockType, color) "sprites/" #blockType "/" #color ".png",
        BLOCK_TYPE_DEF()
#undef IT
    };

    std::array<Texture2D, static_cast<int>(BlockType::Count)> blockTypeTextures{};

    Random rand{};

    void Generate()
    {
        blocks.clear();

        for (std::size_t y = 0; y < 20; y++)
        {
            std::vector<Block> blocks_x;

            for (std::size_t x = 0; x < 20; x++)
            {
                BlockType blockType = static_cast<BlockType>(rand.int_rand(0, static_cast<int>(BlockType::Count) - 1));
#if 1
                if (x == 0 && y == 0)
                {
                    blockType = BlockType::ArmorHeavyViolet;
                }
                else if (y == 0)
                {
                    blockType = BlockType::ArmorHeavyRed;
                }
                else if (x == 0)
                {
                    blockType = BlockType::ArmorHeavyTeal;
                }
                else
                {
                    blockType = BlockType::ArmorLightGray;
                }
#endif
                Block block = {.blockType = blockType, .selected = false};

                blocks_x.push_back(block);
            }

            blocks.push_back(blocks_x);
        }
    }

    static Vector2 GetScreenToWorld(Vector2 position)
    {
        // Rotate isometric grid
        //   /\
        //  /  \
        // y    x
        //
        // to square grid.
        //  ------ x
        // |
        // |
        // y
        return {(1 / TILE_SIZE) * (position.x + 2.0f * position.y), (1 / TILE_SIZE) * (2.0f * position.y - position.x)};
    }

    static Vector2 GetWorldToScreen(Vector2 position)
    {
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
        return {(TILE_SIZE / 2) * (position.x - position.y),
                (TILE_SIZE / 2) * position.y + (TILE_SIZE / 4) * (position.x - position.y)};
    }
};

#endif /* _MAP_HPP */
