#ifndef CODEGEN_CONTEXT_H
#define CODEGEN_CONTEXT_H

#include "../common/ast.h"

// 代码生成上下文
typedef struct {
    char* output_buffer;       // 输出缓冲区
    int buffer_size;           // 缓冲区大小
    int buffer_position;       // 当前位置
    char* target_platform;     // 目标平台 
    int optimization_level;    // 优化级别
    void* llvm_module;         // LLVM模块(如果使用LLVM)
    void* symbols;             // 符号表
    char* error_message;       // 错误信息
} CodegenContext;

// 上下文操作函数
CodegenContext* create_codegen_context();
void destroy_codegen_context(CodegenContext* context);

// 缓冲区操作
void append_output(CodegenContext* context, const char* format, ...);
void ensure_buffer_capacity(CodegenContext* context, int required_size);

#endif // CODEGEN_CONTEXT_H
