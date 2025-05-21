#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include "../../common/ast.h"
#include "../parser_context.h"

// 表达式解析相关函数
ASTNode* parse_expression(ParserContext* context);
ASTNode* parse_primary_expr(ParserContext* context);
ASTNode* parse_binary_expr(ParserContext* context);

#endif // EXPR_PARSER_H
