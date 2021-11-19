#include "secondProcess.h"

/*second process - finish the lists that were made in the first process and print the output files (if no error was found)*/
void secondProcess(entry_node **entryHead, symbol_node **symbolHead, instruction_Node** instructionHead, data_Node** dataHead, char* objectFileName, char* entriesFileName, char* externalsFileName, int* ICF, int* DCF)
{
	/*defining variables*/
	entry_node* currentEntry = *entryHead; /*setting currentEntry to the head of the entry list*/
	while (currentEntry) /*while this entry is not NULL*/
	{
		setEntryAttribute(currentEntry->entry_Name, symbolHead); /*sets the current entry attribute in the symbol list*/
		currentEntry = (currentEntry->nextEntry); /*currentEntry points to the next entry*/
	} /*end of while*/

	setValue(*instructionHead, *symbolHead); /*sets the adresses of all the labels in instruction list using the symbol list*/

	if (errorsFlag == 1) /*if errors were found in this file*/
	{
		return; /*don't print output files*/
	}

	/*print output files*/
	printObject(objectFileName, instructionHead, *symbolHead, *dataHead, *ICF,*DCF); /*print object output file*/
	printEntries(entriesFileName, symbolHead); /*print entries output file*/
	printExternals(externalsFileName, instructionHead); /*print externals output file*/
} /*end of function secondProcess*/

/*a function that gets an entry name, checks in which lists the label already exists and checks for errors. if no error found, it sets symbol's attribute as entry_type*/
void setEntryAttribute (char *entryName, symbol_node **headSymbol)
{
	/*defining variables*/
	symbol_node* currentSymbol;
	currentSymbol = *headSymbol;

	while(currentSymbol) /*if this symbol exists*/
	{
		if (currentSymbol->symbol_Name)
		{
			if(!(strcmp(entryName, currentSymbol->symbol_Name))) /*if this symbol is the symbol we looked for*/
			{
				if ((currentSymbol->EntryOrExtern_Attributes) == extern_type)
				{
					strcpy(errorMessage, "The label \""); /*set error message*/
					strcat(errorMessage, entryName); /*set error message*/
					strcat(errorMessage, "\" was declared both entry and extern type"); /*set error message*/
					printError(0,errorMessage); /*print error with the function printError()*/
					return;
				}
				currentSymbol->EntryOrExtern_Attributes = entry_type; /*set the current symbol's EntryOrExtern_Attributes as entry_type*/
				return;
			}
		}
		currentSymbol = currentSymbol->nextNode; /*move to the next node*/
	}
	strcpy(errorMessage, "The entry \""); /*set error message*/
	strcat(errorMessage, entryName); /*set error message*/
	strcat(errorMessage, "\" wasn't declared"); /*set error message*/
	printError(0,errorMessage); /*print error with the function printError()*/
	return;
} /*end of function setEntryAttribute*/

/*this function prints the entries output file*/
void printEntries(char *entriesFileName, symbol_node **symbolHead)
{
	/*defining variables*/
	int count = 0; /*counter*/
	FILE *fEntries_file;
	symbol_node *current = *symbolHead; /*current points to head*/

	/*checks to see if there are any entries:*/
	while (current) /*while this node exists*/
	{
		if (current->EntryOrExtern_Attributes == entry_type) /*if this symbol is entry type*/
		{
			count++;
			break; /*exit while*/
		}
		current = (current->nextNode); /*move to the next node*/
	}
	if (count == 0) /*if there arn't any entries*/
	{
		return; /*don't print entries*/
	}

	fEntries_file = fopen(entriesFileName, "w"); /*there is at least on entry - creating an entry file*/
	if (!fEntries_file) /*if failed to create file*/
	{
		strcpy(errorMessage, "Failed to create file: "); /*set error message*/
		strcat(errorMessage, entriesFileName); /*addition to error message*/
		printError(0, errorMessage); /*print error with the function printError()*/
		exit(1); /*exit the program*/
	}

	current = *symbolHead; /*current points to head*/
	while(current) /*writing entries until reached end of list*/
	{
		if(current->EntryOrExtern_Attributes == entry_type) /*if this symbol is entry type*/
		{
			fprintf(fEntries_file, "%s %04d\n", current->symbol_Name, current->symbol_Value); /*print to output file*/
		}
		current = (current->nextNode); /*move to the next node*/
	}
} /*end of the function printEntries*/

/*this function prints the externals output file*/
void printExternals(char *externalsFileName, instruction_Node** instructionHead)
{
	/*defining variables*/
	int count = 0;
	FILE *fExternal_file;
	instruction_Node * current = *instructionHead; /*current points to head*/

	/*checks if there are any externals*/
	while (current) /*while the current node exists*/
	{
		if (current->labelFlag1) /*if there is a label1*/
		{
			if ((current->arg1) == 0)
			{
				count++;
				break; /*exit while*/
			}
		}
		if (current->labelFlag2) /*if there is a label2*/
		{
			if ((current->arg2) == 0)
			{
				count++;
				break; /*exit while*/
			}
		}
		current = (current->nextNode); /*move to the next node*/
	}

	if (!count) /*if there arn't any externals*/
	{
		return; /*don't print externals*/
	}

	fExternal_file = fopen(externalsFileName, "w");
	if (!fExternal_file) /*if failed to create file*/
	{
		strcpy(errorMessage, "Failed to create file: "); /*set error message*/
		strcat(errorMessage, externalsFileName); /*addition to error message*/
		printError(0, errorMessage); /*print error with the function printError()*/
		exit(1); /*exit the program*/
	}

	current = *instructionHead; /*current points to head*/
	while (current)
	{
		if (current->labelFlag1) /*if there is a label1*/
		{
			if ((current->arg1) == 0)
			{
				fprintf(fExternal_file, "%s %04d\n", (current->labelName1), ((current->instruction_adress)+1)); /*print to externals file*/
			}
		}
		if (current->labelFlag2) /*if there is a label2*/
		{
			if ((current->arg2) == 0)
			{
				fprintf(fExternal_file, "%s %04d\n", (current->labelName2), ((current->instruction_adress)+2)); /*print to externals file*/
			}
		}
		current = (current->nextNode); /*move to the current node*/
	}
} /*end of the function printExternals*/

/*print instruction list and data list to obj file*/
void printObject(char* objectFileName, instruction_Node **instructionHead, symbol_node *headSymbol, data_Node* dataHead, int ICF, int DCF)
{
	/*defining variables*/
	FILE *fObject_file;
	int y;
	int reg = 1;
	unsigned int tmpNumber = 0;
	instruction_Node *current = *instructionHead;/*pointer to head of instruction list*/
	data_Node* currentD = dataHead;/*pointer to head of data list*/

	if (!current)/*if no data*/
	{
		return;
	}
	/*open obj file*/
	fObject_file = fopen(objectFileName, "w");
	if (!fObject_file)
	{
		strcpy(errorMessage, "Failed to create file: "); /*set error message*/
		strcat(errorMessage, objectFileName); /*addition to error message*/
		printError(0, errorMessage); /*print error with the function printError()*/
		exit(1); /*exit the program*/
	}

	/*print the first line, size of instruction memory and data memory*/
	fprintf(fObject_file, "   %d %d \n", ICF - 100, DCF);

	while (current)/*there is data in instruction list*/
	{
		if ((current->instruction_words_counter) >= 1)/*num of words to print >= 1*/
		{
			/*print the first word*/
			fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress,(getWord(current)&0xFFF), 'A');
		}

		if ((current->instruction_words_counter) >= 2)/*num of words to print >= 2, print first arg*/
		{
			if (current->labelFlag1)/*if arg is label*/
			{
				if(current->firstInstructionWord.destAdressing != Relative_address)/*if his adressing is not Relative_address*/
				{
					fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+1,(current->arg1)&0xFFF,current->arg1?'R':'E');
				}
				else /*this is relative adress*/
				{
					/*check the label is code_attribute*/
					if((getDataOrCode(headSymbol,current->labelName1)) == data_type)
					{
						strcpy(errorMessage,"Relative Adress to data type label"); /*set error message*/
						printError(current->lineNumber, errorMessage); /*print error with the function printError()*/
					}
					/*print with calculate value*/
					fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+1,(current->arg1-(current->instruction_adress+1))&0xFFF,current->arg1?'A':'E');
				}
			}
			else if((current->firstInstructionWord.srcAdressing == Register_Direct_address && (current->instruction_words_counter) == 3) || (current->firstInstructionWord.destAdressing == Register_Direct_address))/*this is register*/
			{
				reg<<=(current->arg1);/*calculate value of register arg*/
				fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+1,reg&0xFFF,'A');
				reg = 1;
			}
			else/*other arg*/
			{
				fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+1,(current->arg1)&0xFFF,'A');
			}
		}
		if ((current->instruction_words_counter) == 3)/*num of words to print >= 3, print second arg*/
		{
			if (current->labelFlag2)/*if arg is label*/
			{
				fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+2,(current->arg2)&0xFFF,current->arg2?'R':'E');
			}
			else if(current->firstInstructionWord.destAdressing == Register_Direct_address)/*this is register*/
			{
				reg<<=(current->arg2);/*calculate value of register arg*/
				fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+2,reg&0xFFF,'A');
				reg = 1;
			}
			else/*other arg*/
			{
				fprintf(fObject_file, "%04d %03X %c\n", current->instruction_adress+2,(current->arg2)&0xFFF,'A');
			}
		}
		current = (current->nextNode);
	}

	/*print data list*/
	while (currentD)/*in every node, print the entire data array, each number in a different line*/
	{
		for (y=0;y < currentD->numberOfWords;y++)
		{
			tmpNumber = currentD->data[y].dataInWord;
			fprintf(fObject_file, "%04d %03X %c\n",(currentD->dataAdress+ICF),tmpNumber&0xFFF,'A');
			currentD->data[y].dataInWord = 0;
			currentD->dataAdress++;
		}
		currentD = currentD->nextNode;
	}
} /*end of function printObject*/

/*returns the mechine word of the first assembly word in decimal base*/
unsigned int getWord(instruction_Node *current)
{
	/*defining variable*/
	unsigned int temp = 0;/*for make the first word to print*/

	temp|=current->firstInstructionWord.Actionopcode;/*put the Actionopcode in the lowest place in temp*/
	temp<<=4;/*move 4 bits to the left*/

	temp|=current->firstInstructionWord.Actionfunct;/*put the Actionfunct in the lowest place in temp*/
	temp<<=2;/*move 2 bits to the left*/

	temp|=current->firstInstructionWord.srcAdressing;/*put the srcAdressing in the lowest place in temp*/
	temp<<=2;/*move 2 bits to the left*/

	temp|=current->firstInstructionWord.destAdressing;/*put the destAdressing in the lowest place in temp*/

	return temp;
} /*end of function getWord*/

/*this function frees all the lists*/
void freeLists(entry_node **entryHead, symbol_node **symbolHead, instruction_Node** instructionHead, data_Node** dataHead)
{
	/*call to all the functions that free lists*/
	freeEntries(entryHead);
	freeSymbols(symbolHead);
	freeInstructions(instructionHead);
	freeDatas(dataHead);
} /*end of function freeLists*/

/*this function frees all the dynamic allocation in the entry list*/
void freeEntries(entry_node **entryHead)
{
	/*defining variable*/
	entry_node *current = *entryHead; /*current points to head*/

	if (!current) /*if current is already free*/
	{
		return;
	}
	if (current->nextEntry) /*if there is a next node (current is not the end of the list), free the next node*/
	{
		freeEntries(&current->nextEntry);
	}
	/*now current is the last node*/

	if (current) /*if current exists*/
	{
		if (current->entry_Name) /*if current->entry_Name exists*/
		{
			free(current->entry_Name);
			(current->entry_Name) = NULL;
		}
		free(current);
		current = NULL;
	}
	return;
} /*end of function freeEntries*/

/*this function frees all the dynamic allocation in the symbol list*/
void freeSymbols(symbol_node **symbolHead)
{
	/*defining variable*/
	symbol_node *current = *symbolHead; /*current points to head*/

	if (!current) /*if current is already free*/
	{
		return;
	}
	if (current->nextNode) /*if there is a next node (current is not the end of the list), free the next node*/
	{
		freeSymbols(&current->nextNode);
	}
	/*now current is the last node*/

	if (current) /*if current exists*/
	{
		if (current->symbol_Name) /*if current->symbol_Name exists*/
		{
			free(current->symbol_Name);
			(current->symbol_Name) = NULL;
		}
		free(current);
		current = NULL;
	}
	return;
} /*end of function freeSymbols*/

/*this function frees all the dynamic allocation in the instruction list*/
void freeInstructions(instruction_Node** instructionHead)
{
	/*defining variable*/
	instruction_Node *current = *instructionHead; /*current points to head*/

	if (!current) /*if current is already free*/
	{
		return;
	}
	if (current->nextNode) /*if there is a next node (current is not the end of the list), free the next node*/
	{
		freeInstructions(&current->nextNode);
	}
	/*now current is the last node*/

	if (current) /*if current exists*/
	{
		free(current);
		current = NULL;
	}
	return;
} /*end of function freeInstructions*/

/*this function frees all the dynamic allocation in the data list*/
void freeDatas(data_Node** dataHead)
{
	/*defining variable*/
	data_Node *current = *dataHead; /*current points to head*/

	if (!current) /*if current is already free*/
	{
		return;
	}
	if (current->nextNode) /*if there is a next node (current is not the end of the list), free the next node*/
	{
		freeDatas(&current->nextNode);
	}
	/*now current is the last node*/

	if (current) /*if current exists*/
	{
		if (current->data) /*if current->data exists*/
		{
			free(current->data);
			(current->data) = NULL;
		}
		free(current);
		current = NULL;
	}
	return;
} /*end of function freeDatas*/
