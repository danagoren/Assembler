#include "firstProcess.h"

/*a function that gets a string and compares it with reserved words. it returns 1 if the given string is a reserved word. if it's not, it returns 0*/
int isReservedWord(char *word, int lineCounter)
{
	/*defining variables*/
	int i = 0; /*the index is 0*/
	char reservedWords [28][6] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop", "data", "string", "entry", "extern", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"}; /*an array of 28 reserved words (strings)*/

	while (i < 28) /*while the index i is inside the array*/
	{
		if (strcmp(word, reservedWords[i]) == 0) /*if this word is a reserved word*/
		{
			strcpy(errorMessage, "The label is a resereved keyword"); /*set error message*/
			printError(lineCounter,errorMessage); /*print error with the function printError()*/
			return 1;
		}
		i++; /*promote the index*/
	}
	return 0;
} /*end of function isReservedWord*/


/*the function get char* and check if the token is CPU register,the function will return the register number, if it is not register return -1*/
int isRegister(char* inputToken)
{
	if (strncmp(inputToken, "r0", 2) == 0)
	{
		if(!inputToken[2]) /*it ends with '\0'*/
		{
			return oger_r0;
		}
	}
	else if (strncmp(inputToken, "r1", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r1;
		}
	}
	else if (strncmp(inputToken, "r2", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r2;
		}
	}
	else if (strncmp(inputToken, "r3", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r3;
		}
	}
	else if (strncmp(inputToken, "r4", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r4;
		}
	}
	else if (strncmp(inputToken, "r5", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r5;
		}
	}
	else if (strncmp(inputToken, "r6", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r6;
		}
	}
	else if (strncmp(inputToken, "r7", 2) == 0)
	{
		if (!inputToken[2])/*it ends with '\0'*/
		{
			return oger_r7;
		}
	}
	return -1;
} /*end of function isRegister*/

/*this function get token of operand and his data , check if valid and save him and his data in sTempInstructionData*/
int setOperandsData(char* line,char* token, int registerFlag,instruction_Node* sTempInstructionData, int currentOperand,int numReqOperands,int lineCounter)
{
	/*defining variables*/
	int isFirst = 1;/*if the first of number*/
	int index = 0;
	int sizeOfLabel;/*to save label len*/
	int tempLength;

	/*if it's not register*/
	if (registerFlag == -1)
	{
		if ((*token) == '#')
		{ /* if is a number -> mion_miadi*/
			while (*(token + (++index)))
			{
				if (isFirst && ((*(token + index)) == 43 || (*(token + index)) == 45))/*+ or - in first*/
				{
					isFirst = 0;
					continue;
				}
				else if (((*(token + index)) < 48 || (*(token + index)) > 57))/*invalid ascii for number*/
				{
					strcpy(errorMessage, "Invalid Number"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					return -1;
				}
			}
			if (currentOperand == 1)/*if this is first operand*/
			{
				sTempInstructionData->arg1 = atoi((token+1));/*get the number*/
				if(numReqOperands == 1)/*if this the first from one we need*/
				{
					if(sTempInstructionData->firstInstructionWord.Actionopcode != opcode_prn)/*check if this is not opcode prn - error*/
					{
						strcpy(errorMessage, "Invalid addressing type of operand"); /*set error message*/
						printError(lineCounter, errorMessage); /*print error with the function printError()*/
						return -1;
					}
					sTempInstructionData->instruction_words_counter = 2;/*num of words is 2*/
					sTempInstructionData->firstInstructionWord.srcAdressing = 0;/*src = 0*/
					sTempInstructionData->firstInstructionWord.destAdressing = Immediate_address;/*dst = Immediate_address*/
				}
				else if(numReqOperands == 2)/*if this is the first operand from 2 */
				{
					if(sTempInstructionData->firstInstructionWord.Actionopcode == opcode_lea)/*check if this is opcode lea error*/
					{
						strcpy(errorMessage, "Invalid addressing type of first operand"); /*set error message*/
						printError(lineCounter, errorMessage); /*print error with the function printError()*/
						return -1;
					}
					sTempInstructionData->instruction_words_counter = 3;/*num of words is 3*/
					sTempInstructionData->firstInstructionWord.srcAdressing = Immediate_address;/*src = Immediate_address*/
				}

			}
			else if(currentOperand == 2)/* if this is second operand*/
			{
				if(sTempInstructionData->firstInstructionWord.Actionopcode != opcode_cmp)/*check if this is not opcode cmp - error*/
				{
					strcpy(errorMessage, "Invalid addressing type of second operand"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					return -1;
				}
				sTempInstructionData->arg2 = atoi((token+1));/*get the number*/
				sTempInstructionData->instruction_words_counter = 3;/*num of words is 3*/
				sTempInstructionData->firstInstructionWord.destAdressing= Immediate_address;/*dst = Immediate_address*/
			}

		}
		else if ((*token) == '%') /* if arg is a label which point to Relative_address*/
		{ 
			if (!(sTempInstructionData->firstInstructionWord.Actionopcode == opcode_jmp))/*if not opcode 9*/
			{
				strcpy(errorMessage, "Invalid %"); /*set error message*/
				printError(lineCounter, errorMessage); /*print error with the function printError()*/
				return -1;
			}

			if (currentOperand == 1)/*if this is first operand, it is must to be*/
			{
				sTempInstructionData->firstInstructionWord.destAdressing = Relative_address;/*dst = Relative_address*/
				sTempInstructionData->labelFlag1 = 1;   /*flag of label on*/
				sizeOfLabel = getLabelLength(token + 1);/*get label len*/
				if(sizeOfLabel <= 0)
				{
					strcpy(errorMessage,"invalid Label"); /*set error message*/
					printError(lineCounter,errorMessage); /*print error with the function printError()*/
				    	return -1;
				}
				/*set label name*/
				sTempInstructionData->labelName1 = (char*)malloc(sizeof(char)*(MAX_LABEL_LENGTH+1));/*allocate to save the label*/
				if (!(sTempInstructionData->labelName1))
				{
					strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
					printError(lineCounter , errorMessage); /*print error with the function printError()*/
					exit(1);
				}
				strncpy(sTempInstructionData->labelName1,token + 1,sizeOfLabel); /*copy the label*/
				tempLength = sizeOfLabel;
				while (tempLength<=MAX_LABEL_LENGTH)
				{
					sTempInstructionData->labelName1[tempLength] = '\0';
					tempLength++;
				}

				/*if is not valid label*/
				if((isLabelLegal(sTempInstructionData->labelName1, sizeOfLabel, lineCounter) == -1) || (sizeOfLabel == -1))
				{
					if (sizeOfLabel == -1)
					{
						strcpy(errorMessage,"Illegal Label"); /*set error message*/
						printError(lineCounter, errorMessage); /*print error with the function printError()*/
					}
					if (sTempInstructionData->labelName1)
					{
						free(sTempInstructionData->labelName1); /*free dinamic allocations*/
					}
					return -1;
				}
				sTempInstructionData->instruction_words_counter = 2;/*num of words is 2*/
			}

		}
		else
		{ /* if nothing from above, so its must to be Direct_address*/
			if (currentOperand == 1)/* if this is first operand*/
			{
				if(numReqOperands == 1)/*it is first from one*/
				{
					sTempInstructionData->firstInstructionWord.destAdressing = Direct_address;/*dst = Direct_address*/
				}
				else/*it is first from two*/
				{
					sTempInstructionData->firstInstructionWord.srcAdressing = Direct_address;/*dst = Direct_address*/

				}
				sTempInstructionData->labelFlag1 = 1;/*flag of label is on*/
				sizeOfLabel = getLabelLength(token);/*get label len*/
				if(sizeOfLabel <= 0)
				{
					strcpy(errorMessage,"Invalid Label"); /*set error message*/
					printError(lineCounter,errorMessage); /*print error with the function printError()*/
					return -1;
				}

				/*set label name*/
				sTempInstructionData->labelName1 = (char*)malloc(sizeof(char)*(MAX_LABEL_LENGTH+1));/*allocate to save the label*/
				if (!(sTempInstructionData->labelName1))
				{
					strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
					printError(lineCounter , errorMessage); /*print error with the function printError()*/
					exit(1);
				}
				strncpy(sTempInstructionData->labelName1,token,sizeOfLabel); /*copy the label*/
				tempLength = sizeOfLabel;
				while (tempLength<=MAX_LABEL_LENGTH)
				{
					sTempInstructionData->labelName1[tempLength] = '\0';
					tempLength++;
				}
				/*if is not valid label*/
				if((isLabelLegal(sTempInstructionData->labelName1, sizeOfLabel, lineCounter) == -1) || (sizeOfLabel == -1))
				{
					if (sizeOfLabel == -1)
					{
						strcpy(errorMessage,"Illegal Label"); /*set error message*/
						printError(lineCounter, errorMessage); /*print error with the function printError()*/
					}
					return -1;
				}
				sTempInstructionData->instruction_words_counter = 2;/*num of words = 2*/
			}
			else if (currentOperand == 2)/* if this is second operand*/
			{
				sTempInstructionData->firstInstructionWord.destAdressing = Direct_address;/*dst = Direct_address*/
				sTempInstructionData->labelFlag2 = 1;/*on flag of label*/
				sizeOfLabel = getLabelLength(token);/*get size of label*/
				if(sizeOfLabel <= 0)
				{
					strcpy(errorMessage,"Invalid Label"); /*set error message*/
					printError(lineCounter,errorMessage); /*print error with the function printError()*/
					return -1;
				}

				/*set label name*/
				sTempInstructionData->labelName2 = (char*)malloc(sizeof(char)*(MAX_LABEL_LENGTH+1));/*allocate to save the label*/
				if (!(sTempInstructionData->labelName2))
				{
					strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
					printError(lineCounter , errorMessage); /*print error with the function printError()*/
					exit(1);
				}
				strncpy(sTempInstructionData->labelName2,token,sizeOfLabel); /*copy the label*/
				tempLength = sizeOfLabel;
				while (tempLength<=MAX_LABEL_LENGTH)
				{
					sTempInstructionData->labelName2[tempLength] = '\0';
					tempLength++;
				}


				/*if is valid label*/
				if((isLabelLegal(sTempInstructionData->labelName2, sizeOfLabel, lineCounter) == -1) || (sizeOfLabel == -1))
				{
					if (sizeOfLabel == -1)
					{
						strcpy(errorMessage,"Illegal Label"); /*set error message*/
						printError(lineCounter, errorMessage); /*print error with the function printError()*/
					}
					return -1;
				}
				sTempInstructionData->instruction_words_counter = 3;/*num of words = 3*/
			}
		}
	}
	else
	{
		/* will set oger number*/
		if (currentOperand == 1)/*if it is first operand*/
		{
			sTempInstructionData->arg1 = registerFlag; /*set enum of register*/
			if(numReqOperands == 1)/*and it is first from one*/
			{
				if(sTempInstructionData->firstInstructionWord.Actionopcode == opcode_jmp)/*check if this is opcode 9 of jmp,bne,jsr*/
				  {
					 strcpy(errorMessage, "Invalid addressing type of operand"); /*set error message*/
					 printError(lineCounter, errorMessage); /*print error with the function printError()*/
					 return -1;
				  }
				sTempInstructionData->firstInstructionWord.srcAdressing = 0;/*src = 0*/
				sTempInstructionData->firstInstructionWord.destAdressing = Register_Direct_address;/*dst = Register_Direct_address*/
				sTempInstructionData->instruction_words_counter =  2;/*num of words*/
			}
			else if(numReqOperands == 2)/*and it is first from 2*/
			{
				if(sTempInstructionData->firstInstructionWord.Actionopcode == opcode_lea)/*check if this is opcode lea*/
				{
					strcpy(errorMessage, "Invalid addressing type of first operand"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					return -1;
				}
				sTempInstructionData->firstInstructionWord.srcAdressing = Register_Direct_address;/*src = Register_Direct_address*/
				sTempInstructionData->instruction_words_counter =  2;/*num of words*/
			}
		}
		/* will set dst oger number*/
		else if (currentOperand == 2)/*if this is the second operand*/
		{
			sTempInstructionData->arg2 = registerFlag; /*enum of register*/
			sTempInstructionData->firstInstructionWord.destAdressing = Register_Direct_address;/*dst = Register_Direct_address*/
			sTempInstructionData->instruction_words_counter =  3;/*num of words*/
		}
	}
	return 0;
} /*end of function setOperandsData*/

/*find the operands in line by strtok ,find the type of operand and send them to process and to save there data*/
int checkOperands(char* inputLine,int numOfOperands,int LineCounter,instruction_Node* sTempInstructionData)
{
	/*defining variables*/
	char* line;
	const char comma[2] = ","; /*for cut args with strtok*/
	char* tokenComma = NULL; /*the string that token with comma;*/
	int operandsCounter = 0; /*if this is the first or second operand*/
	int registerFlag = -1; /*This flag save Register value from enum*/
	int counterCheck = 0;/*count num of operands in line to check after if it is valid for opcode*/
	int i = 0;/*index*/

	line = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH + 1); /*to save inputline pointer*/
	if (!line)
	{
		strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
		printError(0 , errorMessage); /*print error with the function printError()*/
		exit(1);
	}
	memset(line, 0, sizeof(char) * MAX_LINE_LENGTH +1);
	strcpy(line, inputLine);/*copy inputline to save it*/

	/*if this is opcode with no operands*/
	if(numOfOperands == 0)
	{
		/*check there is no data after*/
		sTempInstructionData->instruction_words_counter = 1; /*num of words in this line instruction*/
		sTempInstructionData->firstInstructionWord.destAdressing = 0; /* no destAdressing*/
		sTempInstructionData->firstInstructionWord.srcAdressing = 0; /*no srcAdressing*/
		removeSpaces(&inputLine);
		if(*inputLine !='\0')
		{
			strcpy(errorMessage, "invalid Operand"); /*set error message*/
			printError(LineCounter, errorMessage); /*print error with the function printError()*/
			return -1;
		}
		return 0;
	}
	/*count number of operands according comma*/
	if (strlen(inputLine) != 0)/*there is data*/
	{
		for (i = 0; i < strlen(inputLine); i++)
		{
			if(inputLine[i] == ',') /*count the operands account the comma*/
			{
				counterCheck++; /*promote counter*/
			}
		}
	}

	 /*get the first token from line*/
	tokenComma = strtok(line, comma);

	while(tokenComma != NULL)/*while we cut new arg*/
	{
		/*count num of operands that were found, (src or dst)*/
		operandsCounter++;

		/*put the pointer after spaces*/
		removeSpaces(&tokenComma);

		 /*check if this operand is register*/
		registerFlag = isRegister(tokenComma);

		/*if the start arg is & or # and after this '\0' or ',' or ' ' - error*/
		if ((((*tokenComma) == '&') || ((*tokenComma) == '#')) &&
			  ((*(tokenComma + 1) == '\0') || (*(tokenComma + 1) == ',') || (*(tokenComma + 1) == ' ')))
		{
			strcpy(errorMessage, "The '#' or '&' sign must be before number or label"); /*set error message*/
			printError(LineCounter,errorMessage); /*print error with the function printError()*/
			return -1;
		}

		/*start handling all registers and args*/
		if (numOfOperands == 1 && operandsCounter == 1)/*if this sentence need one argument and now we find the first operand*/
		{
			/*send the operand we found to process and to save data about him in sTempInstructionData*/
			if(setOperandsData(line,tokenComma,registerFlag ,sTempInstructionData, operandsCounter,numOfOperands, LineCounter))
				return -1;/*there is error*/
		}
		else if (numOfOperands == 2 && (operandsCounter == 1 || operandsCounter == 2))/*if this sentence need tow arguments and now we find operand*/
		{
			/*send the operand we found to process and to save data about him in sTempInstructionData*/
			if(setOperandsData(line,tokenComma,registerFlag ,sTempInstructionData, operandsCounter,numOfOperands, LineCounter))
				return -1;/*there is error*/
		}
		else if (operandsCounter > numOfOperands)/*if ew found more operands than need*/
		{
			strcpy(errorMessage, "Unnecessary Operand"); /*set error message*/
			printError(LineCounter, errorMessage); /*print error with the function printError()*/
			return -1;
		}

		inputLine += strlen(tokenComma);/*promote input line after len of operand*/

		if (inputLine[0] != ',' && inputLine[0]!='\0')/*there is not ',' after operand or '\0' if it ended*/
		{
			strcpy(errorMessage, "Missing Comma"); /*set error message*/
			printError(LineCounter,errorMessage); /*print error with the function printError()*/
			return -1;
		}
		else if (inputLine[0] == ',')/*there is ',' - there is operand*/
		{
			inputLine++;/*promote after ','*/
			removeSpaces(&inputLine);

			if (!inputLine)/*thers is not operand after ','*/
			{
				strcpy(errorMessage, "Missing Operand"); /*set error message*/
				printError(LineCounter, errorMessage); /*print error with the function printError()*/
				return -1;
			}
			else/*thers is operand after ','*/
			{
				tokenComma = strtok(inputLine, comma);/*get operand*/
				/*strtok(inputLine, space);*//*remove space*/
				if(tokenComma == NULL)
				{
				tokenComma = strtok(inputLine, "\0");/*get operand*/
				}
			}
		}
		else if (!inputLine[0])/*no more operands*/
		{
			tokenComma = NULL;
		}
	}/*end of while*/
	if((counterCheck+1) > numOfOperands)/* found more operands then we need to this opcode*/
	{
		strcpy(errorMessage, "too much Operands"); /*set error message*/
		printError(LineCounter, errorMessage); /*print error with the function printError()*/
		return -1;
	}

	if(operandsCounter < numOfOperands)/*we not found all operands we need to this opcode*/
	{
		strcpy(errorMessage, "Missing Operand"); /*set error message*/
		printError(LineCounter, errorMessage); /*print error with the function printError()*/
		return -1;
	}
	return 0;
} /*end of function checkOperands*/

/*check how nuch operands we need to currently line according opcode and send it to find them*/
int countL(char** inputLine,int lineCounter, instruction_Node* sTempInstructionData)
{
	/*defining variables*/
	eOpcode opCodeMumber; /*for opcode*/
	int funcReturn = 0; /* -1 if error*/

	opCodeMumber = sTempInstructionData->firstInstructionWord.Actionopcode;

	/* opcode type with no arguments*/
	if(opCodeMumber == opcode_rts || opCodeMumber == opcode_stop)
	{
		funcReturn = checkOperands(*inputLine, 0, lineCounter,sTempInstructionData);/*we need 0 operands*/
	}

	/* opcode type with one arguments*/
	else if (opCodeMumber == opcode_clr || opCodeMumber == opcode_not || opCodeMumber == opcode_inc || opCodeMumber == opcode_dec ||opCodeMumber == opcode_jmp ||
	opCodeMumber == opcode_bne || opCodeMumber == opcode_jsr || opCodeMumber == opcode_red || opCodeMumber == opcode_prn)
	{
		funcReturn = checkOperands(*inputLine, 1,lineCounter, sTempInstructionData);/*we need 1 operand*/
	}

	/* opcode type with tow arguments*/
	else if(opCodeMumber == opcode_mov || opCodeMumber == opcode_cmp || opCodeMumber == opcode_add || opCodeMumber == opcode_sub || opCodeMumber == opcode_lea)
	{
		funcReturn = checkOperands(*inputLine, 2, lineCounter, sTempInstructionData);/*we need 2 operands*/
	}
	return funcReturn;
} /*end of function countL*/

/*checks if there is a label in the line. returns label length, or 0 (if no label found), or -1 if there is a error*/
int isLabel(char *line, int lineCounter)
{
	/*defining and setting variables*/
	int labelLength = 0; /*set label length to 0*/
	int i = 0; /*set index to 0*/

	/*check if there is a label*/
	while (i < MAX_LINE_LENGTH) /*while this char is inside the line length limit*/
	{
		if (line[i] == ':') /*if this char is ':'*/
		{
			labelLength = i; /*set labelLength as the index of the last ':' char*/
			i += MAX_LINE_LENGTH;
		}
		i++; /*promote the index*/
	}

	if ((labelLength) && (!(isspace(line[labelLength+1]))) && (line[labelLength+1] != '\0')) /*if label exists and the char after the : isn't a white space)*/
	{
		strcpy(errorMessage, "There is no white space after the label"); /*set error message*/
		printError(lineCounter,errorMessage); /*print error with the function printError()*/
		return -1; /*return -1 and exit this function*/
	}

	return labelLength; /*return the label length. if there isn't a label, the function returns 0*/
} /*end of function isLabel*/

/*a function that checks errors in the label. returns: 0 if the label is legal, -1 if the label is illigal*/
int isLabelLegal(char *label, int length, int lineCounter)
{
	/*defining variable*/
	int i = 0; /*set index to 0*/

	if (length > MAX_LABEL_LENGTH) /*if the label is too long*/
	{
		strcpy(errorMessage, "The label is too long"); /*set error message*/
		printError(lineCounter,errorMessage); /*print error with the function printError()*/
		return -1; /*return -1 and exit this function*/
	}

	/*gets the name of the label and checks if the it is legal*/
	if (!(isalpha(label[i]))) /*if the label doesn't start with a letter*/
	{
		strcpy(errorMessage, "Illegal start to label"); /*set error message*/
		printError(lineCounter,errorMessage); /*print error with the function printError()*/
		return -1; /*return -1 and exit this function*/
	}

	while (i < length) /*while this char is a part of the label*/
	{
		if (!(isalpha(label[i]))&&(!(isdigit(label[i])))) /*if there is a char in the middle of the label that isn't a letter and isn't a number*/
		{
			strcpy(errorMessage, "Illegal character in label"); /*set error message*/
			printError(lineCounter,errorMessage); /*print error with the function printError()*/
			return -1; /*return -1 and exit this function*/
		}
		i++; /*promote index*/
	}

	/*tests to see if the label is a reserved word*/
	if (isReservedWord(label, lineCounter)) /*if the label is a reserved word*/
	{
		return -1; /*return -1 and exit this function*/
	}
	return 0;
}/*end of function isLabelLegal*/

/*a function that gets a line and returns the label length*/
int getLabelLength(char *line)
{
	/*defining variable*/
	int i = 0; /*index is 0*/

	while ((i < strlen(line)) && (!(isspace(line[i]))) && (line[i] != '\0')) /*while this char is inside the label*/
	{
		i++; /*promote the index*/
	}
	if ((line[i] == '\0') || (i == strlen(line)) || (isspace(line[i])) || (line[i] == ':')) /*if reached end of label*/
	{
		if (i > MAX_LABEL_LENGTH)
		{
			return -1;
		}
		return i; /*return the index of the first char after the label, which is the label length*/
	}
	else /*there is something illegal after the label*/
	{
		return -1;
	}
} /*end of function getLabelLength*/

eData_type checkDotData(char** LineInput)
{
	/*defining variables*/
	int lenCounter = 0;/*tp promote LineInput according len of word we will find*/
	int i = 0;
	char* pLine;

	(*LineInput)++;/*promote after dot*/
	pLine = (*LineInput);

	while((*pLine)!=0) 
	{
		if(isalpha(pLine[i])) /*if this is a valid letter*/
		{
			lenCounter++;
			i++;
		}
		else
		{
			break;
		}
	}

	if(strncmp(pLine, "data", 4) == 0)/*check if its data*/
	{
		(*LineInput) += lenCounter;/*promote LineInput*/
		return data_type;
	}

	if(lenCounter >= 5)
	{
		if(strncmp(pLine, "string", 6) == 0)/*check if its string*/
		{
			(*LineInput) += lenCounter;/*promote LineInput*/
			return string_type;
		}

		if(strncmp(pLine, "entry", 5) == 0)/*check if its entry*/
		{
			(*LineInput) += lenCounter;/*promote LineInput*/
			return entry_type;
		}
	}

	if(lenCounter >= 6)
	{
		if(strncmp(pLine, "extern", 6) == 0)/*check if its extern*/
		{
			(*LineInput) += lenCounter;/*promote LineInput*/
			return extern_type;
		}
	}
	return 0;
} /*end of checkDotData*/

/*this function finds the opcode, promotes inputLine after opcode, saves opcode and funct in stempInstructionStruct*/
eOpcode checkOpCode(char** inputLine,instruction_Node* stempInstructionStruct)
{
	if (strncmp(*inputLine, "mov", 3) == 0) /*if this instruction opCode is "mov"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_mov; /*set opCode*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "cmp", 3) == 0) /*if this instruction opCode is "cmp"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_cmp; /*set opCode*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "add", 3) == 0) /*if this instruction opCode is "add"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_add; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_add; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "sub", 3) == 0) /*if this instruction opCode is "sub"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_sub; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_sub; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "not", 3) == 0) /*if this instruction opCode is "not"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_not; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_not; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "clr", 3) == 0) /*if this instruction opCode is "clr"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_clr; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_clr; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "lea", 3) == 0) /*if this instruction opCode is "lea"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_lea; /*set opCode*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "inc", 3) == 0) /*if this instruction opCode is "inc"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_inc; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_inc; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "dec", 3) == 0) /*if this instruction opCode is "dec"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_dec; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_dec; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "jmp", 3) == 0) /*if this instruction opCode is "jmp"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_jmp; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_jmp; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "bne", 3) == 0) /*if this instruction opCode is "bne"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_bne; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct = funct_bne; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "red", 3) == 0) /*if this instruction opCode is "red"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_red; /*set opCode*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "prn", 3) == 0) /*if this instruction opCode is "prn"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_prn; /*set opCode*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "jsr", 3) == 0) /*if this instruction opCode is "jsr"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_jsr; /*set opCode*/
		stempInstructionStruct->firstInstructionWord.Actionfunct  = funct_jsr; /*set funct*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "rts", 3) == 0) /*if this instruction opCode is "rts"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_rts; /*set opCode*/
		(*inputLine) += 3; /*move pointer forward to after this instruction opCode*/
		return 0;
	}
	else if (strncmp(*inputLine, "stop", 4) == 0) /*if this instruction opCode is "stop"*/
	{
		stempInstructionStruct->firstInstructionWord.Actionopcode = opcode_stop; /*set opCode*/
		(*inputLine) += 4; /*move pointer forward to after this instruction opCode*/
		return 0;
	}

	return opcode_none; /*no opcode type*/
} /*end of function checkOpCode*/

/*the fuction firstProcess. this function reads lines, and makes lists accordingly (using other functions)*/
void firstProcess(instruction_Node** headInstructionList, data_Node** headDataList, symbol_node** headSymbolList,FILE* fAssembly_File,entry_node** entryHeadList, int* ICF, int* DCF)
{
	/*defining variables*/
	int IC = 100; /*to save the length of instructions memory*/
	int DC = 0; /*to save the length of data memory*/
	int lineCounter = 1; /*to print the line of error*/
	int isLineInFile = 1; /*flag is 0 when this is EOF*/
	int sizeOfLabel; /*save the length of label*/
	int functionResult;
	int tempLength;
	struct tempLineData sTempLineData; /*save the temp data in time of process*/
	struct instruction_Node sTempInstructionData; /*save the temp  instruction data in time of process*/
	char *newLine;/*for get line*/
	char *inputLine;/*for process line*/

	/*allocate the pointer newLine*/
	newLine = (char*)calloc((MAX_LINE_LENGTH +1),sizeof(char));
	
	while(isLineInFile >= 0) /*x is used to tell us what value getNextLine returns for tests*/
	{
		/*zeroing the temp struct*/
		sTempLineData.flagLabelExist = 0;
		sTempLineData.typeOfSentence = None_type;
		sTempLineData.typeOfhanchaya = None_type;
		if(sTempLineData.labelName)
		{
			sTempLineData.labelName = NULL;
		}
		/*zeroing the temp instruction struct*/
		sTempInstructionData.firstInstructionWord.Actionfunct = 0;
		sTempInstructionData.firstInstructionWord.Actionopcode = 0;
		sTempInstructionData.firstInstructionWord.destAdressing = 0;
		sTempInstructionData.firstInstructionWord.srcAdressing = 0;
		sTempInstructionData.instruction_adress = 0;
		sTempInstructionData.arg1 = 0;
		sTempInstructionData.labelFlag1 = 0;
		sTempInstructionData.labelName1 = NULL;
		sTempInstructionData.arg2 = 0;
		sTempInstructionData.labelFlag2 = 0;
		sTempInstructionData.labelName2 = NULL;
		sTempInstructionData.instruction_words_counter = 0;
		sTempInstructionData.nextNode = NULL;
		/*zeroing this string*/
		memset(newLine,0,(MAX_LINE_LENGTH+1));

		/*get the line from file*/
		isLineInFile = getNextLine(fAssembly_File, &lineCounter, &newLine);
		/*get the temp pointer to point to newLine*/
		inputLine = newLine;

		if(isLineInFile == 0) /*white line*/
		{
			continue;
		}
		if(isLineInFile == -1) /*end line*/
		{
			break;
		}

		sizeOfLabel = isLabel(inputLine, lineCounter);/*check if there is label and get his length*/

		if(sizeOfLabel == -1) /*if label has an error*/
		{
			continue;
		}
		if(sizeOfLabel == 0) /*if there is no label*/
		{
			sTempLineData.flagLabelExist = 0;
		}
		else /*there is a label*/
		{
			sTempLineData.flagLabelExist = 1;/*on flag of label to add it to symbol list*/
			sTempLineData.labelName = (char*)malloc(sizeof(char)*(MAX_LABEL_LENGTH+1));/*allocate to save the label*/
			if (!(sTempLineData.labelName))
			{
				strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
				printError(lineCounter , errorMessage); /*print error with the function printError()*/
				exit(1);
			}
			strncpy(sTempLineData.labelName,inputLine,sizeOfLabel); /*copy the label*/
			tempLength = sizeOfLabel;
			while (tempLength<=MAX_LABEL_LENGTH)
			{
				sTempLineData.labelName[tempLength] = '\0';
				tempLength++;
			}

			inputLine += sizeOfLabel+1; /*promote inputLine after label*/

			if(isLabelLegal(sTempLineData.labelName, sizeOfLabel, lineCounter) == -1)/*check if the label is legal*/
			{
				continue;
			}
		}

		removeSpaces(&inputLine);

		/*check if this is a dot */
		if ((*inputLine) == '.') /*there is a dot*/
		{
			sTempLineData.typeOfSentence = hanhaya_Sentence;/*save the type of sentence to hanhaya_Sentence*/
			sTempLineData.typeOfhanchaya = checkDotData(&inputLine);/*check if this is data or string or entry or extern */
		}
		else /*there is not a dot*/
		{
			sTempLineData.typeOfSentence = instruction_Sentence; /*save the type of sentence to instruction_Sentence*/
		}

		removeSpaces(&inputLine);

		/*****handle data or string*****/
		/*set data or string in data list, and in symbol list if flag of label is on*/
		if(sTempLineData.typeOfSentence == hanhaya_Sentence && (sTempLineData.typeOfhanchaya == data_type || sTempLineData.typeOfhanchaya == string_type))
		{
			/*if there is label save it to symbol list*/
			if(sTempLineData.flagLabelExist == 1)
			{
				if(addSymbolToList(headSymbolList, sTempLineData.labelName, DC, data_type,None_type) == -1)
				{
					strcpy(errorMessage,"Label was already declared earlier"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					continue;
				}
			}

			/*add data to data list*/
			if(sTempLineData.typeOfhanchaya == data_type)/*if this is data*/
			{
				functionResult = addNodeToDataList(headDataList, inputLine ,DC, data_type,lineCounter);
			}
			else if(sTempLineData.typeOfhanchaya == string_type)/*if this is string*/
			{
				functionResult = addNodeToDataList(headDataList, inputLine,DC, string_type,lineCounter);
			}

			if (functionResult != -1)/*there is not error*/
			{
				DC += functionResult;/*edit num of words of data to DC*/
			}
			continue;
		}

		/*****handle entry or extern*****/
		/*check if the line is entry or extern*/
		if(sTempLineData.typeOfSentence == hanhaya_Sentence && (sTempLineData.typeOfhanchaya == extern_type || sTempLineData.typeOfhanchaya == entry_type))
		{

			if(sTempLineData.flagLabelExist == 1) /*if there is label in the first of line - reset it*/
			{
				sTempLineData.labelName = NULL; /*reset old label name if exist*/
				sTempLineData.flagLabelExist = 0; /*reset old label if exist*/
			}

			/*if it is .extern it will be edit to symbolTable*/
			if(sTempLineData.typeOfhanchaya == extern_type)
			{
				removeSpaces(&inputLine);
				/*find the label after .extern*/
				sizeOfLabel = getLabelLength(inputLine);

				/*no label*/
				if(sizeOfLabel == 0)
				{
					strcpy(errorMessage, "No argument for .extern"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					continue;
				}
				else /*label is legal*/
				{
				sTempLineData.labelName = (char*)malloc(sizeof(char)*(MAX_LABEL_LENGTH+1));/*allocate to save the label*/
				if (!(sTempLineData.labelName))
				{
					strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
					printError(lineCounter , errorMessage); /*print error with the function printError()*/
					exit(1);
				}
				strncpy(sTempLineData.labelName,inputLine,sizeOfLabel); /*copy the label*/
				tempLength = sizeOfLabel;
				while (tempLength<=MAX_LABEL_LENGTH)
				{
					sTempLineData.labelName[tempLength] = '\0';
					tempLength++;
				}
					inputLine += sizeOfLabel+1; /*promote line after label*/

					/*no valid label*/
					if((isLabelLegal(sTempLineData.labelName, sizeOfLabel, lineCounter) == -1) || (sizeOfLabel == -1))
					{
						continue;
					}
					/*this is valid label - edit this extern label to stmbol list*/
					if (addSymbolToList(headSymbolList,sTempLineData.labelName, 0, None_type, extern_type) == -1)
					{
						strcpy(errorMessage,"Label was already declared earlier"); /*set error message*/
						printError(lineCounter, errorMessage); /*print error with the function printError()*/
					}
					continue;
				}
			}

			/*if it is .entry it saved for secondProcess*/
			else if(sTempLineData.typeOfhanchaya == entry_type)
			{
				removeSpaces(&inputLine);

				/*find the label after .entry*/
				sizeOfLabel = getLabelLength(inputLine);

				/*no label*/
				if(sizeOfLabel == 0)
				{
					strcpy(errorMessage, "No argument for .entry"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					continue;
				}
				else
				{
					sTempLineData.labelName = (char*)malloc(sizeof(char)*(MAX_LABEL_LENGTH+1));/*allocate to save the label*/
					if (!(sTempLineData.labelName))
					{
						strcpy(errorMessage, "Problem in allocating memory"); /*set error message*/
						printError(lineCounter , errorMessage); /*print error with the function printError()*/
						exit(1);
					}
					strncpy(sTempLineData.labelName,inputLine,sizeOfLabel); /*copy the label*/
					tempLength = sizeOfLabel;
					while (tempLength<=MAX_LABEL_LENGTH)
					{
						sTempLineData.labelName[tempLength] = '\0';
						tempLength++;
					}
					inputLine += sizeOfLabel+1; /*promote line after label*/

					/*no valid label*/
					if((isLabelLegal(sTempLineData.labelName, sizeOfLabel, lineCounter) == -1) || (sizeOfLabel == -1))
					{
						continue;
					}
					addToEntries(entryHeadList,sTempLineData.labelName);
				}
				continue;
			}
		}

		removeSpaces(&inputLine);
		/*****handle instruction sentence*****/
		if(sTempLineData.typeOfSentence == instruction_Sentence)
		{
			if(sTempLineData.flagLabelExist == 1)/*there is label in first of line = edit it to symbol list*/
			{
				if(addSymbolToList(headSymbolList,sTempLineData.labelName,IC,code_type,None_type) == -1)
				{
					strcpy(errorMessage,"Label was already declared earlier"); /*set error message*/
					printError(lineCounter, errorMessage); /*print error with the function printError()*/
					continue;
				}
			}

			/*set opcode and funct and save it in sTempInstructionData*/
			if(checkOpCode(&inputLine,&sTempInstructionData) == opcode_none)/*invalid opcode or funct*/
			{
				strcpy(errorMessage , "Invalid opCode type"); /*set error message*/
				printError(lineCounter,errorMessage); /*print error with the function printError()*/
				continue;
			}
			removeSpaces(&inputLine);

			if(countL(&inputLine,lineCounter,&sTempInstructionData))/*send instruction to process and find args*/
			{
				continue;/*if something in instruction sentence is failed*/
			}
			/*the instruction is OK*/
			sTempInstructionData.instruction_adress = IC; /*save the adress of instruction line*/
			addToInsructions(headInstructionList,&sTempInstructionData, lineCounter);/*edit to instruction list*/
			IC += sTempInstructionData.instruction_words_counter; /*promote IC according num of words in this instruction sentence*/
		}
		else/*thare is not any sentence type*/
		{
			strcpy(errorMessage,"Invalid sentence"); /*set error message*/
			printError(lineCounter, errorMessage); /*print error with the function printError()*/
			continue;
		}
		if (sTempLineData.labelName) /*if sTempLineData.labelName isn't NULL*/
		{
			free(sTempLineData.labelName); /*free dinamic allocations*/
		}
	}/*end of while*/

	*ICF = IC; /*save the size of instructions memory*/
	*DCF = DC; /*save the size of data memory*/

	/*update adresses of code type labels in symbol list according ICF*/
	updateSymbolTable(*headSymbolList,ICF);
	fclose(fAssembly_File);

	/*free dinamic allocations*/
	if (newLine)
	{
		free(newLine); /*free line pointer*/
	}
	if (sTempLineData.labelName) /*if sTempLineData.labelName isn't NULL*/
	{
		free(sTempLineData.labelName); /*free dinamic allocation*/
	}
}/*end of function firstProcess*/
