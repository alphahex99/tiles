#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <cassert>
#include <cmath>
#include <optional>
#include "raylib.h"
#include <vector>

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
        const BlockDef *blockDefSelection = nullptr;
        if (mSelection.button.has_value())
        {
            if (mSelection.button.value() == MOUSE_BUTTON_LEFT)
            {
                blockDefSelection = &mBlockDefManager.GetBlockDef(BLOCK_ID_DEBUG_SELECTION_ADD);
            }
            else if (mSelection.button.value() == MOUSE_BUTTON_RIGHT)
            {
                blockDefSelection = &mBlockDefManager.GetBlockDef(BLOCK_ID_DEBUG_SELECTION_REMOVE);
            }
        }

        int minX = std::min(mSelection.fromX, mSelection.toX);
        int maxX = std::max(mSelection.fromX, mSelection.toX);
        int minY = std::min(mSelection.fromY, mSelection.toY);
        int maxY = std::max(mSelection.fromY, mSelection.toY);

        for (int x = 0; x < mBlocks.size(); x++)
        {
            bool selectionHitX = (blockDefSelection != nullptr) && (x >= minX) && (x <= maxX);

            for (int y = 0; y < mBlocks.front().size(); y++)
            {
                bool empty = (mBlocks[x][y].blockState == BlockState::EMPTY);
                bool selectionHit = selectionHitX && (y >= minY) && (y <= maxY);

                Vector2 position;
                if (!empty || selectionHit)
                {
                    position = GetWorldToScreen({static_cast<float>(x), static_cast<float>(y)});
                    position.x -= BLOCK_PX_SIZE / 2;
                }

                if (!empty)
                {
                    const BlockDef &blockDef = mBlockDefManager.GetBlockDef(mBlocks[x][y].id);

                    DrawTextureRec(*blockDef.atlasTexture, blockDef.atlasSource, position, selectionHit ? GRAY : WHITE);
                }

                if (selectionHit)
                {
                    DrawTextureRec(*blockDefSelection->atlasTexture, blockDefSelection->atlasSource, position,
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

        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        mSelection.toX = x;
        mSelection.toY = y;
    }

    void OnMouseButtonPressed(Vector2 mousePosition, MouseButton button)
    {
        assert((button == MOUSE_BUTTON_LEFT) || (button == MOUSE_BUTTON_RIGHT));

        if (mSelection.button.has_value())
        {
            return;
        }

        Vector2 position = GetScreenToWorld(mousePosition);

        int x = static_cast<int>(std::floor(position.x));
        int y = static_cast<int>(std::floor(position.y));

        mSelection = {.button = button, .fromX = x, .fromY = y, .toX = x, .toY = y};
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
        maxX = std::min(maxX, static_cast<int>(mBlocks.size()) - 1);
        minY = std::max(minY, 0);
        maxY = std::min(maxY, static_cast<int>(mBlocks.front().size()) - 1);

        for (int x = minX; x <= maxX; x++)
        {
            for (int y = minY; y <= maxY; y++)
            {
                if (mSelection.button == MOUSE_BUTTON_LEFT)
                {
                    mBlocks[x][y].blockState = BlockState::FLOOR;
                    mBlocks[x][y].id = mBlockDefManager.GetRandomBlock(mRandom);
                }
                else if (mSelection.button == MOUSE_BUTTON_RIGHT)
                {
                    mBlocks[x][y].blockState = BlockState::EMPTY;
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
    } mSelection;

    void Generate()
    {
        mBlocks.clear();

        for (int x = 0; x < TILES_WIDTH; x++)
        {
            std::vector<Block> xBlocks;

            for (int y = 0; y < TILES_HEIGHT; y++)
            {
                Block block = {BlockState::FLOOR, mBlockDefManager.GetRandomBlock(mRandom)};

                xBlocks.push_back(block);
            }

            mBlocks.push_back(xBlocks);
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
        return {(1.0f / BLOCK_PX_SIZE) * (position.x + 2.0f * position.y),
                (1.0f / BLOCK_PX_SIZE) * (2.0f * position.y - position.x)};
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
        return {(BLOCK_PX_SIZE / 2) * (position.x - position.y),
                (BLOCK_PX_SIZE / 2) * position.y + (BLOCK_PX_SIZE / 4) * (position.x - position.y)};
    }
};

#endif /* _MAP_HPP */
