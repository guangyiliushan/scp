#include "code_generator.h"
#include "../common/ast.h"
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

// 类型映射辅助函数
static void get_llvm_type_and_align(const char* type_name, const char** llvm_type, int* align) {
    if (strcmp(type_name, "str") == 0) {
        *llvm_type = "i8*";
        *align = 8;
    } else if (strcmp(type_name, "int") == 0 || strcmp(type_name, "i32") == 0) {
        *llvm_type = "i32";
        *align = 4;
    } else if (strcmp(type_name, "bool") == 0) {
        *llvm_type = "i1";
        *align = 1;
    } else if (strcmp(type_name, "i8") == 0) {
        *llvm_type = "i8";
        *align = 1;
    } else if (strcmp(type_name, "i16") == 0) {
        *llvm_type = "i16";
        *align = 2;
    } else if (strcmp(type_name, "i64") == 0) {
        *llvm_type = "i64";
        *align = 8;
    } else if (strcmp(type_name, "f32") == 0) {
        *llvm_type = "float";
        *align = 4;
    } else if (strcmp(type_name, "f64") == 0) {
        *llvm_type = "double";
        *align = 8;
    } else {
        *llvm_type = "i32";
        *align = 4;
    }
}

// 生成变量声明的LLVM IR代码
static void generate_variable_decl_ir(char* ir_code, const ASTNode* node) {
    const char* type_ir;
    int align;
    get_llvm_type_and_align(node->var_decl.type, &type_ir, &align);

    // 只分配空间，不赋初值
    sprintf(ir_code + strlen(ir_code), 
        "    %%%s = alloca %s, align %d\n",
        node->var_decl.name,
        type_ir,
        align);
}

// 生成变量初始化赋值的LLVM IR代码
static void generate_variable_init_ir(char* ir_code, const ASTNode* node) {
    if (!node || node->type != NODE_VARIABLE_DECL) return;
    const char* type_ir;
    int align;
    get_llvm_type_and_align(node->var_decl.type, &type_ir, &align);

    if (node->var_decl.initializer) {
        if (node->var_decl.initializer->type == NODE_LITERAL) {
            if (node->var_decl.initializer->literal.type == LITERAL_STRING) {
                // 处理字符串字面量
                sprintf(ir_code + strlen(ir_code),
                    "    store i8* getelementptr inbounds ([%d x i8], [%d x i8]* @.str, i32 0, i32 0), i8** %%%s, align 8\n",
                    (int)strlen(node->var_decl.initializer->literal.string_value) + 1,
                    (int)strlen(node->var_decl.initializer->literal.string_value) + 1,
                    node->var_decl.name);
            } else if (node->var_decl.initializer->literal.type == LITERAL_INT) {
                // 处理整数字面量
                if (strcmp(type_ir, "i1") == 0) {
                    // bool类型
                    int bool_val = node->var_decl.initializer->literal.int_value ? 1 : 0;
                    sprintf(ir_code + strlen(ir_code),
                        "    store i1 %d, i1* %%%s, align %d\n",
                        bool_val, node->var_decl.name, align);
                } else {
                    sprintf(ir_code + strlen(ir_code),
                        "    store %s %d, %s* %%%s, align %d\n",
                        type_ir,
                        node->var_decl.initializer->literal.int_value,
                        type_ir,
                        node->var_decl.name,
                        align);
                }
            } else if (node->var_decl.initializer->literal.type == LITERAL_FLOAT) {
                // 浮点类型
                if (strcmp(type_ir, "float") == 0) {
                    sprintf(ir_code + strlen(ir_code),
                        "    store float %f, float* %%%s, align %d\n",
                        node->var_decl.initializer->literal.float_value,
                        node->var_decl.name,
                        align);
                } else if (strcmp(type_ir, "double") == 0) {
                    sprintf(ir_code + strlen(ir_code),
                        "    store double %lf, double* %%%s, align %d\n",
                        node->var_decl.initializer->literal.float_value,
                        node->var_decl.name,
                        align);
                }
            }
        }
    }
}

// 生成函数调用的LLVM IR代码
static void generate_function_call_ir(char* ir_code, const ASTNode* node, int* temp_var_count) {
    if (!node || node->type != NODE_FUNCTION_CALL) return;
    
    // 处理println函数调用
    if (strcmp(node->call.name, "println") == 0) {
        if (node->call.arg_count > 0) {
            ASTNode* arg = node->call.arguments[0];
            
            if (arg->type == NODE_VARIABLE) {
                // 加载变量值
                int var_id = (*temp_var_count)++;
                
                if (arg->variable.name) {
                    // 根据变量类型生成不同代码
                    if (strstr(arg->variable.name, "message") || strstr(arg->variable.name, "str")) {
                        // 字符串变量
                        sprintf(ir_code + strlen(ir_code),
                            "    %%_%d = load i8*, i8** %%%s, align 8\n"
                            "    %%call_%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), i8* %%_%d)\n",
                            var_id, arg->variable.name, var_id, var_id);
                    } else {
                        // 整数变量
                        sprintf(ir_code + strlen(ir_code),
                            "    %%_%d = load i32, i32* %%%s, align 4\n"
                            "    %%call_%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), i32 %%_%d)\n",
                            var_id, arg->variable.name, var_id, var_id);
                    }
                }
            } else if (arg->type == NODE_LITERAL) {
                if (arg->literal.type == LITERAL_STRING) {
                    // 直接字符串字面量
                    sprintf(ir_code + strlen(ir_code),
                        "    %%call_%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i32 0, i32 0), "
                        "i8* getelementptr inbounds ([%d x i8], [%d x i8]* @.str, i32 0, i32 0))\n",
                        (*temp_var_count)++, (int)strlen(arg->literal.string_value) + 1, (int)strlen(arg->literal.string_value) + 1);
                } else if (arg->literal.type == LITERAL_INT) {
                    // 整数字面量
                    sprintf(ir_code + strlen(ir_code),
                        "    %%call_%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i32 0, i32 0), i32 %d)\n",
                        (*temp_var_count)++, arg->literal.int_value);
                }
            }
        }
    }
}

// 生成二元操作的LLVM IR代码
static void generate_binary_op_ir(char* ir_code, const ASTNode* node, int* temp_var_count) {
    if (!node || node->type != NODE_BINARY_OP) return;
    
    // 处理赋值操作
    if (node->binary_op.op == OP_ASSIGN) {
        if (node->binary_op.left->type == NODE_VARIABLE && 
            node->binary_op.right->type == NODE_BINARY_OP &&
            node->binary_op.right->binary_op.op == OP_ADD) {
            
            // 加法再赋值的情况 (i = i + 1)
            ASTNode* left_var = node->binary_op.left;
            ASTNode* add_op = node->binary_op.right;
            
            if (add_op->binary_op.left->type == NODE_VARIABLE && 
                add_op->binary_op.right->type == NODE_LITERAL &&
                add_op->binary_op.right->literal.type == LITERAL_INT) {
                
                // 加载左操作数
                int var_id = (*temp_var_count)++;
                sprintf(ir_code + strlen(ir_code),
                    "    %%_%d = load i32, i32* %%%s, align 4\n",
                    var_id, left_var->variable.name);
                
                // 执行加法
                int result_id = (*temp_var_count)++;
                sprintf(ir_code + strlen(ir_code),
                    "    %%_%d = add nsw i32 %%_%d, %d\n",
                    result_id, var_id, add_op->binary_op.right->literal.int_value);
                
                // 存储结果
                sprintf(ir_code + strlen(ir_code),
                    "    store i32 %%_%d, i32* %%%s, align 4\n",
                    result_id, left_var->variable.name);
            }
        }
    }
}

// 生成LLVM IR代码
static char* generate_llvm_ir(const ASTNode* root) {
    char* ir_code = malloc(8192); // 预分配更大的缓冲区
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
    
    // 添加标准库字符串常量和函数声明
    strcat(ir_code, "; SCP标准库包含\n");
    strcat(ir_code, "@.str = private unnamed_addr constant [12 x i8] c\"Hello, scp!\\00\", align 1\n");
    strcat(ir_code, "@.str.1 = private unnamed_addr constant [4 x i8] c\"%s\\0A\\00\", align 1\n");
    strcat(ir_code, "@.str.2 = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\", align 1\n\n");
    
    // 声明外部函数
    strcat(ir_code, "; 声明外部函数\n");
    strcat(ir_code, "declare i32 @printf(i8* nocapture, ...) nounwind\n\n");
    
    // 处理程序节点
    if (root->type == NODE_PROGRAM) {
        // 查找main函数
        for (int i = 0; i < root->program.declaration_count; i++) {
            const ASTNode* decl = root->program.declarations[i];
            if (decl && decl->type == NODE_FUNCTION && 
                decl->function.name && strcmp(decl->function.name, "main") == 0) {

                // 生成main函数
                strcat(ir_code, "; 主函数\n");
                strcat(ir_code, "define i32 @main() {\n");

                // 处理函数体
                if (decl->function.body && decl->function.body->type == NODE_BLOCK) {
                    int temp_var_count = 1;

                    // 1. 先分配所有变量空间
                    for (int j = 0; j < decl->function.body->block.statement_count; j++) {
                        ASTNode* stmt = decl->function.body->block.statements[j];
                        if (stmt && stmt->type == NODE_VARIABLE_DECL) {
                            generate_variable_decl_ir(ir_code, stmt);
                        }
                    }
                    // 2. 再做初始化赋值
                    for (int j = 0; j < decl->function.body->block.statement_count; j++) {
                        ASTNode* stmt = decl->function.body->block.statements[j];
                        if (stmt && stmt->type == NODE_VARIABLE_DECL) {
                            generate_variable_init_ir(ir_code, stmt);
                        }
                    }
                    // 3. 处理其他语句
                    for (int j = 0; j < decl->function.body->block.statement_count; j++) {
                        ASTNode* stmt = decl->function.body->block.statements[j];
                        if (stmt && stmt->type == NODE_FUNCTION_CALL) {
                            generate_function_call_ir(ir_code, stmt, &temp_var_count);
                        } else if (stmt && stmt->type == NODE_BINARY_OP) {
                            generate_binary_op_ir(ir_code, stmt, &temp_var_count);
                        } else if (stmt && stmt->type == NODE_RETURN) {
                            // 处理return语句
                            if (stmt->return_stmt.expression && 
                                stmt->return_stmt.expression->type == NODE_LITERAL &&
                                stmt->return_stmt.expression->literal.type == LITERAL_INT) {
                                sprintf(ir_code + strlen(ir_code), 
                                    "    ret i32 %d\n", 
                                    stmt->return_stmt.expression->literal.int_value);
                            } else {
                                strcat(ir_code, "    ret i32 0\n");
                            }
                        }
                    }
                }
                // 如果没有明确的return语句，添加默认return
                if (!strstr(ir_code, "ret i32")) {
                    strcat(ir_code, "    ret i32 0\n");
                }
                
                strcat(ir_code, "}\n");
            }
        }
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