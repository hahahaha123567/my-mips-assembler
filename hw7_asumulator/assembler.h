#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "mips.h"

unsigned int instructionDrive(FILE * fp, char * name);
unsigned int R_Instruction(FILE * fp, int id);
unsigned int I_Instruction(FILE * fp, int id);
unsigned int J_Instruction(FILE * fp, int id);
unsigned int C_Instruction(FILE * fp, int id);
int regName2Num(char * name);