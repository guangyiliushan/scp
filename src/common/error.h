#ifndef ERROR_H 
#define ERROR_H

typedef enum {
    ERROR_NONE,             // 无错误
    ERROR_LEXICAL,          // 词法错误
    ERROR_SYNTAX,           // 语法错误
    ERROR_SEMANTIC,         // 语义错误
    ERROR_CODEGEN,          // 代码生成错误
    ERROR_IO,               // 输入输出错误
    ERROR_INTERNAL          // 内部错误
} ErrorType;

typedef struct {
    ErrorType type;         // 错误类型
    char* message;          // 错误消息
    const char* filename;   // 文件名
    int line;               // 行号
    int column;             // 列号
    char* source_line;      // 源代码行
} Error;

// 错误处理函数
Error* create_error(ErrorType type, const char* message, const char* filename, int line, int column);
void destroy_error(Error* error);
const char* error_type_to_string(ErrorType type);
void print_error(Error* error);

#endif // ERROR_H
