// Header file for SCP Language Parser

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

// Define the Parser structure
typedef struct Parser Parser;

// 解析器结构在实现文件中定义

// 错误类型枚举
typedef enum {
    PARSER_ERROR_NONE,           // 无错误
    PARSER_ERROR_SYNTAX,         // 语法错误
    PARSER_ERROR_UNEXPECTED_TOKEN,// 意外的标记
    PARSER_ERROR_MEMORY          // 内存分配错误
} ParserErrorType;

// Function prototypes
Parser* create_parser();
void parse_source(Parser* parser, const char* source);
void destroy_parser(Parser* parser);
ASTNode* parse_function_definition(Parser* parser);

// AST相关函数
ASTNode* get_ast_root(Parser* parser);

// 错误处理函数
ParserErrorType get_parser_error(Parser* parser);
const char* get_parser_error_message(Parser* parser);
void clear_parser_error(Parser* parser);

#endif // PARSER_H