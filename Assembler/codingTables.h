#ifndef CODING_TABLE_H
#define CODING_TABLE_H

/*including standard libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*defining macros*/
#define	MAX_LINE_LENGTH 	80
#define	MAX_FILE_NAME_LENGTH	25
#define	MAX_LABEL_LENGTH	31
#define MAX_ERROR_LENGTH	100

/*extern declarations of variables*/
extern int errorsFlag;
extern char errorMessage[MAX_ERROR_LENGTH];
extern char *currentFileName;
extern int newFile;

/*defining eOpcode enum*/
typedef enum eOpcode{
	opcode_mov = 0,
	opcode_cmp = 1,
	opcode_add = 2,
	opcode_sub = 2,
	opcode_lea = 4,
	opcode_clr = 5,
	opcode_inc = 5,
	opcode_not = 5,
	opcode_dec = 5,
	opcode_jmp = 9,
	opcode_bne = 9,
	opcode_jsr = 9,
	opcode_red = 12,
	opcode_prn = 13,
	opcode_rts = 14,
	opcode_stop = 15,
	opcode_none = 16
} eOpcode;

/*defining efunct enum*/
typedef enum efunct {
	funct_add = 10,
	funct_sub = 11,
	funct_clr = 10,
	funct_not = 11,
	funct_inc = 12,
	funct_dec = 13,
	funct_jmp = 10,
	funct_bne = 11,
	funct_jsr = 12
} efunct;

/*defining eOger_num enum*/
typedef enum eOger_num {
	oger_r0 = 0,
	oger_r1 = 1,
	oger_r2 = 2,
	oger_r3 = 3,
	oger_r4 = 4,
	oger_r5 = 5,
	oger_r6 = 6,
	oger_r7 = 7
} eOger_num;

/*defining eAddress_types enum*/
typedef enum eAddress_types {
	Immediate_address = 0,
	Direct_address = 1,
	Relative_address = 2,
	Register_Direct_address = 3
} eAddress_types;

/*defining eData_type enum*/
typedef enum eData_type {
	None_type = 0,
	data_type = 1,
	string_type = 2,
	entry_type = 3,
	extern_type = 4,
	code_type = 5
} eData_type;

/*defining esentence_type enum*/
typedef enum eSentence_Type {
	instruction_Sentence = 0,
	hanhaya_Sentence = 1,
	empty_Sentence = 2,
	comment_Sentence = 3
} esentence_type;

#endif
