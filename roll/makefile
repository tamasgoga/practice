# by Goga Tamas

# compiler
CXX = clang++-4.0 -std=c++14
CXXFLAGS = -Wall -Wextra -Werror

# source files
SRC_REL = main.cpp src/*.cpp
SRC_ALL = $(SRC_REL) test/*.cpp

rel: $(SRC_REL)
	$(CXX) $(CXXFLAGS) -o roll $(SRC_REL)

all: $(SRC_ALL)
	$(CXX) $(CXXFLAGS) -DROLL_TESTING -o roll $(SRC_ALL)
