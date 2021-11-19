#include "instructionList.h"

/*a function that adds an instruction node to instruction list*/
int addToInsructions(instruction_Node** headInstructionNode, instruction_Node* newNode, int lineCounter)
{
	/*defining variable*/
	static instruction_Node* current;

	/*it it is the first instruction, current points to head. in next times current always points to the last instruction*/
	if((*headInstructionNode) == NULL) /*if the list is empty*/
	{
		(*headInstructionNode) = (instruction_Node*)malloc(sizeof(instruction_Node));
		if (!(*headInstructionNode))/*if there is a problem in allocating memory*/
		{
			strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
			printError(lineCounter, errorMessage); /*print error with the function printError()*/
			exit(1); /*exit the program*/
		}
		((*headInstructionNode)->nextNode) = NULL;
		current = (*headInstructionNode);
	}
	else if(current)
	{
	current->nextNode = (instruction_Node*)malloc(sizeof(instruction_Node));
	if (!(current->nextNode)) /*if there is a problem in allocating memory*/
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(lineCounter, errorMessage); /*print error with the function printError()*/
		exit(1); /*exit the program*/
	}
	
	current = current->nextNode; /*move to the next node*/
	current->nextNode = NULL;
	}

	/*set current node*/
	(current->instruction_adress) = (newNode->instruction_adress);
	(current->lineNumber) = (newNode->lineNumber);
	(current->instruction_words_counter) = (newNode->instruction_words_counter);
	(current->firstInstructionWord) = (newNode->firstInstructionWord);
	(current->arg1) = (newNode->arg1);
	(current->arg2) = (newNode->arg2);
	(current->labelFlag1) = (newNode->labelFlag1);
	(current->labelName1) = (newNode->labelName1);
	(current->labelFlag2) = (newNode->labelFlag2);
	(current->labelName2) = (newNode->labelName2);

	return 0;
} /*end of function addToInsructions*/
