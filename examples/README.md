# Examples

This directory contains example code for the SCP language project.

## 运行和编译LLVM IR示例

以`basic/main.scp.ll`为例，假设你已安装LLVM工具链（如`lli`和`clang`）：

### 1. 直接运行LLVM IR

```bash
lli basic/main.scp.ll
```

## 运行和编译LLVM IR示例

以`basic/main.scp.ll`为例，假
设你已安装LLVM工具链（如`lli`和
`clang`）：

### 1. 直接运行LLVM IR

```bash
lli basic/main.scp.ll
```
### 2. 编译为本地可执行文件
```
clang basic/main.scp.ll -o 
main.exe
./main.exe
```
### 3. Windows下运行
```
clang basic/main.scp.ll -o 
main.exe
main.exe
```
注：如需包含SCP标准库IR文件，请在clang命令中追加对应 .ll 文件。

如需将SCP源文件（.scp）编译为LLVM IR，请先使用SCP编译器：

```
scp.exe examples/basic/main.
scp
```
生成的 .ll 文件可按上述方法运行或编译。