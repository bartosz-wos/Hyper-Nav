# Compiler settings 
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Iinclude

# Target executable name
TARGET = hypernav

# Source files and Object files
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile source files to object files
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET) build/*.o

# Run
run: $(TARGET)
	./$(TARGET)
