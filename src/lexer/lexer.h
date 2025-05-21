// 词法分析器头文件

#ifndef LEXER_H
#define LEXER_H

#include "token_types.h"
#include <stdlib.h>  // 添加标准库头文件

// 错误类型枚举
typedef enum {
    LEXER_ERROR_NONE,               // 无错误
    LEXER_ERROR_INVALID_CHAR,       // 无效字符
    LEXER_ERROR_UNTERMINATED_STRING,// 未终止的字符串
    LEXER_ERROR_UNTERMINATED_CHAR,  // 未终止的字符字面量
    LEXER_ERROR_UNTERMINATED_COMMENT,// 未终止的注释
    LEXER_ERROR_NUMBER_FORMAT,      // 数字格式错误
    LEXER_ERROR_INVALID_ESCAPE,     // 无效的转义序列
    LEXER_ERROR_MEMORY              // 内存分配错误
} LexerErrorType;

// 标记结构
typedef struct {
    TokenType type;      // 标记类型
    char* value;         // 标记值
    int line;            // 行号
    int column;          // 列号
    int length;          // 标记长度
    char* source_line;   // 源代码行（用于错误报告）
} Token;

// 词法分析器结构
typedef struct Lexer Lexer;

// 函数原型
Lexer* create_lexer(const char* source);
Token get_next_token(Lexer* lexer);
Token peek_token(Lexer* lexer);
const char* get_error_message(Lexer* lexer);
LexerErrorType get_error_type(Lexer* lexer);
void clear_error(Lexer* lexer);
void destroy_lexer(Lexer* lexer);

// 辅助函数
const char* token_type_to_string(TokenType type);
const char* lexer_error_to_string(LexerErrorType error);
void print_token(Token* token);

#endif // LEXER_H