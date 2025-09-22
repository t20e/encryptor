CXX = clang++
CXXFLAGS = -std=c++17 -Wall

# Directories
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include

# Final executable
TARGET = ${BUILD_DIR}/encryptor


# --- Automation ---

# Find all source files (.cpp) recursively in ./src
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
# SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Create a list of object files (.o) in ./build 
# Eg., src/main.cpp becomes build/main.o
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Create a list of dependency files (.d) for header tracking
DEPS := $(OBJS:.o=.d) 

# Compiler flags for include directory
# -I tells the compiler where to look for header files
CPPFLAGS = -I$(INCLUDE_DIR)

# Search paths for source files
vpath %.cpp $(shell find $(SRC_DIR) -type d)


# --- RULES ---

# Tell 'make' that 'all' and 'clean' are not files
.PHONY: all clean

all:$(TARGET) # default goal executable file

# Rule to link the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@) # Ensure the target directory exists
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile a .cpp file into .o file
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c -o $@ $<

# Rule to clean up built file
clean:
	rm -rf $(BUILD_DIR)


# Include all the generated dependency files
# THe hyphen '-' tells 'make' to ignore errors if the files don't exist yet.
-include $(DEPS)


