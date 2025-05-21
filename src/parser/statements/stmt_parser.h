#ifndef STMT_PARSER_H
#define STMT_PARSER_H

#include "../../common/ast.h"
#include "../parser_context.h"

// 语句解析相关函数
ASTNode* parse_statement(ParserContext* context);
ASTNode* parse_if_statement(ParserContext* context);
ASTNode* parse_return_statement(ParserContext* context);
ASTNode* parse_block_statement(ParserContext* context);

#endif // STMT_PARSER_H
