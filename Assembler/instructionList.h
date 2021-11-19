#ifndef INSTRUCTION_LIST_H
#define INSTRUCTION_LIST_H

#include "dataList.h"

/*defining machine_Line struct*/
typedef struct machine_Instruction{
	unsigned int srcAdressing	:2;
	unsigned int destAdressing	:2;
	unsigned int Actionfunct	:4;
	unsigned int Actionopcode	:4;
} machine_Line;

/*defining instruction_Node struct*/
typedef struct instruction_Node{
	int instruction_adress;
	int instruction_words_counter;
	machine_Line firstInstructionWord;
	char firstInstructionWordARE;
	int arg1	:12;
	int arg2	:12;
	int labelFlag1;
	int labelFlag2;
	char* labelName1;
	char* labelName2;
	int lineNumber;
	struct instruction_Node* nextNode;
} instruction_Node;

/*defining memoryWord struct*/
typedef struct memoryWord{
	char wordSizeinBits[12];
} memoryWord;

/*function declaration*/
int addToInsructions(instruction_Node** headInstructionNode, instruction_Node* newNode, int lineCounter);

#endif
