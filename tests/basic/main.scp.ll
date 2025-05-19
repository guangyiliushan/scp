; 生成的LLVM IR代码

; SCP标准库包含
@.str = private unnamed_addr constant [12 x i8] c"Hello, scp!\00", align 1
declare i32 @puts(i8* nocapture) nounwind

; 主函数
define i32 @main() {
    %call = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i64 0, i64 0))
    ret i32 0
}
