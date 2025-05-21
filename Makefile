CC = gcc
CFLAGS = -Wall -Wextra -g
SRC_DIR = ./src
BUILD_DIR = build
BIN_DIR = bin

SOURCES = \
	$(SRC_DIR)/common/ast.c \
	$(SRC_DIR)/common/compiler.c \
	$(SRC_DIR)/lexer/lexer.c \
	$(SRC_DIR)/lexer/token_types.h \
	$(SRC_DIR)/parser/parser.c \
	$(SRC_DIR)/parser/syntax_analyzer.c \
	$(SRC_DIR)/codegen/code_generator.c

INCLUDES = -I$(SRC_DIR) -I$(SRC_DIR)/common -I$(SRC_DIR)/lexer -I$(SRC_DIR)/parser -I$(SRC_DIR)/codegen -I$(SRC_DIR)/lib

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

EXECUTABLE = $(BIN_DIR)/scp

all: directories $(EXECUTABLE)

directories:
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BUILD_DIR)\\common" mkdir "$(BUILD_DIR)\\common"
	@if not exist "$(BUILD_DIR)\\lexer" mkdir "$(BUILD_DIR)\\lexer"
	@if not exist "$(BUILD_DIR)\\parser" mkdir "$(BUILD_DIR)\\parser"
	@if not exist "$(BUILD_DIR)\\codegen" mkdir "$(BUILD_DIR)\\codegen"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

test: $(EXECUTABLE)
	$(EXECUTABLE) examples/basic/main.scp

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"

.PHONY: all directories clean test