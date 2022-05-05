CXX = clang++

WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

CXXFLAGS = $(CS225) -std=c++11 -O3 $(WARNINGS)
DEBUGFLAGS = $(CS225) -std=c++11 -O0 $(WARNINGS) -g

CPP_FILES = $(filter-out main.cpp, $(wildcard *.cpp))
H_FILES = $(wildcard *.h)

EXE_NAME = recombination
DEBUG_NAME = debug
TEST_NAME = test

all: $(EXE_NAME)

$(EXE_NAME): main.cpp $(CPP_FILES) $(H_FILES)
	$(CXX) $(CXXFLAGS) main.cpp $(CPP_FILES) -o $(EXE_NAME)

$(DEBUG_NAME): main.cpp $(CPP_FILES) $(H_FILES)
	$(CXX) $(DEBUGFLAGS) main.cpp $(CPP_FILES) -o $(DEBUG_NAME)

$(TEST_NAME): testing/test.cpp $(CPP_FILES) $(H_FILES)
	$(CXX) $(DEBUGFLAGS) testing/test.cpp $(CPP_FILES) -o $(TEST_NAME)

clean:
	rm -f $(EXE_NAME) $(DEBUG_NAME) $(TEST_NAME)

.PHONY: all debug test clean