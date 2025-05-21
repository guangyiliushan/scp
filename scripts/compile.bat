@echo off
setlocal

REM 编译SCP项目并运行示例代码
REM 用法: compile.bat [example_name]

set EXAMPLE=basic
if not "%~1"=="" set EXAMPLE=%~1

REM 构建编译器
echo 构建SCP编译器...
mingw32-make -j4

REM 编译示例程序到LLVM IR
echo 编译%EXAMPLE%示例到LLVM IR...
bin\scp examples\%EXAMPLE%\main.scp examples\%EXAMPLE%\main.scp.ll

REM 运行LLVM IR (如果tools/run_llvm.bat存在)
if exist tools\run_llvm.bat (
  echo 运行LLVM IR...
  call tools\run_llvm.bat examples\%EXAMPLE%\main.scp.ll
)

echo 完成！
endlocal
