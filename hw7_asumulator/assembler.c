#include "assembler.h"

// judge the instruction type, and call the correspinding function
unsigned int instructionDrive(FILE * fp, char * name)
{
	for (int i = 0; i < RInstructionNum; ++i)
	{
		if(!strcmp(name, R_name[i])){
			return R_Instruction(fp, i);
		}
	}
	for (int i = 0; i < IInstructionNum; ++i)
	{
		if(!strcmp(name, I_name[i])){
			return I_Instruction(fp, i);
		}
	}
	for (int i = 0; i < JInstructionNum; ++i)
	{
		if(!strcmp(name, J_name[i])){
			return J_Instruction(fp, i);
		}
	}
	for (int i = 0; i < CInstructionNum; ++i)
	{
		if(!strcmp(name, C_name[i])){
			return C_Instruction(fp, i);
		}
	}
	printf("Instruction error!\n");
	return 0;
}

unsigned int R_Instruction(FILE * fp, int id)
{
	unsigned int ins = 0;
	char rs[10] = { 0 }, rt[10] = { 0 }, rd[10] = { 0 }, sa[10] = { 0 };
	unsigned int rsValue = 0, rtValue = 0, rdValue = 0, saValue = 0, func = R_Func[id];

	switch (id) {
	// no arguments
	case 3:
	case 26: break;
	// rs, rt
	case 4:
	case 5:
	case 12:
	case 13: fscanf(fp, "%s %s", rs, rt);
		rsValue = regName2Num(rs);
		rtValue = regName2Num(rt);
		break;
	// rd, rs
	case 6: fscanf(fp, "%s %s", rd, rs);
		rdValue = regName2Num(rd);
		rsValue = regName2Num(rs);
		break;
	// rs
	case 7:
	case 10:
	case 11: fscanf(fp, "%s", rs);
		rsValue = regName2Num(rs);
		break;
	// rd
	case 8:
	case 9: fscanf(fp, "%s", rd);
		rdValue = regName2Num(rd);
		break;
	// rd, rt, sa
	case 16: 
	case 20:
	case 22: fscanf(fp, "%s %s %s", rd, rt, sa);
		rdValue = regName2Num(rd);
		rtValue = regName2Num(rt);
		saValue = regName2Num(sa);
		break;
	// rd, rt, rs
	case 17:
	case 21:
	case 23: fscanf(fp, "%s %s %s", rd, rt, rs);
		rdValue = regName2Num(rd);
		rtValue = regName2Num(rt);
		rsValue = regName2Num(rs);
		break;
	// rd, rs, rt
	default: fscanf(fp, "%s %s %s", rd, rs, rt);
		rdValue = regName2Num(rd);
		rsValue = regName2Num(rs);
		rtValue = regName2Num(rt);
	}
	ins = ins | rsValue;
	ins <<= 5;
	ins = ins | rtValue;
	ins <<= 5;
	ins = ins | rdValue;
	ins <<= 5;
	ins = ins | saValue;
	ins <<= 6;
	ins = ins | func;

	return ins;
}

unsigned int I_Instruction(FILE * fp, int id)
{
	unsigned int ins = 0;
	char rs[10] = { 0 }, rt[10] = { 0 }, label[20] = { 0 };
	unsigned int opcode = I_opcode[id], rsValue = 0, rtValue = 0, imme = 0, labelValue = 0;

	switch (id) {
	// rt, rs, imme
	case 0:
	case 1:
	case 2:
	case 16:
	case 18:
	case 19:
	case 23: fscanf(fp, "%s %s %u", rt, rs, &imme);
		rtValue = regName2Num(rt);
		rsValue = regName2Num(rs);
		break;
	// rs, rt, label
	case 3:
	case 8: fscanf(fp, "%s %s %s", rs, rt, label);
		rsValue = regName2Num(rs);
		rtValue = regName2Num(rt);
		break;
	// rs, label
	case 4:
	case 5:
	case 6:
	case 7: fscanf(fp, "%s %s", rs, label);
		rsValue = regName2Num(rs);
		break;
	// rt, imme
	case 13: fscanf(fp, "%s %u", rt, &imme);
		rtValue = regName2Num(rt);
		break;
	// rt, immediate(rs)
	default: fscanf(fp, "%s %d(%s", rt, &imme, rs);
		rtValue = regName2Num(rt);
		rsValue = regName2Num(rs);
	}
	ins = ins | opcode;
	ins <<= 5;
	ins = ins | rsValue;
	ins <<= 5;
	ins = ins | rtValue;
	ins <<= 16;
	ins = ins | imme;

	return ins;
}

unsigned int J_Instruction(FILE * fp, int id)
{
	unsigned int ins = 0, labelValue = 0;
	char label[20] = { 0 };

	fscanf(fp, "%s", label);
	ins = ins | J_opcode[id];
	ins <<= 26;
	ins = ins | labelValue;

	return ins;
}

unsigned int C_Instruction(FILE * fp, int id)
{
	unsigned int ins = 0, func_format = 0, ftValue = 0, fsValue = 0, fdValue = 0;
	char ft[10] = { 0 }, fs[10] = { 0 }, fd[10] = { 0 };

	switch (id) {
	// ft, fs
	case 4:
	case 6: fscanf(fp, "%s %s", ft, fs);
		ftValue = regName2Num(ft);
		fsValue = regName2Num(fs);
		break;
	// fd, fs
	case 5: fscanf(fp, "%s %s", fd, fs);
		fdValue = regName2Num(fd);
		fsValue = regName2Num(fs);
		break;
	// fd, fs, ft
	default: fscanf(fp, "%s %s %s", fd, fs, ft);
		fdValue = regName2Num(fd);
		fsValue = regName2Num(fs);
		ftValue = regName2Num(ft);
	}
	ins = ins | 0x11;
	ins <<= 5;
	ins = ins | (C_Func[id] & 0x1F);
	ins <<= 5;
	ins = ins | ftValue;
	ins <<= 5;
	ins = ins | fsValue;
	ins <<= 5;
	ins = ins | fdValue;
	ins <<= 6;
	ins = ins | (C_Func[id] >> 5);

	return ins;
}

int regName2Num(char * name)
{
	char * p = name;
	while (*p != '\0') {
		if (*p == ',' || *p == ')') {
			*p = '\0';
			break;
		}
		p++;
	}
	for (int i = 0; i < RegisterNum; ++i) {
		if (strcmp(name, regName[i]) == 0) {
			return i;
		}
	}
	printf("Register error!\n");
	exit(0);
}

