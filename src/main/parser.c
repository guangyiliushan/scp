#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>

// Define the Parser structure
struct Parser {
    Lexer* lexer;          // 词法分析器
    Token current_token;    // 当前标记
    ASTNode* root;         // AST根节点
    char* error_message;    // 错误信息
};

// 获取下一个标记
static void advance(Parser* parser) {
    parser->current_token = get_next_token(parser->lexer);
}

// 检查当前标记类型
static int match(Parser* parser, TokenType type) {
    return parser->current_token.type == type;
}

// 期望并消费一个标记
static void expect(Parser* parser, TokenType type) {
    if (match(parser, type)) {
        advance(parser);
    } else {
        // 设置错误信息
        if (parser->error_message) {
            free(parser->error_message);
        }
        parser->error_message = strdup("语法错误：意外的标记类型");
    }
}

void parse_source(Parser* parser, const char* source) {
    if (!parser || !source) {
        if (parser) {
            parser->error_message = strdup("无效的源代码或解析器");
        }
        return;
    }
    
    // 创建词法分析器
    if (parser->lexer) {
        destroy_lexer(parser->lexer);
    }
    parser->lexer = create_lexer(source);
    
    // 获取第一个标记
    advance(parser);
    
    // 创建程序根节点
    ASTNode* program_node = create_ast_node(NODE_PROGRAM);
    program_node->program.declarations = NULL;
    program_node->program.declaration_count = 0;
    
    // 解析文件内容直到EOF
    while (parser->current_token.type != TOKEN_EOF) {
        // 尝试解析函数定义
        if (match(parser, TOKEN_KEYWORD_FUN)) {
            ASTNode* func_node = parse_function_definition(parser);
            if (func_node) {
                // 添加到程序节点
                program_node->program.declaration_count++;
                program_node->program.declarations = realloc(
                    program_node->program.declarations, 
                    program_node->program.declaration_count * sizeof(ASTNode*)
                );
                program_node->program.declarations[program_node->program.declaration_count - 1] = func_node;
            }
        } else if (match(parser, TOKEN_KEYWORD_INCLUDE) || match(parser, TOKEN_HASH)) {
            // 处理 include 指令 (#include 或 include)
            advance(parser);
            // 暂时跳过include指令的处理
            while (parser->current_token.type != TOKEN_SEMICOLON && 
                   parser->current_token.type != TOKEN_EOF &&
                   parser->current_token.type != TOKEN_KEYWORD_FUN) {
                advance(parser);
            }
        } else {
            // 跳过未识别的标记
            advance(parser);
        }
    }
    
    parser->root = program_node;
}

Parser* create_parser() {
    Parser* parser = (Parser*)malloc(sizeof(Parser));
    if (parser) {
        parser->lexer = NULL;
        parser->root = NULL;
        parser->error_message = NULL;
        parser->current_token.type = TOKEN_UNKNOWN;
        parser->current_token.value = NULL;
    }
    return parser;
}

void destroy_parser(Parser* parser) {
    if (parser) {
        if (parser->lexer) {
            destroy_lexer(parser->lexer);
        }
        if (parser->root) {
            // TODO: 实现AST节点销毁函数
            // destroy_ast_node(parser->root);
        }
        if (parser->error_message) {
            free(parser->error_message);
        }
        free(parser);
    }
}

// 实现函数定义、函数调用等语法结构的解析
ASTNode* parse_function_definition(struct Parser* parser) {
    if (match(parser, TOKEN_KEYWORD_FUN)) {
        advance(parser); // 消费'fun'关键字

        // 期望函数名（标识符）
        if (!match(parser, TOKEN_IDENTIFIER)) {
            if (parser->error_message) free(parser->error_message);
            parser->error_message = strdup("语法错误：期望函数名");
            return NULL;
        }
        char* function_name = strdup(parser->current_token.value);
        advance(parser);

        // 解析函数参数
        expect(parser, TOKEN_LPAREN);
        
        // 跳过参数列表处理
        while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
            advance(parser);
        }
        
        expect(parser, TOKEN_RPAREN);

        // 解析函数体
        expect(parser, TOKEN_LBRACE);
        
        // 跳过函数体处理
        while (!match(parser, TOKEN_RBRACE) && !match(parser, TOKEN_EOF)) {
            advance(parser);
        }
        
        expect(parser, TOKEN_RBRACE);

        // 创建函数节点
        ASTNode* function_node = create_ast_node(NODE_FUNCTION);
        function_node->type = NODE_FUNCTION;
        function_node->function.name = function_name;
        function_node->function.parameters = NULL;
        function_node->function.param_count = 0;
        function_node->function.body = NULL;
        function_node->function.return_type = NULL;

        return function_node;
    }
    return NULL;
}

// 解析语句
static ASTNode* parse_statement(Parser* parser) {
    // TODO: 实现语句解析
    return NULL;
}

// 解析表达式
static ASTNode* parse_expression(Parser* parser) {
    // TODO: 实现表达式解析
    return NULL;
}

ASTNode* get_ast_root(Parser* parser) {
    return parser->root;
}