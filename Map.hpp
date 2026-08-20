#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <assert.h>
#include <cmath>
#include <algorithm>
#include <vector>

#include "raylib.h"

#include "Random.hpp"

// IT(group1, group2, color)
#define BLOCKTYPE_DEF_DEBUG()                                                                                          \
    IT(Debug, , Null) /* 0 */                                                                                          \
    IT(Debug, , SelectionAdd)                                                                                          \
    IT(Debug, , SelectionRemove)
#define BLOCKTYPE_DEF_ARMOR()                                                                                          \
    IT(Armor, Heavy, Default)                                                                                          \
    IT(Armor, Heavy, Green)                                                                                            \
    IT(Armor, Heavy, Red)                                                                                              \
    IT(Armor, Heavy, Teal)                                                                                             \
    IT(Armor, Heavy, Violet)                                                                                           \
    IT(Armor, Light, Default)                                                                                          \
    IT(Armor, Light, Green)                                                                                            \
    IT(Armor, Light, Red)                                                                                              \
    IT(Armor, Light, Teal)                                                                                             \
    IT(Armor, Light, Violet)

static constexpr float TILE_SIZE = 64.0f; // TODO

static constexpr int TILES_WIDTH = 512;
static constexpr int TILES_HEIGHT = 512;

class Map
{
  public:
    enum class BlockType
    {
        DebugEmpty = -1,

#define IT(group1, group2, color) group1##group2##color,
        BLOCKTYPE_DEF_DEBUG()
        BLOCKTYPE_DEF_ARMOR()
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
            mBlockTypeTextures[i] = LoadTexture(mBlockTypeTexturePaths[i]);
            SetTextureFilter(mBlockTypeTextures[i], TEXTURE_FILTER_POINT);
        }
        Generate();
    }

    ~Map()
    {
        for (Texture2D texture : mBlockTypeTextures)
        {
            UnloadTexture(texture);
        }
    }

    void Draw() const
    {
        int minX = std::min(mSelection.fromX, mSelection.toX);
        int maxX = std::max(mSelection.fromX, mSelection.toX);
        int minY = std::min(mSelection.fromY, mSelection.toY);
        int maxY = std::max(mSelection.fromY, mSelection.toY);

        for (int x = 0; x < mBlocks.size(); x++)
        {
            bool hitX = (x >= minX) && (x <= maxX);

            for (int y = 0; y < mBlocks.front().size(); y++)
            {
                const Block &block = mBlocks[x][y];

                bool empty = (block.blockType == BlockType::DebugEmpty);
                bool hit = hitX && (y >= minY) && (y <= maxY);

                Vector2 position;
                if (!empty || hit)
                {
                    position = GetWorldToScreen({static_cast<float>(x), static_cast<float>(y)});
                    position.x -= TILE_SIZE / 2;
                }

                if (!empty)
                {
                    Texture2D texture = mBlockTypeTextures[static_cast<int>(block.blockType)];

                    DrawTextureV(texture, position, hit ? GRAY : WHITE);
                }

                if (hit)
                {
                    Texture2D texture;
                    if (mSelection.button == MOUSE_BUTTON_LEFT)
                    {
                        texture = mBlockTypeTextures[static_cast<int>(BlockType::DebugSelectionAdd)];
                    }
                    else if (mSelection.button == MOUSE_BUTTON_RIGHT)
                    {
                        texture = mBlockTypeTextures[static_cast<int>(BlockType::DebugSelectionRemove)];
                    }

                    DrawTextureV(texture, position, empty ? DARKGRAY : WHITE);
                }
            }
        }
    }

    void OnMouseButtonDown(Vector2 mousePosition)
    {
        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        mSelection.toX = x;
        mSelection.toY = y;
    }

    void OnMouseButtonPressed(Vector2 mousePosition, MouseButton button)
    {
        assert((button == MOUSE_BUTTON_LEFT) || (button == MOUSE_BUTTON_RIGHT));

        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        mSelection = {.button = button, .fromX = x, .fromY = y, .toX = x, .toY = y};
    }

    void OnMouseButtonReleased()
    {
        int minX = std::min(mSelection.fromX, mSelection.toX);
        int maxX = std::max(mSelection.fromX, mSelection.toX);
        int minY = std::min(mSelection.fromY, mSelection.toY);
        int maxY = std::max(mSelection.fromY, mSelection.toY);

        minX = std::max(minX, 0);
        maxX = std::min(maxX, static_cast<int>(mBlocks.size()) - 1);
        minY = std::max(minY, 0);
        maxY = std::min(maxY, static_cast<int>(mBlocks.front().size()) - 1);

        for (int x = minX; x <= maxX; x++)
        {
            for (int y = minY; y <= maxY; y++)
            {
                Block &block = mBlocks[x][y];

                if (mSelection.button == MOUSE_BUTTON_LEFT)
                {
                    block.blockType = GetRandomBlockType(false); // add
                }
                else if (mSelection.button == MOUSE_BUTTON_RIGHT)
                {
                    block.blockType = BlockType::DebugEmpty; // remove
                }
            }
        }

        mSelection.fromX = -1;
        mSelection.fromY = -1;
        mSelection.toX = -1;
        mSelection.toY = -1;
    }

  private:
    std::vector<std::vector<Block>> mBlocks;

    std::array<const char *, static_cast<int>(BlockType::Count)> mBlockTypeTexturePaths{
#define IT(group1, group2, color) "assets/blocks/" #group1 "/" #color ".png",
        BLOCKTYPE_DEF_DEBUG()
#undef IT
#define IT(group1, group2, color) "assets/blocks/" #group1 "/" #group2 "/" #color ".png",
            BLOCKTYPE_DEF_ARMOR()
#undef IT
    };

    std::array<Texture2D, static_cast<int>(BlockType::Count)> mBlockTypeTextures{};

    Random mRand{};

    struct Selection
    {
        MouseButton button;

        int fromX = -1;
        int fromY = -1;
        int toX = -1;
        int toY = -1;
    } mSelection;

    void Generate()
    {
        mBlocks.clear();

        for (int x = 0; x < TILES_WIDTH; x++)
        {
            std::vector<Block> blocksY;

            for (int y = 0; y < TILES_HEIGHT; y++)
            {
                BlockType blockType = GetRandomBlockType(true);
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

                blocksY.push_back(block);
            }

            mBlocks.push_back(blocksY);
        }
    }

    BlockType GetRandomBlockType(bool allowEmpty)
    {
        while (true) // there is a NON-ZERO chance this will hang until the heat death of the universe :P
        {
            BlockType blockType = static_cast<BlockType>(mRand.int_rand(-1, static_cast<int>(BlockType::Count) - 1));

            switch (blockType) // yes, i am THIS lazy to fix the BLOCK_TYPE_DEF() approach
            {
                case BlockType::DebugEmpty:
                    if (!allowEmpty)
                    {
                        continue;
                    }
                    [[fallthrough]];
#define IT(group1, group2, color) case BlockType::group1##group2##color:
                    BLOCKTYPE_DEF_ARMOR()
#undef IT
                    return blockType;

                default:
                    continue;
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
