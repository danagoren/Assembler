OBJS	= errorsHandler.o firstProcess.o main.o secondProcess.o  symbolTable.o instructionList.o dataList.o processInputUtils.o
SOURCE	= errorsHandler.c firstProcess.c main.c secondProcess.c  symbolTable.c instructionList.c dataList.c processInputUtils.c
HEADER	= errorsHandler.h  firstProcess.h main.h secondProcess.h symbolTable.h instructionList.h dataList.h processInputUtils.h
OUT	= assembler
CC	 = gcc
FLAGS	 = -g -c -Wall -ansi -pedantic
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

errorsHandler.o: errorsHandler.c
	$(CC) $(FLAGS) errorsHandler.c 

firstProcess.o: firstProcess.c
	$(CC) $(FLAGS) firstProcess.c 
	
symbolTable.o: symbolTable.c
	$(CC) $(FLAGS) symbolTable.c 

main.o: main.c
	$(CC) $(FLAGS) main.c 

secondProcess.o: secondProcess.c
	$(CC) $(FLAGS) secondProcess.c 

dataList.o: dataList.c
	$(CC) $(FLAGS) dataList.c 
	
instructionList.o: instructionList.c
	$(CC) $(FLAGS) instructionList.c 
	
processInputUtils.o: processInputUtils.c
	$(CC) $(FLAGS) processInputUtils.c 


clean:
	rm -f $(OBJS) $(OUT)
