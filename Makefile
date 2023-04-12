ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
SRC_DIR=$(ROOT_DIR)/src
BUILD_DIR=$(ROOT_DIR)/build
TEST_DIR=$(ROOT_DIR)/test

LIBSRC= \
	$(SRC_DIR)/heightmap.cc 

SRC= \
	$(SRC_DIR)/dist.cc

OBJ=$(LIBSRC:$(SRC_DIR)/%.cc=$(BUILD_DIR)/%.o)
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror -pedantic -O3
#CXX=clang++

all: $(BUILD_DIR)/dist builddir

obj:  $(OBJ)

builddir:
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(LIBSRC) | builddir
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/dist: $(SRC) $(OBJ)
	$(CXX) $(CXXFLAGS) $< -o $@ $(OBJ)

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*

.PHONY: test
test: $(BUILD_DIR)/dist
	$(TEST_DIR)/test.sh $(BUILD_DIR)/dist
	


