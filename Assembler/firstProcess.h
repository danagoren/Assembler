#include "processInputUtils.h"

/*defining a struct that is used to save and process data in the time of processing*/
typedef struct tempLineData{
	eData_type	typeOfhanchaya;
	int		flagLabelExist;
	char*		labelName;
	esentence_type	typeOfSentence;
	eOpcode		opcode;
	efunct		funct;
} tempLineData;

/*function declarations*/
void firstProcess(instruction_Node** headInstructionList, data_Node** headDataList, symbol_node** headSymbolList,FILE* fAssembly_File,entry_node** entryHeadList, int* ICF, int* IDF);
int findLabel(char *line, int lineCounter);
int isReservedWord(char *word, int lineCounter);
int isLabel(char *line, int lineCounter);
int isLabelLegal(char *label, int length, int lineCounter);
int getLabelLength(char *line);
int countL(char** inputLine,int lineCounter, instruction_Node* sTempInstructionData);
int isRegister(char* inputToken);
int setOperandsData(char* line,char* token, int registerFlag,instruction_Node* sTempInstructionData, int operandsCounter,int numReqOperands,int lineCounter);
int checkOperands(char* inputLine,int numOfOperands,int LineCounter,instruction_Node* sTempInstructionData);
eData_type checkDotData(char** LineInput);
eOpcode checkOpCode(char** inputLine,instruction_Node* stempInstructionStruct);
