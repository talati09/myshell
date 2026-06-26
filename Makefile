# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude

# Source files and target executable
SRCS = src/shell.cpp src/tokenizer.cpp src/parser.cpp src/builtins.cpp src/executor.cpp src/history.cpp
TARGET = myshell

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Cleanup rule
clean:
	rm -f $(TARGET)