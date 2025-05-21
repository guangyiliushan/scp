@echo off
REM 工具脚本：编译LLVM IR并执行
REM 使用方法: run_llvm.bat <LLVM IR文件>

if "%1" == "" (
    echo 错误: 请提供LLVM IR文件路径
    echo 用法: run_llvm.bat ^<LLVM IR文件^>
    exit /b 1
)

set LLVM_PATH=C:\Program Files\LLVM
set LLC="%LLVM_PATH%\bin\llc.exe"
set CLANG="%LLVM_PATH%\bin\clang.exe"

echo 编译LLVM IR为机器码...
%LLC% -filetype=obj "%~1" -o "%~dpn1.o"
if %ERRORLEVEL% NEQ 0 (
    echo 错误: LLVM编译失败
    exit /b 1
)

echo 链接生成可执行文件...
%CLANG% "%~dpn1.o" -o "%~dpn1.exe"
if %ERRORLEVEL% NEQ 0 (
    echo 错误: 链接失败
    exit /b 1
)

echo 运行生成的程序:
"%~dpn1.exe"

echo 清理临时文件...
del "%~dpn1.o"

echo 完成
