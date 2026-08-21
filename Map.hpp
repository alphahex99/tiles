#ifndef _MAP_HPP
#define _MAP_HPP

#include <array>
#include <cassert>
#include <cmath>
#include "raylib.h"
#include <vector>

#include "BlockDefManager.hpp"
#include "Random.hpp"

static constexpr float TILE_SIZE = 64.0f; // TODO

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
        int minX = std::min(mSelection.fromX, mSelection.toX);
        int maxX = std::max(mSelection.fromX, mSelection.toX);
        int minY = std::min(mSelection.fromY, mSelection.toY);
        int maxY = std::max(mSelection.fromY, mSelection.toY);

        for (int x = 0; x < mBlocks.size(); x++)
        {
            bool hitX = (x >= minX) && (x <= maxX);

            for (int y = 0; y < mBlocks.front().size(); y++)
            {
                const BlockDef &block = mBlockDefManager.GetBlockDef(mBlocks[x][y]);

                bool empty = false; // TODO
                bool hit = hitX && (y >= minY) && (y <= maxY);

                Vector2 position;
                if (!empty || hit)
                {
                    position = GetWorldToScreen({static_cast<float>(x), static_cast<float>(y)});
                    position.x -= TILE_SIZE / 2;
                }

                if (!empty)
                {
                    DrawTextureV(block.texture, position, hit ? GRAY : WHITE);
                }

                if (hit)
                {
                    Texture2D texture;
                    if (mSelection.button == MOUSE_BUTTON_LEFT)
                    {
                        texture = mBlockDefManager.GetBlockDef(BLOCK_ID_SELECTION_ADD).texture;
                    }
                    else if (mSelection.button == MOUSE_BUTTON_RIGHT)
                    {
                        texture = mBlockDefManager.GetBlockDef(BLOCK_ID_SELECTION_REMOVE).texture;
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
                if (mSelection.button == MOUSE_BUTTON_LEFT)
                {
                    mBlocks[x][y] = mBlockDefManager.GetRandomBlock(mRandom);
                }
                else if (mSelection.button == MOUSE_BUTTON_RIGHT)
                {
                    // TODO: empty
                }
            }
        }

        mSelection.fromX = -1;
        mSelection.fromY = -1;
        mSelection.toX = -1;
        mSelection.toY = -1;
    }

  private:
    BlockDefManager mBlockDefManager{};

    std::vector<std::vector<block_id_t>> mBlocks; // TODO: block_idx_t

    Random mRandom{};

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
            std::vector<block_id_t> xBlocks;

            for (int y = 0; y < TILES_HEIGHT; y++)
            {
                xBlocks.push_back(mBlockDefManager.GetRandomBlock(mRandom));
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
