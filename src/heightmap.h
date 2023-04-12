#pragma once

#include <cmath>
#include <cstdint>


namespace Heightmap
{
    constexpr int32_t cellHeight = 11;
    constexpr int32_t cellWidth = 30;

    constexpr int32_t mapWidth = 512;
    constexpr int32_t mapHeight = 512;

    struct HeightMap_t
    {
        uint8_t data[mapWidth][mapHeight];
    };

    struct Coord_t
    {
        int32_t x;
        int32_t y;
    };


    [[nodiscard]] int32_t distance(const HeightMap_t& heightMap, const Coord_t& start, const Coord_t& end) noexcept;
}