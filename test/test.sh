#!/bin/bash
DIST=$1
declare -a coordList=("0,0 0,0" "0,0 511,511" "511,511 0,0")
# Run some basic tests on our program

# Test 1: Check that the program runs without error
echo "Test 1: Check that the program runs without error"
eval "$DIST ./data/heightmap1.data 0,0 1,1"
R=$?
if [ $R -eq 0 ]; then
    echo "Test 1 passed"
fi

# Test 2: File not found
echo "Test 2: File not found"
eval "$DIST ./data/bogus.data 4,4 5,5"
R=$?
if [ $R -eq 1 ]; then
    echo "Test 2 passed"
fi

# Test 3: Invalid start point
echo "Test 3: Invalid start point"
eval "$DIST ./data/heightmap1.data -1,-1 5,5"
R=$?
if [ $R -eq 1 ]; then
    echo "Test 3 passed"
fi

# Test 4: Invalid end point
echo "Test 4: Invalid end point"
eval "$DIST ./data/heightmap1.data 4,4 600,400"
R=$?
if [ $R -eq 1 ]; then
    echo "Test 4 passed"
fi

# Test 5: Invalid start and end point
echo "Test 5: Invalid start and end point"
eval "$DIST ./data/heightmap1.data 513,512 600,400"
R=$?
if [ $R -eq 1 ]; then
    echo "Test 5 passed"
fi

# Test 6: Test Coord List
echo "Test 6: Test Coord List"
for coord in "${coordList[@]}"
do
    eval "$DIST ./data/heightmap1.data $coord"
    R=$?
    if [ $R -eq 0 ]; then
        echo "Test 6 passed"
    fi
done

# Test 7: Test Second File
echo "Test 7: Test Second File"
eval "$DIST ./data/heightmap2.data 0,0 1,1"
R=$?
if [ $R -eq 0 ]; then
    echo "Test 7 passed"
fi

# Test 8: Test Coord List
echo "Test 8: Test Coord List"
for coord in "${coordList[@]}"
do
    eval "$DIST ./data/heightmap2.data $coord"
    R=$?
    if [ $R -eq 0 ]; then
        echo "Test 8 passed"
    fi
done


# Test 9: Test Parse Error
echo "Test 9: Test Parse Error"
eval "$DIST ./data/heightmap2.data x,y 0,0"
R=$?
if [ $R -eq 1 ]; then
    echo "Test 9 passed"
fi