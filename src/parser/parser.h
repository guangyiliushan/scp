// Header file for SCP Language Parser

#ifndef PARSER_H
#define PARSER_H

#include "../common/ast.h"

// 保留简洁的公共接口，隐藏内部实现细节
typedef struct Parser Parser;

// 创建和销毁解析器
Parser* create_parser();
void destroy_parser(Parser* parser);

// 主解析函数
void parse_source(Parser* parser, const char* source);

// 结果获取函数
ASTNode* get_ast_root(Parser* parser);
const char* get_parser_error(Parser* parser);
int has_parser_error(Parser* parser);

#endif // PARSER_H