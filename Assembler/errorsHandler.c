#include "errorsHandler.h"

/*a function that gets an error message and the line that the error was located, and prints the error message to stdout*/
void printError(int errorLine ,char* errorMessage)
{
	if (errorLine == -1) /*if no file was sent in the command line*/
	{
		printf("\nError: %s", errorMessage); /*prints error to stdout*/
		return; /*exit the function*/
	}
	if (errorsFlag == 0) /*if this is the first call to this function since zero-ing errorsFlag (it means that this is the first error)*/
	{
		printf("\n\nERRORS IN FILE \"%s\":", currentFileName); /*prints the file name as a headline to stdout, before printing the errors in the file*/
		errorsFlag = 1; /*turns errorsFlag on*/
	}
	if (errorLine) /*if the line that the error was located in is known*/
	{
		printf("\nline %d: \tError: %s", errorLine, errorMessage); /*prints error to stdout*/
	}
	else /*if we don't know the line the error was located in (could be a general error)*/
	{
		printf("\nError:\t%s", errorMessage); /*prints error to stdout*/
	}

	errorsFlag = 1; /*turn errorsFlag on*/
	return; /*exit the function*/
}/*end of the function printError*/


