#ifndef PARSER_CONTEXT_H
#define PARSER_CONTEXT_H

#include "../lexer/lexer.h"
#include "../common/ast.h"

// 解析器状态和上下文
typedef struct {
    Lexer* lexer;           // 词法分析器
    Token current_token;    // 当前标记
    Token lookahead;        // 前瞻标记
    ASTNode* root;          // AST根节点
    char* error_message;    // 错误信息
    int error_line;         // 错误行号
    int error_column;       // 错误列号
    int panic_mode;         // 错误恢复模式
} ParserContext;

// 上下文操作函数
ParserContext* create_parser_context(Lexer* lexer);
void destroy_parser_context(ParserContext* context);

// 标记处理函数
void advance_token(ParserContext* context);
int match_token(ParserContext* context, TokenType type);
void expect_token(ParserContext* context, TokenType type);
void synchronize(ParserContext* context);

// 错误处理
void parser_error(ParserContext* context, const char* format, ...);

#endif // PARSER_CONTEXT_H
