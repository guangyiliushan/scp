CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = ./src/main
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(SRC_DIR)/ast.c \
	$(SRC_DIR)/lexer.c \
	$(SRC_DIR)/parser.c \
	$(SRC_DIR)/syntax_analyzer.c \
	$(SRC_DIR)/code_generator.c \
	$(SRC_DIR)/compiler.c

INCLUDES = -I$(SRC_DIR)

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

EXECUTABLE = $(BIN_DIR)/scp

all: directories $(EXECUTABLE)

directories:
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

test: $(EXECUTABLE)
	$(EXECUTABLE) tests/basic/main.scp

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"

.PHONY: all directories clean test