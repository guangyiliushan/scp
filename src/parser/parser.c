#include "parser.h"
#include "../lexer/lexer.h"
#include "../common/ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // 添加布尔类型支持

// parser.c

static ASTNode* parse_function_definition(Parser* parser);
static ASTNode* parse_variable_declaration(Parser* parser);
static ASTNode* parse_function_call(Parser* parser);

// Define the Parser structure
struct Parser {
    Lexer* lexer;          // 词法分析器
    Token current_token;    // 当前标记
    ASTNode* root;         // AST根节点
    char* error_message;    // 错误信息
};

// 函数声明
static ASTNode* parse_statement(Parser* parser);
static ASTNode* parse_expression(Parser* parser);
static ASTNode* parse_variable_declaration(Parser* parser);
static ASTNode* parse_function_call(Parser* parser); // 添加此行

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
        } else if (match(parser, TOKEN_KEYWORD_VAR) || match(parser, TOKEN_KEYWORD_VAL)) {
            ASTNode* var_node = parse_variable_declaration(parser);
            if (var_node) {
                // 添加到程序节点
                program_node->program.declaration_count++;
                program_node->program.declarations = realloc(
                    program_node->program.declarations, 
                    program_node->program.declaration_count * sizeof(ASTNode*)
                );
                program_node->program.declarations[program_node->program.declaration_count - 1] = var_node;
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

// 解析函数定义
ASTNode* parse_function_definition(Parser* parser) {
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
        
        // 参数列表处理（暂时跳过）
        while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF)) {
            advance(parser);
        }
        
        expect(parser, TOKEN_RPAREN);
        
        // 解析返回类型（如果有）
        char* return_type = NULL;
        if (match(parser, TOKEN_ARROW)) {
            advance(parser); // 消费箭头
            
            if (match(parser, TOKEN_IDENTIFIER) || 
                match(parser, TOKEN_TYPE_INT) || 
                match(parser, TOKEN_TYPE_STR)) {
                return_type = strdup(parser->current_token.value);
                advance(parser);
            } else {
                if (parser->error_message) free(parser->error_message);
                parser->error_message = strdup("语法错误：期望返回类型");
                return NULL;
            }
        }

        // 解析函数体
        ASTNode* body = NULL;
        if (match(parser, TOKEN_LBRACE)) {
            advance(parser);
            
            // 创建代码块节点
            body = create_ast_node(NODE_BLOCK);
            body->block.statements = NULL;
            body->block.statement_count = 0;
            
            // 解析语句直到遇到右花括号
            while (!match(parser, TOKEN_RBRACE) && !match(parser, TOKEN_EOF)) {
                ASTNode* stmt = parse_statement(parser);
                if (stmt) {
                    // 将语句添加到代码块
                    body->block.statement_count++;
                    body->block.statements = realloc(
                        body->block.statements, 
                        body->block.statement_count * sizeof(ASTNode*)
                    );
                    body->block.statements[body->block.statement_count - 1] = stmt;
                }
            }
            
            expect(parser, TOKEN_RBRACE);
        } else {
            if (parser->error_message) free(parser->error_message);
            parser->error_message = strdup("语法错误：期望函数体");
            return NULL;
        }

        // 创建函数节点
        ASTNode* function_node = create_function(function_name, NULL, 0, return_type, body);
        
        if (return_type) {
            free(return_type); // 已在create_function中复制
        }
        return function_node;
    }
    return NULL;
}

// 解析函数调用
static ASTNode* parse_function_call(Parser* parser) {
    if (!match(parser, TOKEN_IDENTIFIER)) {
        return NULL;
    }
    
    char* function_name = strdup(parser->current_token.value);
    advance(parser);
    
    expect(parser, TOKEN_LPAREN);
    
    // 解析参数
    ASTNode** arguments = NULL;
    int arg_count = 0;
    
    if (!match(parser, TOKEN_RPAREN)) {
        do {
            ASTNode* arg = parse_expression(parser);
            if (arg) {
                arg_count++;
                arguments = realloc(arguments, arg_count * sizeof(ASTNode*));
                arguments[arg_count - 1] = arg;
            }
            
            if (match(parser, TOKEN_COMMA)) {
                advance(parser);
            } else {
                break;
            }
        } while (!match(parser, TOKEN_RPAREN) && !match(parser, TOKEN_EOF));
    }
    
    expect(parser, TOKEN_RPAREN);
    expect(parser, TOKEN_SEMICOLON);
    
    ASTNode* call_node = create_function_call(function_name, arguments, arg_count);
    
    // 释放临时资源
    free(function_name);
    if (arguments) {
        free(arguments); // 注意：参数节点已被转移到call_node，不需要释放
    }
    
    return call_node;
}

// 解析语句
static ASTNode* parse_statement(Parser* parser) {
    if (match(parser, TOKEN_KEYWORD_VAR) || match(parser, TOKEN_KEYWORD_VAL)) {
        return parse_variable_declaration(parser);
    } else if (match(parser, TOKEN_IDENTIFIER)) {
        // 可能是函数调用或变量赋值
        char* identifier = strdup(parser->current_token.value);
        advance(parser);
        
        if (match(parser, TOKEN_LPAREN)) {
            // 函数调用
            free(identifier);
            return parse_function_call(parser);
        } else if (match(parser, TOKEN_ASSIGN)) {
            // 变量赋值
            advance(parser);
            ASTNode* expr = parse_expression(parser);
            expect(parser, TOKEN_SEMICOLON);
            
            ASTNode* var = create_variable(identifier);
            free(identifier);
            
            return create_binary_op(OP_ASSIGN, var, expr);
        }
        
        free(identifier);
    } else if (match(parser, TOKEN_KEYWORD_RETURN)) {
        advance(parser);
        ASTNode* expr = parse_expression(parser);
        expect(parser, TOKEN_SEMICOLON);
        
        return create_return(expr);
    }
    
    // 跳过未识别的语句
    while (!match(parser, TOKEN_SEMICOLON) && !match(parser, TOKEN_EOF)) {
        advance(parser);
    }
    
    if (match(parser, TOKEN_SEMICOLON)) {
        advance(parser);
    }
    
    return NULL;
}

// 解析表达式
static ASTNode* parse_expression(Parser* parser) {
    // 处理字面量、变量和基本表达式
    ASTNode* expr = NULL;
    
    if (match(parser, TOKEN_NUMBER)) {
        int value = atoi(parser->current_token.value);
        expr = create_int_literal(value);
        advance(parser);
    } else if (match(parser, TOKEN_STRING)) {
        expr = create_string_literal(parser->current_token.value);
        advance(parser);
    } else if (match(parser, TOKEN_IDENTIFIER)) {
        expr = create_variable(parser->current_token.value);
        advance(parser);
    }
    
    // 处理二元运算符
    if (expr && (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS) || 
                 match(parser, TOKEN_MULTIPLY) || match(parser, TOKEN_DIVIDE))) {
        OperatorType op;
        
        if (match(parser, TOKEN_PLUS)) {
            op = OP_ADD;
        } else if (match(parser, TOKEN_MINUS)) {
            op = OP_SUB;
        } else if (match(parser, TOKEN_MULTIPLY)) {
            op = OP_MUL;
        } else {  // TOKEN_DIVIDE
            op = OP_DIV;
        }
        
        advance(parser);
        ASTNode* right = parse_expression(parser);
        
        expr = create_binary_op(op, expr, right);
    }
    
    return expr;
}

// 解析变量声明
static ASTNode* parse_variable_declaration(Parser* parser) {
    // 检查var或val关键字
    if (!match(parser, TOKEN_KEYWORD_VAR) && !match(parser, TOKEN_KEYWORD_VAL)) {
        return NULL;
    }
    bool is_mutable = parser->current_token.type == TOKEN_KEYWORD_VAR;
    advance(parser);

    // 获取变量名
    if (!match(parser, TOKEN_IDENTIFIER)) {
        parser->error_message = strdup("Expected variable name");
        return NULL;
    }
    char* name = strdup(parser->current_token.value);
    advance(parser);

    // 类型声明
    expect(parser, TOKEN_COLON);
    if (!match(parser, TOKEN_IDENTIFIER)) {
        parser->error_message = strdup("Expected type name");
        return NULL;
    }
    char* type = strdup(parser->current_token.value);
    advance(parser);

    // 初始化表达式
    ASTNode* initializer = NULL;
    if (match(parser, TOKEN_ASSIGN)) {
        advance(parser);
        initializer = parse_expression(parser);
    }

    expect(parser, TOKEN_SEMICOLON);
    
    return create_variable_decl(name, type, initializer, is_mutable);
}

ASTNode* get_ast_root(Parser* parser) {
    return parser->root;
}