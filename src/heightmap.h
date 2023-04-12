#pragma once

#include <cmath>


namespace Heightmap
{
    constexpr int32_t cellHeight = 11;
    constexpr int32_t cellWidth = 30;

    constexpr int32_t mapWidth = 512;
    constexpr int32_t mapHeight = 512;

    using HeightMap_t = struct
    {
        uint8_t data[mapWidth][mapHeight];
    };

    using Coord_t = struct
    {
        int32_t x;
        int32_t y;
    };

    [[nodiscard]] int32_t distance(const HeightMap_t& heightMap, const Coord_t& start, const Coord_t& end) noexcept;
}