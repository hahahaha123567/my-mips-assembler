# MIPS simulator

## Need to do
1. label system
2. support more instructions

## Instructions supported
R����(ADD��SUB��SLT)��LW��SW��BEQ��J

## ����
debug.exe
����a���س�����ʼ�༭������
����һ������q���س���������ѡ�����

## File

###### debug.c
��Ŀ¼������ѡ�����

###### mips.c, mips.h
MIPS ��ع�������

###### assembler.c, assembler.h
MIPS assembler: convert mips instructions to machine code

###### disassembler.c, disassembler.h
MIPS disassembler: convert machine code to mips instructions

###### run.c, run.h
ģ��ʵ�ֻ����룬ʵ�ֵ������Ե����й���