// 抽象语法树头文件

#ifndef AST_H
#define AST_H

#include <stdlib.h>

// AST节点类型枚举
typedef enum {
    NODE_PROGRAM,        // 程序
    NODE_FUNCTION,       // 函数定义
    NODE_FUNCTION_CALL,  // 函数调用
    NODE_VARIABLE_DECL,  // 变量声明
    NODE_VARIABLE,       // 变量引用
    NODE_LITERAL,        // 字面量
    NODE_BINARY_OP,      // 二元运算
    NODE_UNARY_OP,       // 一元运算
    NODE_IF_STATEMENT,   // if语句
    NODE_WHILE_STATEMENT,// while语句
    NODE_RETURN,         // return语句
    NODE_BLOCK,          // 代码块
    NODE_INCLUDE         // include指令
} ASTNodeType;

// 字面量类型枚举
typedef enum {
    LITERAL_INT,         // 整数
    LITERAL_FLOAT,       // 浮点数
    LITERAL_STRING,      // 字符串
    LITERAL_BOOL         // 布尔值
} LiteralType;

// 运算符类型枚举
typedef enum {
    OP_ADD,              // +
    OP_SUB,              // -
    OP_MUL,              // *
    OP_DIV,              // /
    OP_EQ,               // ==
    OP_NEQ,              // !=
    OP_LT,               // <
    OP_LTE,              // <=
    OP_GT,               // >
    OP_GTE,              // >=
    OP_NOT,              // !
    OP_ASSIGN            // =
} OperatorType;

// 前向声明
typedef struct ASTNode ASTNode;

// 字面量结构
typedef struct {
    LiteralType type;    // 字面量类型
    union {
        int int_value;       // 整数值
        float float_value;   // 浮点数值
        char* string_value;  // 字符串值
        int bool_value;      // 布尔值
    };
} Literal;

// 函数定义结构
typedef struct {
    char* name;              // 函数名
    ASTNode** parameters;    // 参数列表
    int param_count;         // 参数数量
    ASTNode* body;           // 函数体
    char* return_type;       // 返回类型
} FunctionNode;

// 函数调用结构
typedef struct {
    char* name;              // 函数名
    ASTNode** arguments;     // 参数列表
    int arg_count;           // 参数数量
} FunctionCallNode;

// 变量声明结构
typedef struct {
    char* name;              // 变量名
    char* type;              // 变量类型
    ASTNode* initializer;    // 初始化表达式
} VariableDeclNode;

// 变量引用结构
typedef struct {
    char* name;              // 变量名
} VariableNode;

// 二元运算结构
typedef struct {
    OperatorType op;         // 运算符
    ASTNode* left;           // 左操作数
    ASTNode* right;          // 右操作数
} BinaryOpNode;

// 一元运算结构
typedef struct {
    OperatorType op;         // 运算符
    ASTNode* operand;        // 操作数
} UnaryOpNode;

// if语句结构
typedef struct {
    ASTNode* condition;      // 条件表达式
    ASTNode* then_branch;    // then分支
    ASTNode* else_branch;    // else分支
} IfStatementNode;

// while语句结构
typedef struct {
    ASTNode* condition;      // 条件表达式
    ASTNode* body;           // 循环体
} WhileStatementNode;

// return语句结构
typedef struct {
    ASTNode* expression;     // 返回表达式
} ReturnNode;

// 代码块结构
typedef struct {
    ASTNode** statements;    // 语句列表
    int statement_count;     // 语句数量
} BlockNode;

// include指令结构
typedef struct {
    char* filename;          // 文件名
} IncludeNode;

// 程序结构
typedef struct {
    ASTNode** declarations;  // 声明列表
    int declaration_count;   // 声明数量
} ProgramNode;

// AST节点结构
struct ASTNode {
    ASTNodeType type;        // 节点类型
    int line;                // 行号
    int column;              // 列号
    char* name;              // 函数名
    ASTNode** children;      // 子节点数组
    int children_count;      // 子节点数量
    union {
        Literal literal;             // 字面量
        FunctionNode function;       // 函数定义
        FunctionCallNode call;       // 函数调用
        VariableDeclNode var_decl;   // 变量声明
        VariableNode variable;       // 变量引用
        BinaryOpNode binary_op;      // 二元运算
        UnaryOpNode unary_op;        // 一元运算
        IfStatementNode if_stmt;     // if语句
        WhileStatementNode while_stmt;// while语句
        ReturnNode return_stmt;      // return语句
        BlockNode block;             // 代码块
        IncludeNode include;         // include指令
        ProgramNode program;         // 程序
    };
};

// 函数原型
ASTNode* create_ast_node(ASTNodeType type);
void destroy_ast_node(ASTNode* node);

#endif // AST_H