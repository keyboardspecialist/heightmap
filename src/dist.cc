#include <fstream>
#include <iostream>
#include <vector>
#include <string_view>
#include <chrono>

#include "heightmap.h"

constexpr std::string_view usage = 
u8R"(
	Usage: dist <str: input-file> <int: x>,<int:y> <int: x>,<int: y>
	Example: dist input.bin 0,0 511,511
	)";


int
main(int argc, char** argv)
{
	const std::vector<std::string_view> args(argv + 1, argv + argc);
	
	if (args.size() != 3)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		std::cerr << usage << std::endl;
		return 1;
	}

	try
	{
		Heightmap::HeightMap_t heightMap;
		Heightmap::Coord_t start {
			std::stoi(args[1].substr(0, args[1].find(',')).data()), 
			std::stoi(args[1].substr(args[1].find(',') + 1).data())
			};

		Heightmap::Coord_t end {
			std::stoi(args[2].substr(0, args[2].find(',')).data()), 
			std::stoi(args[2].substr(args[2].find(',') + 1).data())
			};

		std::ifstream file(args[0].data(), std::ios::binary);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << args[0] << std::endl;
			return 1;
		}
		if (start.x < 0 || start.x > 511 || start.y < 0 || start.y > 511)
		{
			std::cerr << "Invalid coordinates: " << start.x << ", " << start.y << std::endl;
			return 1;
		}

		if (end.x < 0 || end.x > 511 || end.y < 0 || end.y > 511)
		{
			std::cerr << "Invalid coordinates: " << end.x << ", " << end.y << std::endl;
			return 1;
		}

		file.read(reinterpret_cast<char*>(&heightMap), sizeof(Heightmap::HeightMap_t));
		file.close();

		const auto startClock{std::chrono::high_resolution_clock::now()};
		int32_t totalDistance{Heightmap::distance(heightMap, start, end)};

		std::cout << "Actual Distance between (" << start.x << ", " << start.y 
		<< ") and (" << end.x << ", " << end.y <<"): " << totalDistance << " meters" << std::endl;

		const auto endClock{std::chrono::high_resolution_clock::now()};
		const auto duration{std::chrono::duration_cast<std::chrono::microseconds>(endClock - startClock).count()};
		std::cout << "Time taken: " << duration << "us" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << usage << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unknown error" << std::endl;
		std::cerr << usage << std::endl;
		return 1;
	}

	return 0;
}