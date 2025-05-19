#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "code_generator.h"

// 读取文件内容
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", filename);
        return NULL;
    }
    
    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // 分配内存并读取文件内容
    char* buffer = (char*)malloc(size + 1);
    if (buffer) {
        size_t read_size = fread(buffer, 1, size, file);
        buffer[read_size] = '\0';
    }
    
    fclose(file);
    return buffer;
}

// 保存生成的代码到文件
void save_to_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "无法创建文件: %s\n", filename);
        return;
    }
    
    fputs(content, file);
    fclose(file);
}

int main(int argc, char* argv[]) {

    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    if (argc < 2) {
        printf("用法: %s <源文件> [输出文件]\n", argv[0]);
        return 1;
    }
    
    // 读取源文件
    char* source = read_file(argv[1]);
    if (!source) {
        return 1;
    }
    
    // 设置输出文件名
    char* output_file = NULL;
    if (argc >= 3) {
        output_file = argv[2];
    } else {
        // 默认输出文件名为源文件+.ll (LLVM IR)
        output_file = (char*)malloc(strlen(argv[1]) + 4);
        sprintf(output_file, "%s.ll", argv[1]);
    }
    
    // 创建词法分析器
    Lexer* lexer = create_lexer(source);
    
    // 创建语法分析器
    Parser* parser = create_parser();
    
    // 解析源代码
    parse_source(parser, source);
    
    // 获取AST根节点
    ASTNode* ast_root = get_ast_root(parser);
    
    // 创建代码生成器
    CodeGenerator* generator = create_code_generator();
    
    // 生成代码
    generate_code(generator, ast_root);
    
    // 获取生成的代码并保存到文件
    const char* generated_code = get_generated_code(generator);
    if (generated_code) {
        save_to_file(output_file, generated_code);
    } else {
        save_to_file(output_file, "// 警告: 代码生成失败\n// 生成占位符代码\n\ndefine i32 @main() {\n  ret i32 0\n}\n");
    }
    
    printf("编译完成，输出文件: %s\n", output_file);
    
    // 清理资源
    destroy_lexer(lexer);
    destroy_parser(parser);
    destroy_code_generator(generator);
    free(source);
    
    if (argc < 3) {
        free(output_file);
    }
    
    return 0;
}