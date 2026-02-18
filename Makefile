# Compiler settings 
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra

# Target executable name
TARGET = hypernav

# Source files and Object files
SRC = src/main.cpp
OBJ = build/main.o

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile source files to object files
build/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o build/main.o

# Clean up build artifacts
clean:
	rm -f $(TARGET) build/*.o

# Run
run: $(TARGET)
	./$(TARGET)
