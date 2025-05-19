#ifndef SCP_STDIO_H
#define SCP_STDIO_H

// 声明标准库函数
void print(const char* message);
void println(const char* message);
void read(char* buffer, int size);
char* readline();

#endif // SCP_STDIO_H