#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <cassert>
#include <cmath>
#include <optional>
#include "raylib.h"
#include <vector>

#include "Isometric.hpp"
#include "Block.hpp"
#include "BlockDefManager.hpp"
#include "Random.hpp"

static constexpr int TILES_WIDTH = 512;
static constexpr int TILES_HEIGHT = 512;

class Map
{
  public:
    Map()
    {
        mBlockDefManager.Load();
        Generate();
    }

    void Draw() const
    {
        for (int x = 0; x < TILES_WIDTH; x++)
        {
            bool selectionHitX = mSelection.button.has_value() && (x >= mSelection.xMin) && (x <= mSelection.xMax);

            for (int y = 0; y < TILES_HEIGHT; y++)
            {
                const Block &block = mBlocks[x][y];

                const bool empty = (block.height == 0);
                const bool selectionHit = selectionHitX && (y >= mSelection.yMin) && (y <= mSelection.yMax);

                Vector2 position;
                if (!empty || selectionHit)
                {
                    position = iso::SquareToIso<BLOCK_PX_SIZE>({static_cast<float>(x), static_cast<float>(y)});
                    position.x -= BLOCK_PX_SIZE / 2;
                }
                if (!empty)
                {
                    const BlockDef &blockDef = mBlockDefManager.GetBlockDef(block.index);

                    for (unsigned int height = block.height;;)
                    {
#warning                                                                                                               \
    "TODO: raylib doesn't do a great job batching these and there's no DrawTextureInstanced, replace with quads + DrawMeshInstanced"
                        DrawTextureRec(*blockDef.atlasTexture, blockDef.atlasSource, position,
                                       selectionHit ? GRAY : WHITE);

                        if (--height == 0)
                        {
                            break;
                        }
                        position.y -= BLOCK_PX_SIZE / 2;
                    }
                }
                if (selectionHit)
                {
                    DrawTextureRec(*mSelection.blockDef->atlasTexture, mSelection.blockDef->atlasSource, position,
                                   empty ? DARKGRAY : WHITE);
                }
            }
        }
    }

    void DrawTextureAtlas() const
    {
        mBlockDefManager.DrawTextureAtlas();
    }

    void OnMouseButtonDown(Vector2 mousePosition)
    {
        if (!mSelection.button.has_value())
        {
            return;
        }

        Vector2 position = iso::IsoToSquare<BLOCK_PX_SIZE>(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        mSelection.toX = x;
        mSelection.toY = y;

        mSelection.xMin = std::min(mSelection.fromX, mSelection.toX);
        mSelection.xMax = std::max(mSelection.fromX, mSelection.toX);
        mSelection.yMin = std::min(mSelection.fromY, mSelection.toY);
        mSelection.yMax = std::max(mSelection.fromY, mSelection.toY);
    }

    void OnMouseButtonPressed(Vector2 mousePosition, MouseButton button)
    {
        assert((button == MOUSE_BUTTON_LEFT) || (button == MOUSE_BUTTON_RIGHT));

        if (mSelection.button.has_value())
        {                                     // already holding another button
            mSelection.button = std::nullopt; // cancel selection
            return;
        }

        Vector2 position = iso::IsoToSquare<BLOCK_PX_SIZE>(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        mSelection.button = button;
        mSelection.fromX = x;
        mSelection.fromY = y;
        mSelection.toX = x;
        mSelection.toY = y;

        if (button == MOUSE_BUTTON_LEFT)
        {
            mSelection.blockDef = &mBlockDefManager.GetBlockDef(BLOCK_ID_DEBUG_SELECTION_ADD);
        }
        else if (button == MOUSE_BUTTON_RIGHT)
        {
            mSelection.blockDef = &mBlockDefManager.GetBlockDef(BLOCK_ID_DEBUG_SELECTION_REMOVE);
        }
        mSelection.xMin = x;
        mSelection.xMax = x;
        mSelection.yMin = y;
        mSelection.yMax = y;
    }

    void OnMouseButtonReleased(MouseButton button)
    {
        if (!mSelection.button.has_value() || (mSelection.button.value() != button))
        {
            return;
        }

        int minX = std::min(mSelection.fromX, mSelection.toX);
        int maxX = std::max(mSelection.fromX, mSelection.toX);
        int minY = std::min(mSelection.fromY, mSelection.toY);
        int maxY = std::max(mSelection.fromY, mSelection.toY);

        minX = std::max(minX, 0);
        maxX = std::min(maxX, TILES_WIDTH - 1);
        minY = std::max(minY, 0);
        maxY = std::min(maxY, TILES_HEIGHT - 1);

        for (int x = minX; x <= maxX; x++)
        {
            for (int y = minY; y <= maxY; y++)
            {
                Block &block = mBlocks[x][y];

                if (mSelection.button == MOUSE_BUTTON_LEFT)
                {
                    if (block.height < 3)
                    {
                        if (block.height == 0)
                        {
                            block.index = mBlockDefManager.GetRandomBlock(mRandom);
                        }
                        block.height++;
                    }
                }
                else if (mSelection.button == MOUSE_BUTTON_RIGHT)
                {
                    if (block.height > 0)
                    {
                        block.height--;
                    }
                }
            }
        }

        mSelection.button = std::nullopt;
    }

  private:
    BlockDefManager mBlockDefManager{};

    std::vector<std::vector<Block>> mBlocks;

    Random mRandom{};

    struct Selection
    {
        std::optional<MouseButton> button = std::nullopt;
        int fromX;
        int fromY;
        int toX;
        int toY;

        const BlockDef *blockDef;
        int xMin;
        int yMin;
        int xMax;
        int yMax;
    } mSelection;

    void Generate()
    {
        mBlocks.clear();
        mBlocks.reserve(TILES_WIDTH * TILES_HEIGHT);

        for (int x = 0; x < TILES_WIDTH; x++)
        {
            std::vector<Block> xBlocks;

            for (int y = 0; y < TILES_HEIGHT; y++)
            {
                Block block;

                block.height = mRandom.uint_rand(0, 3);
                block.index = mBlockDefManager.GetRandomBlock(mRandom);

                xBlocks.push_back(std::move(block));
            }

            mBlocks.push_back(std::move(xBlocks));
        }
    }
};

#endif /* _MAP_HPP */
