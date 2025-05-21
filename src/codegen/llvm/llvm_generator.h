#ifndef LLVM_GENERATOR_H
#define LLVM_GENERATOR_H

#include "../codegen_context.h"
#include "../../common/ast.h"

// LLVM IR生成函数
void generate_llvm_module(CodegenContext* context, const ASTNode* root);
void generate_llvm_function(CodegenContext* context, const ASTNode* node);
void generate_llvm_expr(CodegenContext* context, const ASTNode* node);
void generate_llvm_stmt(CodegenContext* context, const ASTNode* node);
void generate_llvm_decl(CodegenContext* context, const ASTNode* node);

#endif // LLVM_GENERATOR_H
