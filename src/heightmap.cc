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
    int8_t curH{heightMap.data[start.x][start.y]};

    for(;;)
    {
        int8_t dh = std::abs(curH - heightMap.data[x0][y0]);
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

} // namespace Heightmap