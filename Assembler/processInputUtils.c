#include "processInputUtils.h"

/*moves the pointer after spaces */
void removeSpaces(char** pLine)
{
	while (isspace(**pLine)) /*while this char is a white space*/
	{
		(*pLine)++; /*move to the next char*/
	} /*end of while*/
} /*end of function removeSpaces*/

/*a function that gets an assembly line from the assembly file, checks errors, and returns an organized line*/
int getNextLine(FILE *fAssembly_File, int *lineCountPointer, char **newLine)
{
	/*defining variables*/
	static int lineCounter = 0; /*usud to count the lines*/
	int i, a; /*indexes*/
	int length; /*the length of the line*/
	int firstSpace; /*used to find excess spaces and remove it*/
	int count; /*counter*/
	int first; /*the index of the first quotation mark*/
	char *line; /*the assembly line*/
	char trash[100];
	i = 0; /*set the index to zero (the first char)*/
	line = *newLine;/*line points to newLine*/

	if ((fgets(line, (MAX_LINE_LENGTH+1), fAssembly_File)) == 0) /*if the rest of the file is empty*/
	{
		return -1;/*exit the function*/
	}/*end of if*/

	if (newFile) /*if this is a new file*/
	{
		lineCounter = 0; /*start counting from 0*/
		newFile = 0;
	}

	*lineCountPointer = ++lineCounter; /*update the variables lineCountPointer and lineCounter*/

	if (line[strlen(line)-1] != '\n') /*if the line is too long*/
	{
		/*move the file position forward to the end of the line*/
		fgets(trash, (MAX_LINE_LENGTH+1), fAssembly_File);
		while ((strlen(trash)) && (trash[strlen(trash)-1] != '\n') && (trash[strlen(trash)-1] != '\0'))
		{
			fgets(trash, (MAX_LINE_LENGTH+1), fAssembly_File);
		}
		/*print error*/
		strcpy(errorMessage, "Line is too long");/*set error message*/
		printError(lineCounter,errorMessage);/*print error with the function printError()*/
		return 0;/*exit the function*/
	} /*end of if*/

	/***tests to see if we should skip this line***/

	if (line[0] == ';') /*if this line is a comment line - we skip it and return the next line*/
	{
		return 0;/*exit the function*/
	}/*end of if*/

	length = strlen(line); /*update line's length variable*/
	while ((isspace(line[i]))&&(i<length)) /*while this char is a white space*/
	{
		i++;/*promote the index*/
	}/*end of while*/
	if (i == length) /*if reached end of line - either all characters are white spaces, or it is an empty line - we skip it and return the next line*/
	{
		return 0;/*exit the function*/
	}/*end of if*/

	/***organazing and checking errors in the line***/

	/*removes spaces at the begining of the line*/
	i = 0;/*set the index to zero (the first char)*/
	while (isspace(line[0])) /*while the first char in the line is a white space*/
	{
		i = 0; /*set the index to zero (the first char)*/
		while ((i+1)<length) /*while this char is not the last char*/
		{
			line[i] = line[i+1]; /*move this char one place backwards*/
			i++; /*promote the index to check the next char*/
		} 
		line[length-1] = '\0'; /*set the last char to be empty*/
		length--; /*update the line's length*/
	}

	/*removes spaces at the end of the line*/
	while (isspace(line[length-1]))/*while the last char is a white space*/
	{
		line[length-1] = '\0';/*set the last char to be empty*/
		length--; /*update the line's length*/
	}/*end of while*/
	
	/***STRING TESTS:***/
	i = length-1;/*set the index to be the last char*/
	count = 0;/*set quotation mark counter to zero*/
	if (line[i] != '"') /*if line doesn't end with a quotation mark*/
	{
		while (i >= 0)/*while this index is positive or zero (not outside of the line)*/
		{
			if (line[i] == '"') /*if this char is a quotation mark*/
			{
				count++;/*promote the quotation mark counter*/
			} 
			i--;/*move to the previous char*/
		}/*end of while*/
		if (count) /*if there is at least one quotation mark*/
		{
			strcpy(errorMessage, "String line that doesn't end with a quotation mark (\").");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
	}/*end of if*/
	i = length-1;/*set the index to be the last char*/
	count = 0;/*set quotation mark counter to zero*/
	if (line[i] == '"') /*if line ends with a quotation mark*/
	{
		while (i >= 0)/*while this index is positive or zero (not outside of the line)*/
		{
			if (line[i] == '"') /*if this char is a quotation mark*/
			{
				count++;/*promote the quotation mark counter*/
				first = i; /*first is the index of the first quotation mark*/
			}
			i--;/*move to the previous char*/
		}/*end of while*/
	}/*end of if*/
	if (count) /*if there is at least one quotation mark*/
	{
		if (count == 1) /*if there is an illegal quantity of quotation marks (1)" */
		{
			strcpy(errorMessage, "Illegal quantity of quotation marks (\").");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/

		i = first-1;/*i is the index of the character before the first quotation mark*/
		if (!(isspace(line[i])))/*if this char is not a white space*/
		{
			strcpy(errorMessage, "No space before string");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
		i--;/*move to the previous char*/
		a = i;/*the index a gets i'w value*/

		while ((i >= 0)&&(isspace(line[i]))) /*while the previous char is an extra white space, remove it*/
		{
			while ((i+1) < length)/*while this char is not the last char*/
			{
				line[i] = line[i+1];/*move this char one place backwards*/
				i++;/*promote i*/
			}
			line[length-1] = '\0';/*set the last char to be empty*/
			length--;/*update line's length*/
			i = --a;/*move the indexes one place backwards*/
		}/*end of while*/

		if (i < 6) /*not enough characters to have .string in the line*/
		{
			strcpy(errorMessage, "Quotation marks without a .string command.");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
		/*if there is not a .string command*/
		if (!((line[i-6]=='.') && (line[i-5]=='s') && (line[i-4]=='t') && (line[i-3]=='r') && (line[i-2]=='i') && (line[i-1]=='n') && (line[i]=='g')))
		{
			strcpy(errorMessage, "Quotation marks without a .string command.");/*set error message*/
			printError(lineCounter,errorMessage);
			return 0;/*exit the function*/
		}/*end of if*/
		i -= 7;/*move the index to the char before the .string command*/
		if (i < 0)/*if there is no more characters in the line*/
		{
			return 1;/*exit the function*/
		}/*end of if*/
		if (!(isspace(line[i])))/*if this char is not a white space*/
		{
			strcpy(errorMessage, "No space before .string command");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
		i--;/*move i one place backwards*/
		a = i;/*a gets i's value*/

		while ((i >= 0)&&(isspace(line[i]))) /*while the previous char is an extra white space, remove it*/
		{
			while ((i+1) < length)/*while this char is not the last char*/
			{
				line[i] = line[i+1];/*move this char one place backwards*/
				i++;/*promote i*/
			}/*end of while*/
			line[length-1] = '\0';/*set the last char to be empty*/
			length--;/*update line's length*/
			i = --a;/*move the indexes one place backwards*/
		}/*end of while*/
		if ((i >= 0) && (line[i] != ':'))/*if this char isn't a ':' */
		{
			strcpy(errorMessage, "Illegal word before .string command.");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
		i--; /*i is the index of the last character in the label*/
		if (!(isdigit(line[i])) && !(isalpha(line[i])))/*if this char is not a letter, nor a number*/
		{
			strcpy(errorMessage, "Illegal character before :");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
		while ((isdigit(line[i])) || (isalpha(line[i])))/*while this char is a letter or a number*/
		{
			if (!i) /*if i is the index of the first char*/
			{
				i--;/*i = -1*/
				break;/*exit this while to prevent checking of characters before the line*/
			}
			i--;/*check the last char*/
		}/*end of while*/
		if (i >= 0) /*if there is a char that is not a letter nor a number in the label*/
		{
			strcpy(errorMessage, "Illegal characters in label");/*set error message*/
			printError(lineCounter,errorMessage);/*print error with the function printError()*/
			return 0;/*exit the function*/
		}/*end of if*/
	return 1;
	} /*end of if (END OF STRING TESTS)*/

	/*removes excess spaces*/
	i = 0; /*sets the index to 0 (the first char)*/
	while (i<length)/*while i is inside the line*/
	{
		if ((i<length)&&(isspace(line[i])))/*if this character is a white space*/
		{
			firstSpace = i;/*firstSpace is the index of the first space found*/
			i++;/*promote i to look at the next char*/
			if ((i<length)&&(isspace(line[i])))/*if the following character is a white space*/
			{
				while ((i+1)<length)/*while this char is not the last char*/
				{
					line[i] = line[i+1];/*move this char one place backwards*/
					i++;/*promote i*/
				}
				line[length-1] = '\0';/*set the last char to be empty*/
				length--;/*update line's length*/
				i = firstSpace-1;/*move i one place backwards*/
			}/*end of if*/
		}/*end of if*/
		i++;/*promote i*/
	}/*end of while*/

	/*checks if there is an illegal character at the beginning of the line*/
	if (((line[0]) != '.') && (!isalpha(line[0])))/*if the line starts with an illegal character*/
	{
		strcpy(errorMessage, "There is an illegal character at the beginning of the line");/*set error message*/
		printError(lineCounter,errorMessage);
		return 0;/*exit the function*/
	}/*end of if*/

	/*checks if there is an illegal character at the end of the line*/
	if (((line[length-1]) != '"') && (!isalpha(line[length-1])) && (!isdigit(line[length-1])))/*if the line ends with an illegal character*/
	{
		strcpy(errorMessage, "There is an illegal character at the end of the line");/*set error message*/
		printError(lineCounter,errorMessage);/*print error with the function printError()*/
		return 0;/*exit the function*/
	}/*end of if*/

	/*removes white spaces before and after commas*/
	i = 0;/*set the index to 0 (the first char in the line)*/
	while (i<length)/*while i is an index of a character that is inside the line*/
	{
		if (line[i] == ',') /*i can't be 0 because there was an error check to make sure the beginning of the line isn't illegal*/
		{
			a = i-1; /*a is the character before the coma that was found*/
			if (isspace(line[a])) /*if there is a white space before the comma*/
			{
				while ((a+1) < length)/*while this char is not the last char*/
				{
					line[a] = line[a+1];/*move this char one place backwards*/
					a++;/*move to the next char*/
				}
				line[length-1] = '\0';/*set the last char to be empty*/
				length--;/*update the line's length*/
				i--;/*move the index one place backwards*/
			}/*end of if*/
			a = i+1; /*a is the character after the coma that was found*/
			if (isspace(line[a])) /*if there is a white space after the comma*/
			{
				while ((a+1) < length)/*while this char is not the last char*/
				{
					line[a] = line[a+1];/*move this char one place backwards*/
					a++;/*move to the next char*/
				}
				line[length-1] = '\0';/*set the last char to be empty*/
				length--;/*update the line's length*/
			}/*end of if*/
		}/*end of if*/
		i++;/*move to the next char*/
	}/*end of while*/

	/*checks if there are two commas next to each other*/
	i = 0;/*set the index to 0 (the first char in the line)*/
	while (i<length)/*while i is an index of a character that is inside the line*/
	{
		while ((i<length)&&(line[i] != ','))/*while this char is not a coma*/
		{
			i++;/*move to the next char*/
		}
		if ((i<length)&&(line[i] == ','))/*if this char is a coma*/
		{
			i++;/*move to the next char*/
			if ((i<length)&&(line[i] == ','))
			{
				strcpy(errorMessage, "There are two commas next to each other");/*set error message*/
				printError(lineCounter,errorMessage);/*print error with the function printError()*/
				return 0;/*exit the function*/
			}
		}/*end of if*/
	}/*end of while*/

	/*checks if there are two or more : */
	i = 0;/*set the index i to 0 (the first char in the line)*/
	count = 0;/*set a to 0. used to count the ':' characters*/
	while (i<length)/*while i is an index of a character that is inside the line*/
	{
		if (line[i] == ':')/*if this char is a ':'*/
		{
			count++; /*promote the counter*/
		}
		i++;/*move to the next char*/
	}/*end of while*/

	if (count>1)/*if there are two or more ':'*/
	{
		strcpy(errorMessage, "There are two or more ':'");/*set error message*/
		printError(lineCounter,errorMessage);/*print error with the function printError()*/
		return 0;/*exit the function*/
	}/*end of if*/

	(*newLine) = line; /*update the variable 'newLine' to contain 'line'*/
	return 1;/*exit the function*/
}/*end of the function getNextLine*/
