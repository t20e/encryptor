# Notes on Make commands
# 	`:=` is the simple expansion, the variable is evaluated only once, at the very first occurrence.
# 	`=` is the recursive expansion, the value of the variable is evaluated every time it's used. This has downsides!
# 	`@` tells `make` to execute the command after it silently, so make doesn't print the command its about to run,.


# Assign variable `CXX` to the clang++ compiler
CXX = clang++ 


# ---------------------- DIRECTORIES ----------------------
BUILD_DIR := ./build
SRC_DIR := src
TESTS_DIR := tests


# ---------------------- TARGETS ----------------------
# Final executables both for the build and the test
APP_TARGET := ${BUILD_DIR}/encryptor 
TEST_TARGET := ${BUILD_DIR}/test_encryptor


# ---------------------- FLAGS ----------------------
# Assign CXXFLAGS variable that defines compiler flags
# `-Wall` enable all compiler warnings
# The `-MMD` and `-MP` Generate dependency (.d) files for header tracking . When you modify a header file, 'make' will know to recompile the source files that include that header file.
CXXFLAGS = -std=c++23 -Wall -MMD -MP
# Add this project's ./include and the homebrew include so we can grab the Google Test library
INCLUDE_PATHS := ./include /opt/homebrew/include
LIBS_PATHS := /opt/homebrew/lib

# Generate compiler and linker flags from paths
# The -I tells the compiler where to look for header files (#include <...>)
# -I for includes, -L for library paths
CPPFLAGS := $(addprefix -I,$(INCLUDE_PATHS))
LDFLAGS := $(addprefix -L, $(LIBS_PATHS))

# Libraries to link against
# -L/opt/homebrew/lib tells the linker where to find the library files (.a or .dylib)
# lgtest_main: GTest main() for tests, -lgtest: Google Test, -lgmock: Google Mock, -lpthread GTest requirement
LDLIBS := -lgtest_main -lgtest -lgmock -lpthread


# ---------------------- SOURCE FILES AUTOMATION ----------------------

# Find all (.cpp) files recursively in the application source (./src) directory
APP_SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
# Find all (.cpp) files recursively in the tests (./tests) directory


TEST_SRCS := $(shell find $(TESTS_DIR) -name "*.cpp")

# For testing, we need to compile te test code and the application code (excluding out projects main.cpp)
APP_SRCS_FOR_TESTS := $(filter-out $(SRC_DIR)/main.cpp, $(APP_SRCS))


# ---------------------- OBJECT FILES ----------------------
# Generate lists of object (.o) file paths in the build directory. With an identical layout
# 	Eg., src/main.cpp becomes build/main.o
APP_OBJS := $(APP_SRCS:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:%.cpp=$(BUILD_DIR)/%.o) $(APP_SRCS_FOR_TESTS:%.cpp=$(BUILD_DIR)/%.o)

# Create a list of dependency files (.d) for header tracking
DEPS := $(APP_OBJS:.o=.d) $(TEST_OBJS:.o=.d)


# ---------------------- RULES ----------------------

# Makefile Automatic Variables 
# 	`$@`: The name of the APP_TARGET.
# 	`$^`: The names of all the prerequisites (e.g., main.o math_utils.o)
# 	`$<`: The name of the first prerequisite (e.g., main.cpp)

# PHONY targets are not actual files. This rule tells `Make` that `all`, `clean`, and 'test' are just commands to be executed. 
.PHONY: all clean test

# Default rule runs when `make` command is typed in the terminal, builds the main application.
all:$(APP_TARGET)

# Rule to build and run the tests with `make test`
test: $(TEST_TARGET)
	@echo "Running tests..."
	@./$(TEST_TARGET)


# Rule for linking all object files into the final main application executable (encryptor).
#  This rule only runs if the executable is missing or an object file has been modified.
$(APP_TARGET): $(APP_OBJS)
	@mkdir -p $(dir $@) # Ensure the build directory exists
	@echo "Linking encryptor application: $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to link the test_encryptor executable
$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p $(dir $@) 
	@echo "Linking test_encryptor application: $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)


# Rule to compile any source (.cpp) file into its corresponding object (.o) file in the build directory.
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Rule to clean-up all generated files. Run with `make clean`.
clean:
	@echo "Cleaning building files..."
	@rm -rf $(BUILD_DIR)


# Include all the generated dependency files
# THe hyphen '-' tells 'make' to ignore errors if the files don't exist yet.
-include $(DEPS)


