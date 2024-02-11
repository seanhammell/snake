TARGET_EXEC := snake

# C++ compiler, compiler flags, include directories, and libraries.
CXX := clang
CXXFLAGS := -g -pedantic-errors -Wall -Wextra -Werror
CXXFLAGS += -I .

# Finds all of the .cc files in the specified ./src directories.
SRC_DIR := ./src
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Specifies .o files in the ./build directory, maintaining the same file
# structure as the ./src directory.
BUILD_DIR := ./build
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Makes sure the executable is updated if any dependencies change.
DEPS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.d)
CPPFLAGS := -MMD -MP

# Final build step where the executable is created.
$(BUILD_DIR)/$(TARGET_EXEC) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Build step for C++ source files.
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Cleans up the ./build directory.
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

-include $(DEPS)