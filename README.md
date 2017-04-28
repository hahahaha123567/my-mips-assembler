# MIPS simulator

## Need to do
1. label system
2. support more instructions

## Instructions supported
R类型(ADD、SUB、SLT)、LW、SW、BEQ、J

## 运行
debug.exe
输入a并回车，开始编辑汇编代码
在新一行输入q并回车返回命令选择界面

## File

###### debug.c
主目录，功能选择界面

###### mips.c, mips.h
MIPS 相关公共数据

###### assembler.c, assembler.h
MIPS assembler: convert mips instructions to machine code

###### disassembler.c, disassembler.h
MIPS disassembler: convert machine code to mips instructions

###### run.c, run.h
模拟实现机器码，实现单步调试的运行功能