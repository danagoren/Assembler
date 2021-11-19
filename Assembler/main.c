/*******************************
Assembler Project (mmn 14)
By: Dana Goren and Bina Kryschek
*******************************/

#include "main.h"

/*global variables*/
int errorsFlag = 0; /*if an error was found, it will turn the errorsFlag on (to 1). every time we start to read a new file, errorsFlag is reset to 0 (off)*/
char errorMessage[MAX_ERROR_LENGTH]; /*if an error is found, we print errorMessage. it is set individualy for every new error*/
char *currentFileName; /*a string that contains the current file name. it is used in the function printError() to tell us in which files the errors are located*/
int newFile; /*a flag for the first process. used to reset the line counter when we start a new file (in the function getNextLine)*/

int main(int argc, char* argv[])
{
	/*arrays for FileNames*/
	char assemblyFileName[MAX_FILE_NAME_LENGTH];
	char objectFileName[MAX_FILE_NAME_LENGTH];
	char externalsFileName[MAX_FILE_NAME_LENGTH];
	char entriesFileName[MAX_FILE_NAME_LENGTH];

	/*defining lists heads*/
	instruction_Node* instruction_List_head = NULL;
	data_Node* data_List_head = NULL;
	symbol_node* symbol_head = NULL;
	entry_node* entry_List_head = NULL;

	/*defining variables*/
	FILE* fAssembly_File; /*a file pointer that later will be pointing to the assembly file(s)*/
	int i = 0; /*index*/
	int ICF = 0; /*to save the length of instructions memory*/
	int IDF = 0; /*to save the length of data memory*/

	/*Checking that there are files that were sent as arguments (in the command line)*/
	if (argc <= 1) /*if no file was sent (the first file should be the 2nd argument in the commend line*/
	{
		strcpy(errorMessage, "No file recieved from command line");/*set error message*/
		printError(-1, errorMessage);/*print error with the function printError()*/
		exit(1);/*exit the program*/
	}

	/*for each selected files - compile it. we compile all the files that were sent in command line*/
	for (i = 1; i < argc; i++)
	{
		/*set names of files*/
		sprintf(assemblyFileName, "%s.as", argv[i]);
		sprintf(objectFileName, "%s.ob", argv[i]);
		sprintf(externalsFileName, "%s.ext", argv[i]);
		sprintf(entriesFileName, "%s.ent", argv[i]);

		currentFileName = argv[i]; /*set currentFileName to contain the current file name*/

		/*Open Assembly File (in read mode)*/
		fAssembly_File = fopen(assemblyFileName, "r"); /*opening the assembly file (in read mode) and setting the fAssembly_File pointer to it*/
		if (!fAssembly_File) /*if there was a problem in opening the assembly file (in read mode)*/
		{
			strcpy(errorMessage, "Failed to open file: "); /*set error message*/
			strcat(errorMessage, argv[i]); /*addition to error message*/
			printError(0, errorMessage); /*print error with the function printError()*/
			continue; /*moving on to the next file*/
		}

		/*resetting variables*/
		errorsFlag = 0;
		ICF = 0;
		IDF = 0;
		instruction_List_head = NULL;
		data_List_head = NULL;
		symbol_head = NULL;
		entry_List_head = NULL;
		newFile = 1;

		/*start the first process*/
		firstProcess(&instruction_List_head, &data_List_head, &symbol_head,fAssembly_File,&entry_List_head,&ICF,&IDF);

		/*start the second process*/
		secondProcess(&entry_List_head, &symbol_head, &instruction_List_head, &data_List_head, objectFileName, entriesFileName, externalsFileName,&ICF,&IDF);

		/*free all the linked lists*/
		freeLists(&entry_List_head, &symbol_head, &instruction_List_head, &data_List_head);
	} /*end of for loop*/
	return 0;
} /*end of main*/
