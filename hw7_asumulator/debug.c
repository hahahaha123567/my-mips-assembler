#include "mips.h"
#include "assembler.h"
#include "disassembler.h"
#include "run.h"
#include <stdio.h>
#include <stdlib.h>


void showReg();
void showData();
void showAsm();
void edit();
void assembler();
void debug();

int main()
{
	char order;
	// every statement has 50 char
	for (int i = 0; i < 100; i++) {
		asm[i] = malloc(sizeof(char) * 50);
	}
	reg[0] = 0;
	cursor = 0;
	count = 0;
	
	printf("R: show registers\nD: show memory by data\nU: show memory by instruction\nA: edit code\nT: single step debugging\n");
	while ((order = getchar()) != 'q' && order != 'Q') {
		if (order == '\n') {
			continue;
		}
		getchar();
		switch (order) {
		case 'r':
		case 'R': showReg();
			break;
		case 'd':
		case 'D': showData();
			break;
		case 'u':
		case 'U': showAsm();
			break;
		case 'a':
		case 'A': edit();
			assembler();
			break;
		case 't':
		case 'T': debug();
			break;
		default: printf("Invalid order, please input again:\n");
		}
		printf("R: show registers\nD: show memory by data\nU: show memory by instruction\nA: edit code\nT: single step debugging\n");
	}

	return 0;
}

void showReg()
{
	for (int i = 0; i < RegisterNum; ++i) {
		printf("%s:\t%08X   ", regName[i], reg[i]);
		if (i % 4 == 3) {
			printf("\n");
		}
	}
}

void showData()
{
	unsigned int index = 0;
	printf("address data\n");
	for (int i = 0; i < count; i++) {
		printf("%04X:%04X  %08X\n", index>>16, index&0x0000FFFF, code[i]);
		index += 4;
	}
}

void showAsm()
{
	unsigned int index = 0;
	printf("address data\n");
	for (int i = 0; i < count; i++) {
		printf("%08X  %s\n", index, asm[i]);
		index += 4;
	}
}

void edit()
{
	// read to memory
	gets(asm[cursor]);
	while (asm[cursor][0] != 'q' && asm[cursor][0] != 'Q') {
		cursor++;
		gets(asm[cursor]);
	}
	// memory to disk
	FILE * fp = fopen("input.asm", "wb");
	count = 0;
	for (int j = 0; j < cursor; j++) {
		count++;
		fprintf(fp, "%s\n", asm[j]);
	}
	fclose(fp);
}

void assembler()
{
	int i = 0;
	char str[20] = { 0 };
	FILE * fp = fopen("input.asm", "r");
	
	while (feof(fp) == 0) {
		fscanf(fp, "%s", str);
		if (*str == '\n' || *str == '\0') {
			break;
		}
		code[i] = instructionDrive(fp, str);
		for (int i = 0; i < 20; ++i) {
			str[i] = 0;
		}
		i++;
		count++;
	}
	fclose(fp);
}

void debug()
{
	run(code[pc]);
	pc += 4;
	showReg();
}
