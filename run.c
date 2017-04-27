#include "mips.h"
#include "run.h"


void run(unsigned int ins)
{
	switch (ins >> 26) {
	case 0x00: runR(ins);
		break;
	case 0x02:
	case 0x03: runJ(ins);
		break;
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13: runC(ins);
		break;
	default:   runI(ins);
	}
}

void runR(unsigned int ins)
{
	unsigned int rs = (ins >> 21) & 0x1F;
	unsigned int rt = (ins >> 16) & 0x1F;
	unsigned int rd = (ins >> 11) & 0x1F;
	unsigned int sa = (ins >> 6) & 0x1F;
	unsigned int func = ins & 0x3F;

	switch (func) {
	// add
	case 0x20:
		reg[rd] = reg[rs] + reg[rt];
		break;
	// sub
	case 0x22:
		reg[rd] = reg[rs] - reg[rt];
		break;
	// slt
	case 0x2A:
		reg[rd] = reg[rs] < reg[rt] ? 1 : 0;
		break;
	}
}

void runI(unsigned int ins)
{
	unsigned int opcode = ins >> 26;
	unsigned int rs = (ins >> 21) & 0x1F;
	unsigned int rt = (ins >> 16) & 0x1F;
	unsigned int imme = ins & 0x0000FFFF;

	switch (opcode) {
	// lw
	case 0x23:
		;// to declare a varible after a label
		char * ptr = code;
		ptr += reg[rs];
		ptr += (int)imme;
		reg[rt] = *(unsigned int *)ptr;
		break;
	// sw
	case 0x2B:
		;// to declare a varible after a label
		char * ptr = code;
		ptr += reg[rs];
		ptr += (int)imme;
		*(unsigned int *)ptr = reg[rt];
		break;
	// beq
	case 0x04:
		if (reg[rs] == reg[rt]) {
			pc += imme;
		}
		break;
	}
}

void runJ(unsigned int ins)
{
}

void runC(unsigned int ins)
{
}
