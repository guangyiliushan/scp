#ifndef DECL_PARSER_H
#define DECL_PARSER_H

#include "../../common/ast.h"
#include "../parser_context.h"

// 声明解析相关函数
ASTNode* parse_function_definition(ParserContext* context);
ASTNode* parse_variable_declaration(ParserContext* context);
ASTNode* parse_include_directive(ParserContext* context);

#endif // DECL_PARSER_H
