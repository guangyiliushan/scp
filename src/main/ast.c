#include "ast.h"
#include <stdlib.h>
#include <string.h>

// 创建AST节点
ASTNode* create_ast_node(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node) {
        node->type = type;
        node->line = 0;
        node->column = 0;
        
        // 根据节点类型初始化
        switch (type) {
            case NODE_PROGRAM:
                node->program.declarations = NULL;
                node->program.declaration_count = 0;
                break;
            case NODE_FUNCTION:
                node->function.name = NULL;
                node->function.parameters = NULL;
                node->function.param_count = 0;
                node->function.body = NULL;
                node->function.return_type = NULL;
                break;
            case NODE_FUNCTION_CALL:
                node->call.name = NULL;
                node->call.arguments = NULL;
                node->call.arg_count = 0;
                break;
            case NODE_VARIABLE_DECL:
                node->var_decl.name = NULL;
                node->var_decl.type = NULL;
                node->var_decl.initializer = NULL;
                break;
            case NODE_VARIABLE:
                node->variable.name = NULL;
                break;
            case NODE_LITERAL:
                // 字面量值将在创建后设置
                break;
            case NODE_BINARY_OP:
                node->binary_op.left = NULL;
                node->binary_op.right = NULL;
                break;
            case NODE_UNARY_OP:
                node->unary_op.operand = NULL;
                break;
            case NODE_IF_STATEMENT:
                node->if_stmt.condition = NULL;
                node->if_stmt.then_branch = NULL;
                node->if_stmt.else_branch = NULL;
                break;
            case NODE_WHILE_STATEMENT:
                node->while_stmt.condition = NULL;
                node->while_stmt.body = NULL;
                break;
            case NODE_RETURN:
                node->return_stmt.expression = NULL;
                break;
            case NODE_BLOCK:
                node->block.statements = NULL;
                node->block.statement_count = 0;
                break;
            case NODE_INCLUDE:
                node->include.filename = NULL;
                break;
        }
    }
    return node;
}

// 销毁AST节点
void destroy_ast_node(ASTNode* node) {
    if (!node) return;
    
    // 根据节点类型释放资源
    switch (node->type) {
        case NODE_PROGRAM:
            if (node->program.declarations) {
                for (int i = 0; i < node->program.declaration_count; i++) {
                    destroy_ast_node(node->program.declarations[i]);
                }
                free(node->program.declarations);
            }
            break;
        case NODE_FUNCTION:
            if (node->function.name) free(node->function.name);
            if (node->function.return_type) free(node->function.return_type);
            if (node->function.parameters) {
                for (int i = 0; i < node->function.param_count; i++) {
                    destroy_ast_node(node->function.parameters[i]);
                }
                free(node->function.parameters);
            }
            if (node->function.body) destroy_ast_node(node->function.body);
            break;
        case NODE_FUNCTION_CALL:
            if (node->call.name) free(node->call.name);
            if (node->call.arguments) {
                for (int i = 0; i < node->call.arg_count; i++) {
                    destroy_ast_node(node->call.arguments[i]);
                }
                free(node->call.arguments);
            }
            break;
        case NODE_VARIABLE_DECL:
            if (node->var_decl.name) free(node->var_decl.name);
            if (node->var_decl.type) free(node->var_decl.type);
            if (node->var_decl.initializer) destroy_ast_node(node->var_decl.initializer);
            break;
        case NODE_VARIABLE:
            if (node->variable.name) free(node->variable.name);
            break;
        case NODE_LITERAL:
            if (node->literal.type == LITERAL_STRING && node->literal.string_value) {
                free(node->literal.string_value);
            }
            break;
        case NODE_BINARY_OP:
            if (node->binary_op.left) destroy_ast_node(node->binary_op.left);
            if (node->binary_op.right) destroy_ast_node(node->binary_op.right);
            break;
        case NODE_UNARY_OP:
            if (node->unary_op.operand) destroy_ast_node(node->unary_op.operand);
            break;
        case NODE_IF_STATEMENT:
            if (node->if_stmt.condition) destroy_ast_node(node->if_stmt.condition);
            if (node->if_stmt.then_branch) destroy_ast_node(node->if_stmt.then_branch);
            if (node->if_stmt.else_branch) destroy_ast_node(node->if_stmt.else_branch);
            break;
        case NODE_WHILE_STATEMENT:
            if (node->while_stmt.condition) destroy_ast_node(node->while_stmt.condition);
            if (node->while_stmt.body) destroy_ast_node(node->while_stmt.body);
            break;
        case NODE_RETURN:
            if (node->return_stmt.expression) destroy_ast_node(node->return_stmt.expression);
            break;
        case NODE_BLOCK:
            if (node->block.statements) {
                for (int i = 0; i < node->block.statement_count; i++) {
                    destroy_ast_node(node->block.statements[i]);
                }
                free(node->block.statements);
            }
            break;
        case NODE_INCLUDE:
            if (node->include.filename) free(node->include.filename);
            break;
    }
    
    free(node);
}

// 创建整数字面量节点
ASTNode* create_int_literal(int value) {
    ASTNode* node = create_ast_node(NODE_LITERAL);
    if (node) {
        node->literal.type = LITERAL_INT;
        node->literal.int_value = value;
    }
    return node;
}

// 创建浮点数字面量节点
ASTNode* create_float_literal(float value) {
    ASTNode* node = create_ast_node(NODE_LITERAL);
    if (node) {
        node->literal.type = LITERAL_FLOAT;
        node->literal.float_value = value;
    }
    return node;
}

// 创建字符串字面量节点
ASTNode* create_string_literal(const char* value) {
    ASTNode* node = create_ast_node(NODE_LITERAL);
    if (node) {
        node->literal.type = LITERAL_STRING;
        node->literal.string_value = strdup(value);
    }
    return node;
}

// 创建布尔字面量节点
ASTNode* create_bool_literal(int value) {
    ASTNode* node = create_ast_node(NODE_LITERAL);
    if (node) {
        node->literal.type = LITERAL_BOOL;
        node->literal.bool_value = value;
    }
    return node;
}

// 创建二元运算节点
ASTNode* create_binary_op(OperatorType op, ASTNode* left, ASTNode* right) {
    ASTNode* node = create_ast_node(NODE_BINARY_OP);
    if (node) {
        node->binary_op.op = op;
        node->binary_op.left = left;
        node->binary_op.right = right;
    }
    return node;
}

// 创建一元运算节点
ASTNode* create_unary_op(OperatorType op, ASTNode* operand) {
    ASTNode* node = create_ast_node(NODE_UNARY_OP);
    if (node) {
        node->unary_op.op = op;
        node->unary_op.operand = operand;
    }
    return node;
}

// 创建变量引用节点
ASTNode* create_variable(const char* name) {
    ASTNode* node = create_ast_node(NODE_VARIABLE);
    if (node) {
        node->variable.name = strdup(name);
    }
    return node;
}

// 创建变量声明节点
ASTNode* create_variable_decl(const char* name, const char* type, ASTNode* initializer) {
    ASTNode* node = create_ast_node(NODE_VARIABLE_DECL);
    if (node) {
        node->var_decl.name = strdup(name);
        node->var_decl.type = type ? strdup(type) : NULL;
        node->var_decl.initializer = initializer;
    }
    return node;
}

// 创建函数调用节点
ASTNode* create_function_call(const char* name, ASTNode** arguments, int arg_count) {
    ASTNode* node = create_ast_node(NODE_FUNCTION_CALL);
    if (node) {
        node->call.name = strdup(name);
        node->call.arg_count = arg_count;
        
        if (arg_count > 0) {
            node->call.arguments = (ASTNode**)malloc(sizeof(ASTNode*) * arg_count);
            if (node->call.arguments) {
                for (int i = 0; i < arg_count; i++) {
                    node->call.arguments[i] = arguments[i];
                }
            } else {
                node->call.arg_count = 0;
            }
        } else {
            node->call.arguments = NULL;
        }
    }
    return node;
}

// 创建函数定义节点
ASTNode* create_function(const char* name, ASTNode** parameters, int param_count, 
                        const char* return_type, ASTNode* body) {
    ASTNode* node = create_ast_node(NODE_FUNCTION);
    if (node) {
        node->function.name = strdup(name);
        node->function.param_count = param_count;
        node->function.return_type = return_type ? strdup(return_type) : NULL;
        node->function.body = body;
        
        if (param_count > 0) {
            node->function.parameters = (ASTNode**)malloc(sizeof(ASTNode*) * param_count);
            if (node->function.parameters) {
                for (int i = 0; i < param_count; i++) {
                    node->function.parameters[i] = parameters[i];
                }
            } else {
                node->function.param_count = 0;
            }
        } else {
            node->function.parameters = NULL;
        }
    }
    return node;
}

// 创建代码块节点
ASTNode* create_block(ASTNode** statements, int statement_count) {
    ASTNode* node = create_ast_node(NODE_BLOCK);
    if (node) {
        node->block.statement_count = statement_count;
        
        if (statement_count > 0) {
            node->block.statements = (ASTNode**)malloc(sizeof(ASTNode*) * statement_count);
            if (node->block.statements) {
                for (int i = 0; i < statement_count; i++) {
                    node->block.statements[i] = statements[i];
                }
            } else {
                node->block.statement_count = 0;
            }
        } else {
            node->block.statements = NULL;
        }
    }
    return node;
}

// 创建if语句节点
ASTNode* create_if_statement(ASTNode* condition, ASTNode* then_branch, ASTNode* else_branch) {
    ASTNode* node = create_ast_node(NODE_IF_STATEMENT);
    if (node) {
        node->if_stmt.condition = condition;
        node->if_stmt.then_branch = then_branch;
        node->if_stmt.else_branch = else_branch;
    }
    return node;
}

// 创建while语句节点
ASTNode* create_while_statement(ASTNode* condition, ASTNode* body) {
    ASTNode* node = create_ast_node(NODE_WHILE_STATEMENT);
    if (node) {
        node->while_stmt.condition = condition;
        node->while_stmt.body = body;
    }
    return node;
}

// 创建return语句节点
ASTNode* create_return(ASTNode* expression) {
    ASTNode* node = create_ast_node(NODE_RETURN);
    if (node) {
        node->return_stmt.expression = expression;
    }
    return node;
}

// 创建include指令节点
ASTNode* create_include(const char* filename) {
    ASTNode* node = create_ast_node(NODE_INCLUDE);
    if (node) {
        node->include.filename = strdup(filename);
    }
    return node;
}

// 创建程序节点
ASTNode* create_program(ASTNode** declarations, int declaration_count) {
    ASTNode* node = create_ast_node(NODE_PROGRAM);
    if (node) {
        node->program.declaration_count = declaration_count;
        
        if (declaration_count > 0) {
            node->program.declarations = (ASTNode**)malloc(sizeof(ASTNode*) * declaration_count);
            if (node->program.declarations) {
                for (int i = 0; i < declaration_count; i++) {
                    node->program.declarations[i] = declarations[i];
                }
            } else {
                node->program.declaration_count = 0;
            }
        } else {
            node->program.declarations = NULL;
        }
    }
    return node;
}