#include "symbolTable.h"

/*the function addSymbolToList gets information for a new symbol and adds it to the list symbol_node*/
int addSymbolToList(symbol_node** headSymbolList,char* symbolName, int symbolAdress, eData_type DataOrCode_Attributes, eData_type EntryOrExtern_Attributes)
{
	/*defining variables*/
	static symbol_node* current;
	static symbol_node* head;
	symbol_node* tempHead;
	symbol_node* tempCurrent = current;

	/*it it is the first symbol, current points to head. in next times current always point to the last node*/
	if((*headSymbolList) == NULL)
	{
		(*headSymbolList) = (symbol_node*)malloc(sizeof(symbol_node));
		if (!(*headSymbolList))
		{
			strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
			printError(0, errorMessage); /*print error with the function printError()*/
			exit(1); /*exit the program*/
		}
		(*headSymbolList)->nextNode = NULL;
		tempHead = current = head = (*headSymbolList);
	}
	else if(current)
	{
		current->nextNode = (symbol_node*)malloc(sizeof(symbol_node));
		if (!(current->nextNode))
		{
			strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
			printError(0, errorMessage); /*print error with the function printError()*/
			exit(1); /*exit the program*/
		}
		current = current->nextNode;
		current->nextNode = NULL;
	}

	/*check if this label already exist*/
	tempHead = head;

	while (tempHead->nextNode != NULL)
	{
		if(tempHead->symbol_Name != NULL)
		{
			if(strcmp(tempHead->symbol_Name,symbolName) == 0)
			{
				/*if the symbol already exists but its extern_type, continue*/
				if(EntryOrExtern_Attributes == extern_type && tempHead->EntryOrExtern_Attributes == extern_type)
				{
					if (current)
					{
						free(current);
						current = tempCurrent;
					}
					return 0;
				}
				/*this label already exist*/
				if (current)
				{
					free(current);
					current = tempCurrent;
				}
				return -1;
			}
			tempHead = tempHead->nextNode;
		}
		else
		{
			break;
		}
	}

	/*set label name*/
	current->symbol_Name = (char*)malloc(sizeof(char)*(strlen(symbolName)+1));/*allocate symbol node*/
	if (!(current->symbol_Name))
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(0, errorMessage);  /*print error with the function printError()*/
		exit(1); /*exit the program*/
	}

	strcpy(current->symbol_Name, symbolName);/*set symbol*/
	strcat(current->symbol_Name, "\0");/*add '\0'*/

	/*set symbol Adress*/
	current->symbol_Value = symbolAdress;

	/*set Attributes*/
	current->EntryOrExtern_Attributes = EntryOrExtern_Attributes;
	current->DataOrCode_Attributes = DataOrCode_Attributes;

	return 0;
} /*end of function addSymbolToList*/

/*a function that adds a new entry to entry_node*/
int addToEntries(entry_node** headEntryList,char* entryName)
{
	/*defining variable*/
	static entry_node* current;

	/*it it is the first entry, current points to head. in next times current always points to the last entry*/
	if((*headEntryList) == NULL)
	{
		(*headEntryList) = (entry_node*)malloc(sizeof(entry_node));
		if (!(*headEntryList))
		{
			strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
			printError(0, errorMessage); /*print error with the function printError()*/
			exit(1); /*exit the program*/
		}
		((*headEntryList)->nextEntry) = NULL;
		current = (*headEntryList);
	}
	else if(current)
	{
		current->nextEntry = (entry_node*)malloc(sizeof(entry_node));
		if (!(current->nextEntry))
		{
			strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
			printError(0, errorMessage); /*print error with the function printError()*/
			exit(1); /*exit the program*/
		}
		current = current->nextEntry;
		current->nextEntry = NULL;
	}

	/*set entry name*/
	current->entry_Name = (char*)malloc(sizeof(char)*(strlen(entryName)+1));
	if (!(current->entry_Name))
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(0, errorMessage); /*print error with the function printError()*/
		exit(1); /*exit the program*/
	}
	strcpy(current->entry_Name,entryName);

	return 1;
} /*end of function addToEntries*/

/*a function that gets a symbol name and returns its value*/
int getSymbolValue(symbol_node* headSymbolList, char* symbolName)
{
	/*defining variable*/
	symbol_node *current = headSymbolList;

	while ((current)) /*while this is not the label*/
	{
		if (current->symbol_Name)
		{
			if (!strcmp((current->symbol_Name),symbolName))
			{
				return (current->symbol_Value); /*return the value of the current label*/
			}
		}
		current = (current->nextNode); /*move to the next node*/
	}
	if (!current) /*if reached end of list*/
	{
		strcpy(errorMessage, "The label \"");/*set error message*/
		strcat(errorMessage, symbolName);/*set error message*/
		strcat(errorMessage, "\" wasn't declared");/*set error message*/
		printError(0,errorMessage); /*print error with the function printError()*/
		return -1;
	}
	return -1;
} /*end of function getSymbolValue*/

/*a function that gets a symbol name and returns its DataOrCode_Attributes*/
int getDataOrCode(symbol_node* headSymbolList, char* symbolName)
{
	/*defining variable*/
	symbol_node *current = headSymbolList;

	while ((current) && (strcmp((current->symbol_Name),symbolName))) /*while this is not the label*/
	{
		current = (current->nextNode); /*move to the next node*/
	}
	if (!current) /*if reached end of list*/
	{
		strcpy(errorMessage, "The label \"");/*set error message*/
		strcat(errorMessage, symbolName);/*set error message*/
		strcat(errorMessage, "\" wasn't declared");/*set error message*/
		printError(0,errorMessage); /*print error with the function printError()*/
		return -1;
	}
	if (!(strcmp((current->symbol_Name),symbolName))) /*if symbol = symbol name*/
	{
		return (current->DataOrCode_Attributes); /*return the label's DataOrCode_Attributes*/
	}
	return -1;
} /*end of function getDataOrCode*/

/*a function that sets the adresses of all the labels that were not known in first process in instruction list*/
void setValue(instruction_Node* instruction_List_head, symbol_node* headSymbolList)
{
	/*defining variables*/
	int val; /*the label's value*/
	instruction_Node* current = instruction_List_head;

	while (current) /*while this node exists*/
	{
		if (current->labelFlag1) /*if labelflag1 is on*/
		{
			val = getSymbolValue(headSymbolList, (current->labelName1)); /*get the value using the function getSymbolValue*/
			if (val == -1) /*error in getSymbolValue()*/
			{
				current = (current->nextNode);
				continue;
			}
			else
			{
				(current->arg1) = val; /*set arg1*/
			}
		}
		if (current->labelFlag2) /*if labelflag2 is on*/
		{
			val = getSymbolValue(headSymbolList, (current->labelName2)); /*get the value using the function getSymbolValue*/
			if (val == -1) /*error in getSymbolValue()*/
			{
				current = (current->nextNode);
				continue;
			}
			else
			{
				(current->arg2) = val; /*set arg2*/
			}
		}
		current = (current->nextNode); /*move to the next node*/
	}
	return;
} /*end of function setValue*/

/*update adresses of data symbols according ICF*/
void updateSymbolTable(symbol_node* headSymbolList, int* ICF)
{
	/*defining variables*/
	symbol_node* current = headSymbolList;

	while (current) /*while this node exists*/
	{
		if (current->DataOrCode_Attributes == data_type) /*if this is a data type symbol*/
		{
			current->symbol_Value += (*ICF); /*adds ICF's value to data labels*/
		}
		current = current->nextNode; /*move to the next node*/
	}
} /*end of function updateSymbolTable*/
