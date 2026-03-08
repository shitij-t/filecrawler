# Define variables for compiler and flags
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -I./include
LDFLAGS = -lpthread

# Define variables for source files
SRCS = ./src/main.cpp ./src/SearchEngine.cpp
TARGET = program

# --- Rules ---
all: $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compile directly from source files, no object files
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(TARGET)
	@echo "Clean complete."

.PHONY: run
run: $(TARGET)
	./$(TARGET)
