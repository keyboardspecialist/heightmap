# Heightmap Distance Calc

  This tool is designed to calculate distance from terrain heightmap data. Each heightmap cell has a spatial resolution of 30m and an 11m height value.
  
  Height maps should be 512x512 unsigned 8bit data
  
  It uses Bresenham's line algorithm to traverse across the terrain.
  
  Time spent: 4 hours
  
## Prerequisites
  * bash 
  * C++ compiler (clang or GCC)
  * GNU Make

## Building and Running
These are run from the root repo folder
  * ```make``` build all
  * ```make clean``` cleans the build dir
  * ```make test``` runs the test suite, builds if necessary

### Built and Tested
  - [x] MacOS
  - [ ] Linux
  - [ ] Windows


  
  
  
  
  
  
 
