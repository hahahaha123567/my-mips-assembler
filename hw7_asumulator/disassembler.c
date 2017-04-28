#include "mips.h"
#include <stdio.h>
#include <stdlib.h>

void Rtype(FILE *, unsigned int ins);
void Itype(FILE *, unsigned int ins);
void Jtype(FILE *, unsigned int ins);
void Ctype(FILE *, unsigned int ins);

void Rtype(FILE * fWrite, unsigned int ins)
{
	unsigned int rs = (ins >> 21) & 0x1F;
	unsigned int rt = (ins >> 16) & 0x1F;
	unsigned int rd = (ins >> 11) & 0x1F;
	unsigned int sa = (ins >> 6) & 0x1F;
	unsigned int func = ins & 0x3F;

	// print instruction name
	for (int i = 0; i < RInstructionNum; ++i) {
		if (func == R_Func[i]) {
			fprintf(fWrite, "%s ", R_name[i]);
		}
	}
	// print arguments
	switch (func) {
	// print rd, rs, rt
	case 0x20:
	case 0x21:
	case 0x24:
	case 0x27:
	case 0x25:
	case 0x2A:
	case 0x2B:
	case 0x22:
	case 0x23: fprintf(fWrite, "%s %s %s\n", regName[rd], regName[rs], regName[rt]);
		break;
	// print rs, rt
	case 0x1A:
	case 0x1B:
	case 0x18:
	case 0x19: fprintf(fWrite, "%s %s\n", regName[rs], regName[rt]);
		break;
	// print rd, rs
	case 0x09: fprintf(fWrite, "%s %s\n", regName[rd], regName[rs]);
		break;
	// print rs
	case 0x08:
	case 0x11:
	case 0x13: fprintf(fWrite, "%s\n", regName[rs]);
		break;
	// print rd
	case 0x10:
	case 0x12: fprintf(fWrite, "%s\n", regName[rd]);
		break;
	// print rd, rt, sa
	case 0x00:
	case 0x03:
	case 0x02: fprintf(fWrite, "%s %s %s\n", regName[rd], regName[rt], regName[sa]);
		break;
	// print rd, rt, rs
	case 0x04:
	case 0x07:
	case 0x06: fprintf(fWrite, "%s %s %s\n", regName[rd], regName[rt], regName[rs]);
		break;
	// print none
	default: 
		fprintf(fWrite, "\n");
	}
}

void Itype(FILE * fWrite, unsigned int ins)
{
	unsigned int opcode = ins >> 26;
	unsigned int rs = (ins >> 21) & 0x1F;
	unsigned int rt = (ins >> 16) & 0x1F;
	unsigned int imme = ins & 0x0000FFFF;

	// print instruction name
	for (int i = 0; i < RInstructionNum; ++i) {
		if (opcode == I_opcode[i]) {
			fprintf(fWrite, "%s ", I_name[i]);
		}
	}
	// print arguments
	switch (opcode) {
	// rt, rs, imme
	case 0x08:
	case 0x09:
	case 0x0C:
	case 0x0D:
	case 0x0A:
	case 0x0B:
	case 0x0E: fprintf(fWrite, "%s %s 0x%X\n", regName[rt], regName[rs], imme);
		break;
	// rs, rt, label
	case 0x04:
	case 0x05: fprintf(fWrite, "%s %s\n", regName[rs], regName[rt]);
		break;
	// rs, label
	case 0x01:
	case 0x07:
	case 0x06: fprintf(fWrite, "%s\n", regName[rs]);
		break;
	// rt, immediate
	case 0x0F: fprintf(fWrite, "%s  0x%X\n", regName[rt], imme);
		break;
	// rt, immediate(rs)
	default:  fprintf(fWrite, "%s  0x%X(%s)\n", regName[rt], imme, regName[rs]);
	}
}

void Jtype(FILE * fWrite, unsigned int ins)
{
	// j
	if ((ins & 0x04000000) == 0) {
		fprintf(fWrite, "%s \n", J_name[0]);
	}
	// jal
	else {
		fprintf(fWrite, "%s \n", J_name[1]);
	}
}

void Ctype(FILE * fWrite, unsigned int ins)
{
	unsigned int func_format = ((ins & 0x3F) << 5) | ((ins >> 21) & 0x1F);
	unsigned int ft = (ins >> 16) & 0x1F;
	unsigned int fs = (ins >> 11) & 0x1F;
	unsigned int fd = (ins >> 6) & 0x1F;

	// print instruction name
	for (int i = 0; i < RInstructionNum; ++i) {
		if (func_format == C_Func[i]) {
			fprintf(fWrite, "%s ", C_name[i]);
		}
	}
	// print arguments
	switch (func_format) {
	// fd, fs, ft
	case 0x010:
	case 0x414:
	case 0x490:
	case 0x070:
	case 0x050:
	case 0x030: fprintf(fWrite, "%s %s %s\n", regName[fd], regName[fs], regName[ft]);
		break;
	// ft, fs
	case 0x000:
	case 0x004: fprintf(fWrite, "%s %s\n", regName[ft], regName[fs]);
		break;
	// fd, fs
	default: fprintf(fWrite, "%s %s\n", regName[fd], regName[fs]);
	}
	printf("niconiconi\n");
}
