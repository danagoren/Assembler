#ifndef PROCESS_INPUT_UTILS_H
#define PROCESS_INPUT_UTILS_H

#include "symbolTable.h"

/*function declarations*/
void removeSpaces(char** pLine);
int getNextLine(FILE *fAssembly_File, int *lineCountPointer, char **newLine);

#endif
