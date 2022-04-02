CXX = clang++

WARNINGS = -pedantic -Wall -Werror -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function

CXXFLAGS = $(CS225) -std=c++14 -stdlib=libc++ -O3 $(WARNINGS)
DEBUGFLAGS = $(CS225) -std=c++14 -stdlib=libc++ -O0 $(WARNINGS) -g

CPP_FILES = $(filter-out main.cpp, $(wildcard *.cpp))
H_FILES = $(patsubst *.cpp, *.h, $(CPP_FILES))

EXE_NAME = recombination
DEBUG_NAME = debug

all: $(EXE_NAME)

$(EXE_NAME): $(CPP_FILES) $(H_FILES)
	$(CXX) $(CXXFLAGS) main.cpp $(CPP_FILES) -o $(EXE_NAME)

$(DEBUG_NAME): $(CPP_FILES) $(H_FILES)
	$(CXX) $(CXXFLAGS) main.cpp $(CPP_FILES) -o $(DEBUG_NAME)

clean:
	rm $(EXE_NAME)
	rm $(DEBUG_NAME)

.PHONY: all debug clean