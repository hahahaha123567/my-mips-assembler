#include "mips.h"

unsigned int reg[RegisterNum] = { 0 };
pc = 0;
char * regName[RegisterNum] = {
	"$zero",//------------------------------------------------Always has the value 0, any write to this register are ignored;
	"$at",	//------------------------------------------------Reverse for assembler
	"$v0", "$v1",	//----------------------------------------Values for results and expression evaluation
	"$a0", "$a1", "$a2", "$a3",	//----------------------------Arguments
	"$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",	//Temporaries
	"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",	//Saved
	"$t8", "$t9",	//----------------------------------------More temporaries
	"$k0", "$k1",	//----------------------------------------Reversed for the operating system kernel and exception return
	"$gp",	//------------------------------------------------Global pointer
	"$sp",	//------------------------------------------------Stack pointer
	"$fp",	//------------------------------------------------Frame pointer
	"$ra"	//------------------------------------------------Return address
};

// Instruction
// R-Type: 
// This group contains all instructions that don't require an immediate value, target offset, memory address displacement, or memory address to specify an oprand. 
// This includes arithmetic and logic with all operands in registers, shift instructions, and register direct jump instructions.
// All R-Type instructions use opcode 000000.
char * R_name[RInstructionNum] = {
	"add", "addu", "and", "break", "div", "divu", "jalr",
	"jr", "mfhi", "mflo", "mthi", "mtlo", "mult", "multu",
	"nor", "or", "sll", "sllv", "slt", "sltu", "sra",
	"srav", "srl", "srlv", "sub", "subu", "syscall", "xor"
};
// store information of 6 bits with unsigned int
unsigned int R_Func[RInstructionNum] = {
	0x20, 0x21, 0x24, 0x0D, 0x1A, 0x1B, 0x09,
	0x08, 0x10, 0x12, 0x11, 0x13, 0x18, 0x19,
	0x27, 0x25, 0x00, 0x04, 0x2A, 0x2B, 0x03,
	0x07, 0x02, 0x06, 0x22, 0x23, 0x0C, 0x26
};
// I-Type:
// This group includes instructions with an immediate operand, branch instructions, load and store instructions.
// In the MIPS architechture, all memory accesses are handled by the main processor, so coprocessor load and store instructions are included in this group.
// All opcodes except 000000, 00001x, and 0100xx are used for I-type instructions.
char * I_name[IInstructionNum] = {
	"addi", "addiu", "andi", "beq", "bgez", "bgtz", "blez", "bltz",
	"bne", "lb", "lbu", "lh", "lhu", "lui", "lw", "lwcl",
	"ori", "sb", "slti", "sltiu", "sh", "sw", "swcl", "xori"
};
unsigned int I_opcode[IInstructionNum] = {
	0x08, 0x09, 0x0C, 0x04, 0x01, 0x07, 0x06, 0x01,
	0x05, 0x20, 0x24, 0x21, 0x25, 0x0F, 0x23, 0x31,
	0x0D, 0x28, 0x0A, 0x0B, 0x29, 0x2B, 0x39, 0x3E
};
// J-Type:
// This group consists of two direct jump instructions (j and jar).
// These instructions require a memory address to specify their operand.
// J-type instructions use opcodes 00001x.
char * J_name[JInstructionNum] = {
	"j", "jal"
};
unsigned int J_opcode[JInstructionNum] = {
	0x02, 0x03
};
// Coprocessor Instructions: MIPS processors all have two standard coprocessors, CP0 and CP1.
// CP0 processor various kinds of program exceptions.
// CP1 is a floating point processor.
// The MIPS architecture makes allowance for future inclusion of two additional coprocessors, CP2 and CP3.
// All coprocessor instructions use opcede 0100xx.
char * C_name[CInstructionNum] = {
	"add.s", "cvt.s.w", "cvt.w.s",
	"div.s", "mfcl", "mov.s",
	"mtcl", "mul.s", "sub.s"
};
unsigned int C_Func[CInstructionNum] = {
	0x010, 0x414, 0x490,
	0x070, 0x000, 0x0D0,
	0x004, 0x050, 0x030
};
