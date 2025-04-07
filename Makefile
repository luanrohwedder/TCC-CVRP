CC = g++
CFLAGS = -Wall -Iinclude -g

SRC_DIR = src
CORE_DIR = $(SRC_DIR)/Core
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TEST_DIR = tests
USE_OPENMP ?= 0
    
ifeq ($(USE_OPENMP), 1)
	CFLAGS += -fopenmp
endif

EXEC = $(BIN_DIR)/ex

SRCS = $(CORE_DIR)/genetic_algorithm.cpp \
       $(CORE_DIR)/memetic_algorithm.cpp \
	   $(SRC_DIR)/main.cpp 


OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(EXEC)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test: $(OBJS) $(TEST_DIR)/unit_tests.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_executable $^
	./$(BUILD_DIR)/test_executable

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean test
