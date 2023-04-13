#include "heightmap.h"

namespace Heightmap
{

int32_t 
distance(const HeightMap_t& heightMap, const Coord_t& start, const Coord_t& end) noexcept
{
    //bresenham algorithm
    int32_t x0{start.x};
    int32_t y0{start.y};
    const int32_t x1{end.x};
    const int32_t y1{end.y};
    const int32_t dx{std::abs(x1 - x0)};
    const int32_t dy{std::abs(y1 - y0)};
    const int32_t sx{x0 < x1 ? 1 : -1};
    const int32_t sy{y0 < y1 ? 1 : -1};
    int32_t err{dx - dy};
    int32_t e2{};
    int32_t cellDistance{};
    uint8_t curH{heightMap.data[start.x][start.y]};

    for(;;)
    {
        uint8_t dh = std::abs(curH - heightMap.data[x0][y0]);
        curH = heightMap.data[x0][y0];
        //std::cout << "Cell: " << x0 << ", " << y0 << " Height: " << (int)heightMap.data[x0][y0] << " Differential: " << (int)dh << " Distance: " << cellDistance << " meters" << std::endl;
        
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
        cellDistance += ((dh * cellHeight) + cellWidth);
    }

    return cellDistance;
}


namespace idastar
{

bool valid_move(const Coord_t& cell)
{
    return cell.x >= 0 && cell.x < mapWidth && cell.y >= 0 && cell.y < mapHeight;
}

std::string_view
dirToString(const Direction_t& dir)
{
    switch(dir)
    {
        case Direction_t::UP:
            return "UP";
        case Direction_t::DOWN:
            return "DOWN";
        case Direction_t::LEFT:
            return "LEFT";
        case Direction_t::RIGHT:
            return "RIGHT";
        case Direction_t::UP_LEFT:
            return "UP_LEFT";
        case Direction_t::UP_RIGHT:
            return "UP_RIGHT";
        case Direction_t::DOWN_LEFT:
            return "DOWN_LEFT";
        case Direction_t::DOWN_RIGHT:
            return "DOWN_RIGHT";
        case Direction_t::NONE:
            return "NONE";
    }
}

MoveState_t
moveCell(const HeightMap_t& hm, const Coord_t& cell, const Direction_t& dir)
{
    Coord_t tc = cell;
    switch(dir)
    {
        case Direction_t::UP:
            tc.y -= 1;
            break;
        case Direction_t::DOWN:
            tc.y += 1;
            break;
        case Direction_t::LEFT:
            tc.x -= 1;
            break;
        case Direction_t::RIGHT:
            tc.x += 1;
            break;
        case Direction_t::UP_LEFT:
            tc.x -= 1;
            tc.y -= 1;
            break;
        case Direction_t::UP_RIGHT:
            tc.x += 1;
            tc.y -= 1;
            break;
        case Direction_t::DOWN_LEFT:
            tc.x -= 1;
            tc.y += 1;
            break;
        case Direction_t::DOWN_RIGHT:
            tc.x += 1;
            tc.y += 1;
            break;
        case Direction_t::NONE:
            break;
    }

    if(valid_move(tc))
    {
        Cost_t cost = cellWidth + (std::abs(hm.data[tc.x][tc.y] - hm.data[cell.x][cell.y]) * cellHeight);
    //    std::cout << "Cell: " << tc.x << ", " << tc.y << " Cost: " << cost << ", Direction: " << dirToString(dir) << std::endl;
        return {tc, cost, true};
    }

    return {tc, 0, false};
}

SearchState_t
dfs_contour(const HeightMap_t& hm,
            const Coord_t cell,
            const Coord_t& goal, 
            Cost_t g, 
            Cost_t bound, 
            Direction_t dir,
            std::function<Cost_t(const Coord_t&,const Coord_t&)> costFn)
{
    Cost_t f = g + costFn(cell, goal);
    if(f > bound)
        return {f, false};
    
    if(cell == goal)
    {
        return {g, true};
    }

    //std::cout << "Cell: " << cell.x << ", " << cell.y << " Cost: " << g << std::endl;

    Coord_t tc = cell;
    Cost_t min{std::numeric_limits<Cost_t>::max()};
    for(auto d : {  Direction_t::UP, 
                    Direction_t::DOWN, 
                    Direction_t::LEFT, 
                    Direction_t::RIGHT,
                    Direction_t::UP_LEFT, 
                    Direction_t::UP_RIGHT, 
                    Direction_t::DOWN_LEFT, 
                    Direction_t::DOWN_RIGHT
                    })
    {
        if(dir == d)
            continue;
        MoveState_t ms = moveCell(hm, tc, dir);
        if(std::get<2>(ms) == true)
        {
            tc = std::get<0>(ms);
            Cost_t ng = g + std::get<1>(ms);
            const auto [t, found] = dfs_contour(hm, tc, goal, ng, bound, d, costFn);
            if(found)
                return {t, true};

            if(t < min)
                min = t;
        }
    }
    return {min, false};
}

SearchState_t
ida_star(const HeightMap_t& hm, const Coord_t& root, const Coord_t& goal) noexcept
{
    auto heuristic   = 
    [](const Coord_t& start, const Coord_t& goal) -> Cost_t
    {
        const int32_t dx{std::abs(start.x - goal.x)};
        const int32_t dy{std::abs(start.y - goal.y)};
        const int32_t D{cellWidth + cellHeight}; 
        const int32_t D2{D * 2};

        return D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy);
    };
    auto bound = heuristic(root, goal);

    std::cout << "Starting IDA* search" << std::endl;
    std::cout << "Heuristic: " << bound << std::endl;
    std::cout << "Root: " << root.x << ", " << root.y << std::endl;
    std::cout << "Goal: " << goal.x << ", " << goal.y << std::endl;

    auto dbgIter{0};
    while(true)
    {
        //descend
        const auto [t, found] = dfs_contour(hm, root, goal, 0, bound, Direction_t::NONE, heuristic);

        if(found)
            return {t, true};

        std::cout << "Iteration: " << dbgIter++ << " Bound: " << bound << std::endl;
        bound = t;
    }
}


} // namespace idastar
} // namespace Heightmap

