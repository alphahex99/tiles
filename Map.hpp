#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <cmath>
#include <algorithm>
#include <vector>

#include "raylib.h"

#include "Random.hpp"

#define BLOCK_TYPE_DEF()                                                                                               \
    IT(Debug, Null) /* 0 */                                                                                            \
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
        DebugEmpty = -1,

#define IT(blockType, color) blockType##color,
        BLOCK_TYPE_DEF()
#undef IT

        Count
    };
    struct Block
    {
        BlockType blockType;
    };

    Map()
    {
        for (int i = 0; i < static_cast<int>(BlockType::Count); i++)
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
        int minX = std::min(selection.fromX, selection.toX);
        int maxX = std::max(selection.fromX, selection.toX);
        int minY = std::min(selection.fromY, selection.toY);
        int maxY = std::max(selection.fromY, selection.toY);

        for (int x = 0; x < blocks.size(); x++)
        {
            bool hitX = (x >= minX) && (x <= maxX);

            for (int y = 0; y < blocks[x].size(); y++)
            {
                const Block &block = blocks[x][y];

                if (block.blockType == BlockType::DebugEmpty)
                {
                    continue;
                }

                bool selected = hitX && (y >= minY) && (y <= maxY);

                BlockType blockType = selected ? BlockType::DebugNull : block.blockType;

                Texture2D texture = blockTypeTextures[static_cast<int>(blockType)];
                Vector2 position = GetWorldToScreen({static_cast<float>(x), static_cast<float>(y)});
                position.x -= TILE_SIZE / 2;

                DrawTextureV(texture, position, WHITE);
            }
        }
    }

    void OnMouseButtonLeftDown(Vector2 mousePosition)
    {
        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        selection.toX = x;
        selection.toY = y;
    }

    void OnMouseButtonLeftPressed(Vector2 mousePosition)
    {
        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        selection = {.fromX = x, .fromY = y, .toX = x, .toY = y};
    }

    void OnMouseButtonLeftReleased()
    {
    }

    void OnMouseButtonRightPressed(Vector2 mousePosition)
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

    struct Selection
    {
        int fromX = -1;
        int fromY = -1;
        int toX = -1;
        int toY = -1;
    } selection;

    void Generate()
    {
        blocks.clear();

        for (int x = 0; x < TILES_WIDTH; x++)
        {
            std::vector<Block> blocks_y;

            for (int y = 0; y < TILES_HEIGHT; y++)
            {
                BlockType blockType = GetRandomBlockType();
#if 0
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
                Block block = {.blockType = blockType};

                blocks_y.push_back(block);
            }

            blocks.push_back(blocks_y);
        }
    }

    BlockType GetRandomBlockType()
    {
        while (true) // there is a NON-ZERO chance this will hang until the heat death of the universe :P
        {
            BlockType result = static_cast<BlockType>(rand.int_rand(-1, static_cast<int>(BlockType::Count) - 1));

            if (result != BlockType::DebugNull) // yes, i am THIS lazy to fix the BLOCK_TYPE_DEF() approach
            {
                return result;
            }
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
        //
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
        //
        return {(TILE_SIZE / 2) * (position.x - position.y),
                (TILE_SIZE / 2) * position.y + (TILE_SIZE / 4) * (position.x - position.y)};
    }
};

#endif /* _MAP_HPP */
