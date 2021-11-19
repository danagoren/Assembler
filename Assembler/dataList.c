#include "dataList.h"

/*check additinal to the number input if there is any invalid char*/
int checkDataArgument(char* numberAdditional)
{
	removeSpaces(&numberAdditional);

	if((*numberAdditional) == '+' || (*numberAdditional) == '-')/*check if there is sign*/
	{
		numberAdditional++; /*promote numberAdditional*/
	}

	while(isdigit(*numberAdditional))/*check if this digit*/
	{
		numberAdditional++; /*promote numberAdditional*/
	}

	removeSpaces(&numberAdditional);

	if(*numberAdditional == '\0')/*all chars are valid*/
	{
		return 0;
	}
	return -1;/*there is an invalid char - error*/
} /*end of function checkDataArgument*/

int addDataNode(data_Node* pCurrentNode, int lineNumber, char* inputData, int dataAdress)
{
	/*defining variables*/
	const char s[2] = ","; /*string for cut with strtok*/
	char* token = NULL; /*pointer to point the cut string*/
	int tmpNumber = 0;
	int counter; /*to count num of words of data to allocate it*/
	int tempCounter = 0;
	int result = 0;
	int i,j = 0;

	/*count number of words for data*/
	if (strlen(inputData) != 0)/*there is data*/
	{
		counter = 1;
		for (i = 0; i < strlen(inputData); i++)
		{
			if(inputData[i] == ',') /*count the words account the comma*/
			{
				counter++; /*promote counter*/
			}
		}
	}
	else/*there is not data*/
	{
		strcpy(errorMessage, "No arguments"); /*set error message*/
		printError(lineNumber, errorMessage); /*print error with the function printError()*/
		return -1;
	}

	/*malloc size of words according of counter*/
	(pCurrentNode->data) = (sWord*)malloc(sizeof(sWord)*counter);
	if (!(pCurrentNode->data))
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(lineNumber , errorMessage); /*print error with the function printError()*/
		exit(1);
	}

	tempCounter = counter; /*according tempCounter put data in aloccated array until tempCounter= 0*/
	pCurrentNode->dataAdress = dataAdress;/*save the adress of the first of data*/

	/*take the input numbers*/
	token = strtok(inputData, s); /*cut the data according comma*/

	while (token != NULL && tempCounter != 0) /*while we have data*/
	{
		removeSpaces(&token);
		tmpNumber = atoi(token); /*get the data number by atoi*/
		result = checkDataArgument(token); /*check the number is valid*/
		if(result) /*number is invalid*/
		{
			strcpy(errorMessage, "Invalid data number"); /*set error message*/
			printError(lineNumber, errorMessage); /*print error with the function printError()*/
			return -1;
		}
		else /*number is valid*/
		{
			pCurrentNode->data[j].dataInWord = tmpNumber; /*set the number in the allocated array*/
			j++; /*promote to the next place in array*/
			tempCounter--; /*sub 1 from the  counter of data we have replace*/
		}
		token = strtok(NULL, s); /*get the next data*/
	} /*end of while*/

	pCurrentNode->numberOfWords = counter; /*save in node the number of data in array*/
	return counter; /*return to promote DC*/
} /*end of function addDataNode*/

/*this function add to currently node data from type of string*/
int addStringNode(data_Node* pCurrentNode,int lineNumber, char* inputData, int dataAdress)
{
	/*defining variables*/
	int counter = 0; /*to check length of the string*/
	int i = 0;
	char* p = inputData; /*temp pointer to save inputData pointer*/
	char* saveString = NULL;
	saveString = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH); /*allocated array to save string after process*/
	if (!saveString)/*check the allocated*/
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(lineNumber , errorMessage); /*print error with the function printError()*/
		exit(1);
	}

	memset(saveString, 0, MAX_LINE_LENGTH);

	for (i = 0; i < strlen(p); i++) /*check the strlen of string without \" */
	{
		if ((p[i] != '\"') && (p[i] != '\n') && (p[i] != '\0'))
		{
			strncat(saveString, &p[i], 1); /*save the char in saveString*/
			counter++;
		}
	} /*end of for*/

	if (counter + 2 != strlen(inputData)) /*check that there are not invalid chars without string and \" */
	{
		strcpy(errorMessage, "Missing \" in string"); /*set error message*/
		printError(lineNumber, errorMessage); /*print error with the function printError()*/
		return -1;
	}

	/*malloc size of words according of counter*/
	pCurrentNode->data = (sWord*)malloc(sizeof(sWord)*(counter + 1));
	if (!(pCurrentNode->data))
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(lineNumber , errorMessage); /*print error with the function printError()*/
		exit(1);
	}

	pCurrentNode->dataAdress = dataAdress;/*save the adress of the first of data*/

	/*save every char in one word +'/0'  */
	for (i = 0; i < counter; i++)
	{
		pCurrentNode->data[i].dataInWord = saveString[i];
	}

	pCurrentNode->data[i].dataInWord = '\0'; /*put the end of string*/

	if(saveString)
	{
		 free(saveString);
	} /*end of if*/

	pCurrentNode->numberOfWords = (counter + 1);  /*save the number of chars of current string in current node*/
	return (counter + 1); /*retern to promote DC*/
} /*end of function addStringNode*/

/*add node to dataList and send the node's pointer to relevant function numbers or string*/
int addNodeToDataList(data_Node** headDataList,char* inputData, int dataAdress, eData_type typeOfAddData,int lineNumber)
{
	/*defining variables*/
	static data_Node* current;
	int num_of_words = 0;

	/*it it is the first symbol, current points to head. in next times current always point to the last node*/
	if((*headDataList) == NULL)
	{
		(*headDataList) = (data_Node*)malloc(sizeof(data_Node));
		((*headDataList)->nextNode) = NULL;
		current = (*headDataList);
	} /*end of if*/
	else if(current)
	{
		current->nextNode = (data_Node*)malloc(sizeof(data_Node));
		current = current->nextNode;
		current->nextNode = NULL;
	} /*end of if*/

	if(typeOfAddData == data_type)
	{
		num_of_words = addDataNode(current, lineNumber,inputData, dataAdress); /*send the node to function handle numbers*/
	} /*end of if*/
	else if(typeOfAddData == string_type)
	{
		num_of_words = addStringNode(current, lineNumber, inputData, dataAdress); /*send the string  to function handle string*/
	} /*end of if*/
	return num_of_words;/*to promote DC*/
} /*end of function addNodeToDataList*/
