#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// 定义词法分析器结构
struct Lexer {
    char* source;          // 源代码
    int position;          // 当前位置
    int line;              // 当前行号
    int column;            // 当前列号
    char current_char;     // 当前字符
    Token current_token;   // 当前标记
    char* error_message;   // 错误信息
};

// 创建词法分析器
Lexer* create_lexer(const char* source) {
    Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
    if (lexer) {
        lexer->source = strdup(source);
        lexer->position = 0;
        lexer->line = 1;
        lexer->column = 1;
        lexer->current_char = lexer->source[0];
        lexer->error_message = NULL;
        
        // 初始化当前标记
        lexer->current_token.type = TOKEN_UNKNOWN;
        lexer->current_token.value = NULL;
        lexer->current_token.line = 1;
        lexer->current_token.column = 1;
    }
    return lexer;
}

// 前进到下一个字符
static void advance(Lexer* lexer) {
    if (lexer->current_char == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }

    lexer->position++;
    if (lexer->position < (int)strlen(lexer->source)) { // 强制转换 strlen 返回值为 int
        lexer->current_char = lexer->source[lexer->position];
    } else {
        lexer->current_char = '\0'; // EOF
    }
}

// 查看下一个字符
static char peek(Lexer* lexer) {
    if (lexer->position + 1 < (int)strlen(lexer->source)) {
        return lexer->source[lexer->position + 1];
    }
    return '\0';  // 如果已经到达末尾，返回 EOF
}

// 跳过空白字符
static void skip_whitespace(Lexer* lexer) {
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {
        advance(lexer);
    }
}

// 跳过注释
static void skip_comment(Lexer* lexer) {
    // 单行注释
    if (lexer->current_char == '/' && lexer->source[lexer->position + 1] == '/') {
        while (lexer->current_char != '\0' && lexer->current_char != '\n') {
            advance(lexer);
        }
    }
    // 多行注释
    else if (lexer->current_char == '/' && lexer->source[lexer->position + 1] == '*') {
        advance(lexer); // 跳过 '/'
        advance(lexer); // 跳过 '*'
        
        while (lexer->current_char != '\0') {
            if (lexer->current_char == '*' && lexer->source[lexer->position + 1] == '/') {
                advance(lexer); // 跳过 '*'
                advance(lexer); // 跳过 '/'
                break;
            }
            advance(lexer);
        }
    }
}

// 读取标识符或关键字
static Token read_identifier(Lexer* lexer) {
    int start_position = lexer->position;
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    while (lexer->current_char != '\0' && (isalnum(lexer->current_char) || lexer->current_char == '_')) {
        advance(lexer);
    }
    
    int length = lexer->position - start_position;
    char* value = (char*)malloc(length + 1);
    strncpy(value, lexer->source + start_position, length);
    value[length] = '\0';
    
    // 检查是否为关键字
    TokenType type = TOKEN_IDENTIFIER;
    if (strcmp(value, "fun") == 0) {
        type = TOKEN_KEYWORD_FUN;
    } else if (strcmp(value, "if") == 0) {
        type = TOKEN_KEYWORD_IF;
    } else if (strcmp(value, "else") == 0) {
        type = TOKEN_KEYWORD_ELSE;
    } else if (strcmp(value, "while") == 0) {
        type = TOKEN_KEYWORD_WHILE;
    } else if (strcmp(value, "return") == 0) {
        type = TOKEN_KEYWORD_RETURN;
    } else if (strcmp(value, "include") == 0) {
        type = TOKEN_KEYWORD_INCLUDE;
    } else if (strcmp(value, "enum") == 0) {
        type = TOKEN_KEYWORD_ENUM;
    }
    
    // 检查是否为类型
    else if (strcmp(value, "int") == 0) {
        type = TOKEN_TYPE_INT;
    } else if (strcmp(value, "str") == 0) {
        type = TOKEN_TYPE_STR;
    } else if (strcmp(value, "bool") == 0) {
        type = TOKEN_TYPE_BOOL;
    } else if (strcmp(value, "i8") == 0) {
        type = TOKEN_TYPE_I8;
    } else if (strcmp(value, "i16") == 0) {
        type = TOKEN_TYPE_I16;
    } else if (strcmp(value, "i32") == 0) {
        type = TOKEN_TYPE_I32;
    } else if (strcmp(value, "i64") == 0) {
        type = TOKEN_TYPE_I64;
    } else if (strcmp(value, "u8") == 0) {
        type = TOKEN_TYPE_U8;
    } else if (strcmp(value, "u16") == 0) {
        type = TOKEN_TYPE_U16;
    } else if (strcmp(value, "u32") == 0) {
        type = TOKEN_TYPE_U32;
    } else if (strcmp(value, "u64") == 0) {
        type = TOKEN_TYPE_U64;
    }
    
    Token token;
    token.type = type;
    token.value = value;
    token.line = start_line;
    token.column = start_column;
    
    return token;
}

// 读取数字
static Token read_number(Lexer* lexer) {
    int start_position = lexer->position;
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {
        advance(lexer);
    }
    
    // 处理小数点
    if (lexer->current_char == '.') {
        advance(lexer);
        while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {
            advance(lexer);
        }
    }
    
    int length = lexer->position - start_position;
    char* value = (char*)malloc(length + 1);
    strncpy(value, lexer->source + start_position, length);
    value[length] = '\0';
    
    Token token;
    token.type = TOKEN_NUMBER;
    token.value = value;
    token.line = start_line;
    token.column = start_column;
    
    return token;
}

// 读取字符串
static Token read_string(Lexer* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    
    advance(lexer); // 跳过开始的引号
    
    int start_position = lexer->position;
    while (lexer->current_char != '\0' && lexer->current_char != '"') {
        // 处理转义字符
        if (lexer->current_char == '\\') {
            advance(lexer);
        }
        advance(lexer);
    }
    
    int length = lexer->position - start_position;
    char* value = (char*)malloc(length + 1);
    strncpy(value, lexer->source + start_position, length);
    value[length] = '\0';
    
    if (lexer->current_char == '"') {
        advance(lexer); // 跳过结束的引号
    } else {
        // 错误：未闭合的字符串
        if (lexer->error_message) {
            free(lexer->error_message);
        }
        lexer->error_message = strdup("未闭合的字符串");
    }
    
    Token token;
    token.type = TOKEN_STRING;
    token.value = value;
    token.line = start_line;
    token.column = start_column;
    
    return token;
}

// 获取下一个标记
Token get_next_token(Lexer* lexer) {
    // 释放之前的标记值
    if (lexer->current_token.value) {
        free(lexer->current_token.value);
        lexer->current_token.value = NULL;
    }
    
    // 跳过空白字符和注释
    skip_whitespace(lexer);
    if (lexer->current_char == '/' && (lexer->source[lexer->position + 1] == '/' || 
                                      lexer->source[lexer->position + 1] == '*')) {
        skip_comment(lexer);
        skip_whitespace(lexer);
    }
    
    // 检查EOF
    if (lexer->current_char == '\0') {
        Token token;
        token.type = TOKEN_EOF;
        token.value = NULL;
        token.line = lexer->line;
        token.column = lexer->column;
        return token;
    }
    
    // 识别标记
    Token token;
    if (isalpha(lexer->current_char) || lexer->current_char == '_') {
        // 标识符或关键字
        token = read_identifier(lexer);
    } else if (isdigit(lexer->current_char)) {
        // 数字
        token = read_number(lexer);
    } else if (lexer->current_char == '"') {
        // 字符串
        token = read_string(lexer);
    } else {
        // 运算符和标点符号
        int start_line = lexer->line;
        int start_column = lexer->column;
        char current = lexer->current_char;
        
        switch (current) {
            case '+':
                advance(lexer);
                if (lexer->current_char == '+') {
                    advance(lexer);
                    token.type = TOKEN_INCREMENT;
                } else if (lexer->current_char == '=') {
                    advance(lexer);
                    token.type = TOKEN_PLUS_ASSIGN;
                } else {
                    token.type = TOKEN_PLUS;
                }
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            case '-':
                advance(lexer);
                if (lexer->current_char == '-') {
                    advance(lexer);
                    token.type = TOKEN_DECREMENT;
                } 
                // 处理其他-开头的标记...
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            case '*':
                token.type = TOKEN_MULTIPLY;
                break;
            case '/':
                token.type = TOKEN_DIVIDE;
                break;
            case '=':
                advance(lexer);
                if (lexer->current_char == '=') {
                    token.type = TOKEN_EQUAL;
                    advance(lexer);
                } else {
                    token.type = TOKEN_ASSIGN;
                }
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            case '!':
                advance(lexer);
                if (lexer->current_char == '=') {
                    token.type = TOKEN_NOT_EQUAL;
                    advance(lexer);
                } else {
                    token.type = TOKEN_NOT;
                }
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            case '<':
                advance(lexer);
                if (lexer->current_char == '<') {
                    advance(lexer);
                    if (lexer->current_char == '=') {
                        advance(lexer);
                        token.type = TOKEN_SHIFT_LEFT_ASSIGN;
                    }
                    token.type = TOKEN_SHIFT_LEFT;
                } else {
                    token.type = TOKEN_LESS;
                }
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            case '>':
                advance(lexer);
                if (lexer->current_char == '=') {
                    token.type = TOKEN_GREATER_EQUAL;
                    advance(lexer);
                } else {
                    token.type = TOKEN_GREATER;
                }
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            case '(':
                token.type = TOKEN_LPAREN;
                break;
            case ')':
                token.type = TOKEN_RPAREN;
                break;
            case '{':
                token.type = TOKEN_LBRACE;
                break;
            case '}':
                token.type = TOKEN_RBRACE;
                break;
            case '[':
                token.type = TOKEN_LBRACKET;
                break;
            case ']':
                token.type = TOKEN_RBRACKET;
                break;
            case ',':
                token.type = TOKEN_COMMA;
                break;
            case '.':
                token.type = TOKEN_DOT;
                break;
            case ';':
                token.type = TOKEN_SEMICOLON;
                break;
            case ':':
                token.type = TOKEN_COLON;
                break;
            case '#':
                token.type = TOKEN_HASH;
                break;
            case '"':
                token.type = TOKEN_QUOTE;
                break;
            case '@':
                token.type = TOKEN_AT;
                break;
            case '$':
                token.type = TOKEN_DOLLAR;
                break;
            case '_':
                // 判断是否单独的下划线或标识符的一部分
                if (!isalpha(peek(lexer)) && !isdigit(peek(lexer)) && peek(lexer) != '_') {
                    token.type = TOKEN_UNDERSCORE;
                } else {
                    // 作为标识符的一部分处理
                    token = read_identifier(lexer);
                }
                token.line = start_line;
                token.column = start_column;
                token.value = NULL;
                return token;
            default:
                token.type = TOKEN_UNKNOWN;
                if (lexer->error_message) {
                    free(lexer->error_message);
                }
                char error[100];
                sprintf(error, "未知字符: '%c'", current);
                lexer->error_message = strdup(error);
                break;
        }
        
        advance(lexer);
        token.line = start_line;
        token.column = start_column;
        token.value = NULL;
    }
    
    lexer->current_token = token;
    return token;
}

// 销毁词法分析器
void destroy_lexer(Lexer* lexer) {
    if (lexer) {
        if (lexer->source) {
            free(lexer->source);
        }
        if (lexer->current_token.value) {
            free(lexer->current_token.value);
        }
        if (lexer->error_message) {
            free(lexer->error_message);
        }
        free(lexer);
    }
}

// 添加错误处理和位置跟踪
void handle_error(Lexer* lexer, const char* message) {
    if (lexer->error_message) {
        free(lexer->error_message);
    }
    lexer->error_message = strdup(message);
}

// 获取当前位置
void get_position(Lexer* lexer, int* line, int* column) {
    *line = lexer->line;
    *column = lexer->column;
}

// 在已有的静态函数声明部分添加
static void advance(Lexer* lexer);
static void skip_whitespace(Lexer* lexer);
static void skip_comment(Lexer* lexer);
static Token read_identifier(Lexer* lexer);
static Token read_number(Lexer* lexer);
static Token read_string(Lexer* lexer);
static char peek(Lexer* lexer);  // 添加这一行