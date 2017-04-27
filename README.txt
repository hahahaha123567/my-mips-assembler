Homework 7
运行debug.exe，显示可用指令
输入方式：输入a并回车，开始编辑汇编代码，在新一行输入q并回车返回命令选择界面
支持的指令：R类型(ADD、SUB、SLT)、LW、SW、BEQ、J
注意：标号系统尚未实现，beq/j指令的label参数应输入对应的offset立即数
输入q退出

代码：
mips.c/mips.h为mips指令公共数据
assembler.c/assembler.h修改自homework5
disassembler.c/assembler.h修改自homework6
run.c/run.h实现单步调试的运行功能
debug.c主程序