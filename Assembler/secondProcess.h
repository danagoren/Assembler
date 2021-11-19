#include "firstProcess.h"

/*function declarations*/
void secondProcess(entry_node **entryHead, symbol_node **symbolHead, instruction_Node** instructionHead, data_Node** dataHead, char* objectFileName, char* entriesFileName, char* externalsFileName, int* ICF, int* DCF);
void printEntries(char *, symbol_node **);
void printExternals(char *externalsFileName, instruction_Node **instructionHead);
void printObject(char* objectFileName, instruction_Node **instructionHead, symbol_node *headSymbol, data_Node* dataHead, int ICF, int DCF);
void setEntryAttribute(char *entryName, symbol_node **headSymbol);
void freeLists(entry_node **entryHead, symbol_node **symbolHead, instruction_Node** instructionHead, data_Node** dataHead);
void freeEntries(entry_node **entryHead);
void freeSymbols(symbol_node **symbolHead);
void freeInstructions(instruction_Node** instructionHead);
void freeDatas(data_Node** dataHead);
char getLetter(char *labelName, symbol_node* headSymbolList);
unsigned int getWord(instruction_Node *current);
