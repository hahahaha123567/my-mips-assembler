#ifndef mips_h
#define mips_h

#define RegisterNum 32
#define RInstructionNum 28
#define IInstructionNum 24
#define JInstructionNum 2
#define CInstructionNum 9

//Register
unsigned int reg[RegisterNum];
unsigned int pc;
char * regName[RegisterNum];

// Instruction
// R-Type: 
// This group contains all instructions that don't require an immediate value, target offset, memory address displacement, or memory address to specify an oprand. 
// This includes arithmetic and logic with all operands in registers, shift instructions, and register direct jump instructions.
// All R-Type instructions use opcode 000000.
char * R_name[RInstructionNum];
// store information of 6 bits with unsigned int
unsigned int R_Func[RInstructionNum];
// I-Type:
// This group includes instructions with an immediate operand, branch instructions, load and store instructions.
// In the MIPS architechture, all memory accesses are handled by the main processor, so coprocessor load and store instructions are included in this group.
// All opcodes except 000000, 00001x, and 0100xx are used for I-type instructions.
char * I_name[IInstructionNum];
unsigned int I_opcode[IInstructionNum];
// J-Type:
// This group consists of two direct jump instructions (j and jar).
// These instructions require a memory address to specify their operand.
// J-type instructions use opcodes 00001x.
char * J_name[JInstructionNum];
unsigned int J_opcode[JInstructionNum];
// Coprocessor Instructions: MIPS processors all have two standard coprocessors, CP0 and CP1.
// CP0 processor various kinds of program exceptions.
// CP1 is a floating point processor.
// The MIPS architecture makes allowance for future inclusion of two additional coprocessors, CP2 and CP3.
// All coprocessor instructions use opcede 0100xx.
char * C_name[CInstructionNum];
unsigned int C_Func[CInstructionNum];

// asm document that the user edit
char * asm[100];
int cursor;
// machine code that translated from the uesr input document
int count;
unsigned int code[0xFF];

#endif