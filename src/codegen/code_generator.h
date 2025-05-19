// Header file for SCP Language Code Generator

#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "ast.h"

// Define the CodeGenerator structure
typedef struct CodeGenerator CodeGenerator;

// 代码生成器结构在实现文件中定义

// Function prototypes
CodeGenerator* create_code_generator();
void generate_code(CodeGenerator* generator, const ASTNode* ast_root);
const char* get_generated_code(CodeGenerator* generator);
void destroy_code_generator(CodeGenerator* generator);

#endif // CODE_GENERATOR_H