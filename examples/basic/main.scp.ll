; 生成的LLVM IR代码

; SCP标准库包含
@.str = private unnamed_addr constant [12 x i8] c"Hello, scp!\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; 声明外部函数
declare i32 @printf(i8* nocapture, ...) nounwind

; 主函数
define i32 @main() {
    %_1 = load i32, i32* %i, align 4
    %_2 = add nsw i32 %_1, 1
    store i32 %_2, i32* %i, align 4
    ret i32 0
}
