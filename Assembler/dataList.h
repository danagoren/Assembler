#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "errorsHandler.h"

/*defining sWord struct*/
typedef struct  word
{
	int dataInWord	:12;
} sWord;

/*defining data_Node struct*/
typedef struct dataNode
{
	int dataAdress;
	sWord* data;
	int numberOfWords;
	struct dataNode* nextNode;
} data_Node;

/*function declarations*/
int checkDataArgument(char* numberAdditional);
int addNodeToDataList(data_Node** headDataList,char* inputData, int dataAdress, eData_type typeOfAddData, int lineNumber);
int addDataNode(data_Node* pCurrentNode, int lineNumber, char* inputData, int dataAdress);
int addStringNode(data_Node* pCurrentNode, int lineNumber, char* inputData, int dataAdress);
void removeSpaces(char** pLine);

#endif
