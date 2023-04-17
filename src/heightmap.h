#pragma once

#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <algorithm>
#include <tuple>
#include <iostream>


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

        bool operator==(const Coord_t& rhs) const noexcept
        {
            return x == rhs.x && y == rhs.y;
        }
    };


    namespace idastar 
    {
        using Cost_t = double;
        using SearchState_t = std::tuple<Cost_t, bool>;
        using MoveState_t = std::tuple<Coord_t, Cost_t, bool>;
        enum class Direction_t : uint8_t
        {
            UP,
            DOWN,
            LEFT,
            RIGHT,
            UP_LEFT,
            UP_RIGHT,
            DOWN_LEFT,
            DOWN_RIGHT,
            NONE,
        };

        [[nodiscard]] SearchState_t ida_star(const HeightMap_t& hm, const Coord_t& root, const Coord_t& goal) noexcept;
    }



    [[nodiscard]] double distance(const HeightMap_t& heightMap, const Coord_t& start, const Coord_t& end) noexcept;
}