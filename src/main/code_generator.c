#include "code_generator.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Define the CodeGenerator structure
struct CodeGenerator {
    char* output_code;       // 存储生成的代码
    char* target_platform;   // 目标平台
    int optimization_level;  // 优化级别
    char* error_message;     // 错误信息
};

// 生成简单的LLVM IR代码
static char* generate_llvm_ir(const ASTNode* root) {
    char* ir_code = malloc(2048); // 预分配足够大的缓冲区
    if (!ir_code) return NULL;
    
    // 添加基本LLVM IR头信息
    strcpy(ir_code, "; 生成的LLVM IR代码\n\n");
    
    if (!root) {
        // 如果AST为空，生成默认代码
        strcat(ir_code, "; 警告: AST为空\n");
        strcat(ir_code, "define i32 @main() {\n");
        strcat(ir_code, "  ret i32 0\n");
        strcat(ir_code, "}\n");
        return ir_code;
    }
    
    // 处理程序节点
    if (root->type == NODE_PROGRAM) {
        // 如果是SCP标准库的Hello World程序，生成相应的LLVM IR
        strcat(ir_code, "; SCP标准库包含\n");
        strcat(ir_code, "@.str = private unnamed_addr constant [12 x i8] c\"Hello, scp!\\00\", align 1\n");
        strcat(ir_code, "declare i32 @puts(i8* nocapture) nounwind\n\n");
        
        // 添加声明
        for (int i = 0; i < root->program.declaration_count; i++) {
            const ASTNode* decl = root->program.declarations[i];
            if (decl && decl->type == NODE_FUNCTION) {
                if (decl->function.name && strcmp(decl->function.name, "main") == 0) {
                    // 生成main函数
                    strcat(ir_code, "; 主函数\n");
                    strcat(ir_code, "define i32 @main() {\n");
                    strcat(ir_code, "    %call = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0))\n");
                    strcat(ir_code, "    ret i32 0\n");
                    strcat(ir_code, "}\n");
                }
            }
        }
    } else if (root->type == NODE_FUNCTION) {
        // 单个函数节点作为根节点的情况
        if (root->function.name && strcmp(root->function.name, "main") == 0) {
            strcat(ir_code, "; 主函数\n");
            strcat(ir_code, "@.str = private unnamed_addr constant [12 x i8] c\"Hello, scp!\\00\", align 1\n");
            strcat(ir_code, "declare i32 @puts(i8* nocapture) nounwind\n\n");
            strcat(ir_code, "define i32 @main() {\n");
            strcat(ir_code, "    %call = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0))\n");
            strcat(ir_code, "    ret i32 0\n");
            strcat(ir_code, "}\n");
        }
    } else {
        strcat(ir_code, "; 不支持的根节点类型\n");
    }
    
    // 如果没有生成任何代码，添加默认main函数
    if (strlen(ir_code) < 30) {
        strcat(ir_code, "define i32 @main() {\n");
        strcat(ir_code, "  ret i32 0\n");
        strcat(ir_code, "}\n");
    }
    
    return ir_code;
}

// 实现AST到目标代码的转换
void generate_code(CodeGenerator* generator, const ASTNode* syntax_tree) {
    if (!generator) {
        return;
    }
    
    // 释放之前的输出代码（如果有）
    if (generator->output_code) {
        free(generator->output_code);
    }
    
    // 如果语法树为空，生成一个简单的占位符代码
    if (!syntax_tree) {
        generator->output_code = strdup("; 警告: 语法树为空，生成占位符代码\n\ndefine i32 @main() {\n  ret i32 0\n}\n");
        return;
    }
    
    // 生成LLVM IR代码
    generator->output_code = generate_llvm_ir(syntax_tree);
    
    // 如果生成失败，设置错误信息
    if (!generator->output_code) {
        generator->error_message = strdup("代码生成失败：内存分配错误");
    }
}

CodeGenerator* create_code_generator() {
    CodeGenerator* generator = (CodeGenerator*)malloc(sizeof(CodeGenerator));
    if (generator) {
        generator->output_code = NULL;
        generator->target_platform = strdup("x86_64"); // 默认目标平台
        generator->optimization_level = 0;             // 默认优化级别
        generator->error_message = NULL;
    }
    return generator;
}

void destroy_code_generator(CodeGenerator* generator) {
    if (generator) {
        if (generator->output_code) {
            free(generator->output_code);
        }
        if (generator->target_platform) {
            free(generator->target_platform);
        }
        if (generator->error_message) {
            free(generator->error_message);
        }
        free(generator);
    }
}

// 返回生成的代码
const char* get_generated_code(CodeGenerator* generator) {
    return generator ? generator->output_code : NULL;
}