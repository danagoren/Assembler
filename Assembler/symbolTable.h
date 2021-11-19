#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "instructionList.h"

/*defining symbol_node struct*/
typedef struct symbol {
	char*		symbol_Name;
	int		symbol_Value;
	eData_type	DataOrCode_Attributes;
	eData_type	EntryOrExtern_Attributes;
	struct symbol*	nextNode;
} symbol_node;

/*defining entry_node struct*/
typedef struct entries {
	char* entry_Name;
	struct entries* nextEntry;
} entry_node;

/*function declarations*/
int addSymbolToList(symbol_node** headSymbolList,char* symbolName, int symbolAdress, eData_type DataOrCode_Attributes, eData_type EntryOrExtern_Attributes);
int addToEntries(entry_node** headEntryList,char* entryName);
int getSymbolValue(symbol_node* headSymbolList, char* symbolName);
void setValue(instruction_Node* instruction_List_head, symbol_node* headSymbolList);
void updateSymbolTable(symbol_node* headSymbolList, int* ICF);
int getValue(symbol_node* headSymbolList, char* symbolName);
int getDataOrCode(symbol_node* headSymbolList, char* symbolName);

#endif
