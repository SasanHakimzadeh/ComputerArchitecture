#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "second.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr,"error\n");
		exit(0);
	}
	
	trueVar = malloc(sizeof(variable));
	trueVar->index = -1;
	trueVar->value = 1;
	trueVar->defined = TRUE;
	strcpy(trueVar->name, "1");

	falseVar = malloc(sizeof(variable));
	falseVar->index = -2;
	falseVar->value = 0;
	falseVar->defined = TRUE;
	strcpy(falseVar->name, "0");


	FILE* file = fopen(argv[1], "r");
	if (file == NULL) 
	{
		fprintf(stderr, "error\n");
		exit(1);
	}
	else 
	{
		char line[1024];
		while (fgets(line, 1024, file) != NULL) 
			addToCircuit(line);
	}
	fclose(file);

	doCircuit();

	return 0;
}

void circuitSort(){
	int i = 0;
	int j;
	gate* curGate;
	gate* switchGate;
	int varDefineAtGate;
	variable* currentVar;
	while(i < numVariables)
	{
		currentVar = getVariable(i);
		if(currentVar->isValidInput == FALSE)
			currentVar->defined = FALSE;
		else
			currentVar->defined = TRUE;
		i++;
	}
	i = 0;
	while(i < numGates)
	{
		curGate = getGate(i);
		if(strcmp(curGate->type, "MULTIPLEXER") == 0)
		{
			for(j = 0; j < curGate->inputNum + logBaseTwo(curGate->inputNum) ; j++)
			{
				currentVar = getVariable(*(curGate->indices + j));
				if(currentVar->defined == FALSE)
				{
					varDefineAtGate = getvarDefineAtGate(currentVar);
					switchGate = getGate(varDefineAtGate);
					switchGate->index = curGate->index;
					curGate->index = varDefineAtGate;
					circuitSort();
					return;
				}
			}
			currentVar = getVariable(*(curGate->indices + (curGate->inputNum + logBaseTwo(curGate->inputNum))));
			currentVar->defined = TRUE;
		}
		else if(strcmp(curGate->type, "DECODER") == 0)
		{
			for(j = 0; j < curGate->inputNum; j++)
			{
				currentVar = getVariable(*(curGate->indices + j));
				if(currentVar->defined == FALSE)
				{
					varDefineAtGate = getvarDefineAtGate(currentVar);
					switchGate = getGate(varDefineAtGate);
					switchGate->index = curGate->index;
					curGate->index = varDefineAtGate;
					circuitSort();
					return;
				}
			}
			for(j = 0; j < powerOfTwo(curGate->inputNum); j++)
			{
					currentVar = getVariable(*(curGate->indices +curGate-> inputNum + j));
					currentVar->defined = TRUE;
			}
		}
		else if(strcmp(curGate->type, "NOT") == 0)
		{
			currentVar = getVariable(*(curGate->indices));
			if(currentVar->defined == FALSE)
			{
					varDefineAtGate = getvarDefineAtGate(currentVar);
					switchGate = getGate(varDefineAtGate);
					switchGate->index = curGate->index;
					curGate->index = varDefineAtGate;
					circuitSort();
					return;
			}
			currentVar = getVariable(*(curGate->indices + 1));
			currentVar->defined = TRUE;
		}
		else if(strcmp(curGate->type, "OR") == 0 ||  strcmp(curGate->type, "AND") == 0|| strcmp(curGate->type, "NAND") == 0 || strcmp(curGate->type, "NOR") == 0 || strcmp(curGate->type, "XOR") == 0)
		{
			currentVar = getVariable(*(curGate->indices));
			if(currentVar->defined == FALSE)
			{
					varDefineAtGate = getvarDefineAtGate(currentVar);
					switchGate = getGate(varDefineAtGate);
					switchGate->index = curGate->index;
					curGate->index = varDefineAtGate;
					circuitSort();
					return;
			}
			currentVar = getVariable(*(curGate->indices + 1));
			if(currentVar->defined == FALSE)
			{
					varDefineAtGate = getvarDefineAtGate(currentVar);
					switchGate = getGate(varDefineAtGate);
					switchGate->index = curGate->index;
					curGate->index = varDefineAtGate;
					circuitSort();
					return;
			}
			currentVar = getVariable(*(curGate->indices + 2));
			currentVar->defined = TRUE;
		}
		i++;
	}
}

int getvarDefineAtGate(variable* undefined){
	int i = 0;
	int j;
	gate* curGate;
	variable* currentVar;
	while(i < numGates)
	{
		curGate = getGate(i);
		if(strcmp(curGate->type, "MULTIPLEXER") == 0)
		{
			currentVar = getVariable(*(curGate->indices + (curGate->inputNum + logBaseTwo(curGate->inputNum))));
			if(currentVar->index == undefined->index)
				break;
		}
		else if(strcmp(curGate->type, "DECODER") == 0)
		{
			int found = FALSE;
			for(j = 0; j < powerOfTwo(curGate->inputNum); j++)
			{
					currentVar = getVariable(*(curGate->indices +curGate-> inputNum + j));
					if(currentVar->index == undefined->index)
						found = TRUE;
			}
			if(found == TRUE)
				break;
		}
		else if(strcmp(curGate->type, "NOT") == 0)
		{
			currentVar = getVariable(*(curGate->indices + 1));
			if(currentVar->index == undefined->index)
				break;
		}
		else if(strcmp(curGate->type, "OR") == 0 ||  strcmp(curGate->type, "AND") == 0|| strcmp(curGate->type, "NAND") == 0 || strcmp(curGate->type, "NOR") == 0 || strcmp(curGate->type, "XOR") == 0)
		{
			currentVar = getVariable(*(curGate->indices + 2));
			if(currentVar->index == undefined->index)
				break;
		}
		i++;
	}
	return curGate->index;
}


variable* addVariable(char name[]){
	variable* newVariable = malloc(sizeof(variable));
	newVariable->index = numVariables;
	strcpy(newVariable->name, name);
	newVariable->next = varList;
	varList = newVariable;
	numVariables++;
	return newVariable;
}

void addInitialVariables(char args[], int isValidInput){
	int argsIndex = 0;
	char numVarString[50];
	while(args[argsIndex] != ' ')
	{
		numVarString[argsIndex] = args[argsIndex];
		argsIndex++;
	}
	numVarString[argsIndex] = '\0'; 
	if(isValidInput == TRUE)
		inputNum = atoi(numVarString);
	else
		outputNum = atoi(numVarString);
	char currentVar[100];
	int varIndex;
	while(args[argsIndex] != '\0')
	{
		varIndex = 0;
		for(argsIndex++; args[argsIndex] != ' ' && args[argsIndex] != '\0'; argsIndex++, varIndex++)
			currentVar[varIndex] = args[argsIndex];
		currentVar[varIndex] = '\0';
		variable* newVariable = addVariable(currentVar);
		newVariable->isValidInput = isValidInput;
	}
}

void addGate(char gateType[], char args[]){
	gate* newGate = malloc(sizeof(gate));
	strcpy(newGate->type, gateType);
	newGate->index = numGates;
	newGate->next = gateList;
	gateList = newGate;
	numGates++;

	char arg[100];
	int argsIndex = 0;
	int argIndex;
	if(strcmp(gateType, "NOT") == 0)
	{
		newGate->inputNum = 1;
		newGate->indices = malloc(sizeof(int) * 2);
	}		
	else if(strcmp(gateType, "DECODER") == 0 || strcmp(gateType, "MULTIPLEXER") == 0)
	{
		for(argIndex = 0; args[argsIndex] != ' '; argsIndex++, argIndex++)
			arg[argIndex] = args[argsIndex];
		arg[argIndex] = '\0';
		argsIndex++;
		newGate->inputNum = atoi(arg);
		if(strcmp(gateType, "DECODER") == 0 )
			newGate->indices = malloc(sizeof(int *) * (newGate->inputNum + powerOfTwo(newGate->inputNum)));
		else
			newGate->indices = malloc(sizeof(int *) * (2 + newGate->inputNum + logBaseTwo( newGate->inputNum)));	
	}
	else
	{
		newGate->inputNum = 2;
		newGate->indices = malloc(sizeof(int) * 3);
	}
	int varIndex = 0;
	while(args[argsIndex] != '\0')
	{
		for(argIndex = 0; args[argsIndex] != ' ' && args[argsIndex] != '\0'; argsIndex++, argIndex++)
			arg[argIndex] = args[argsIndex];
		arg[argIndex] = '\0';
		if(args[argsIndex] != '\0')
			argsIndex++;
		if(!isdigit(arg[0]))
		{
			variable* currentVar = getVarName(arg);
			if(currentVar == NULL)
				currentVar = addVariable(arg);
			*(newGate->indices + varIndex) = currentVar->index;
			varIndex++;
		}
		else
		{
			if(strcmp(arg, "1") == 0)
				*(newGate->indices + varIndex) = -1;
			else
				*(newGate->indices + varIndex) = -2; 
			varIndex++;
		}
	}
}

void addToCircuit(char line[]) {
	char command[100];
	int lineIndex = 0;
	while(line[lineIndex] != ' ')
	{
		command[lineIndex] = line[lineIndex];
		lineIndex++;
	}
	command[lineIndex] = '\0';

	lineIndex++;
	char args[1024];
	int argIndex = 0;
	while(line[lineIndex] != '\n')
	{
		args[argIndex] = line[lineIndex];
		lineIndex++;
		argIndex++;
	}
	args[argIndex] = '\0';

	if(strcmp(command, "INPUTVAR") == 0)
		addInitialVariables(args, TRUE);
	else if (strcmp(command, "OUTPUTVAR") == 0)
		addInitialVariables(args, FALSE);
	else
		addGate(command, args);
}

void doCircuit(){
	int x = 0;
	int y;
	gate* curGate;
	variable* currentVar;
	circuitSort();
	while(x < powerOfTwo(inputNum))
	{
		y = 0;
		initialInputs(x);
		while(y < numGates)
		{
			curGate = getGate(y);
			int val1;
			int val2;
			if(strcmp(curGate->type, "NOT") == 0)
			{
				currentVar = getVariable(*(curGate->indices));
				val1 = currentVar->value;
				currentVar = getVariable(*(curGate->indices + 1));
				if(val1 == 0)
					currentVar->value = 1;
				else
					currentVar->value = 0;
			}
			if(strcmp(curGate->type, "AND") == 0 || strcmp(curGate->type, "OR") == 0 || strcmp(curGate->type, "NAND") == 0 || strcmp(curGate->type, "NOR") == 0 || strcmp(curGate->type, "XOR") == 0)
			{
				currentVar = getVariable(*(curGate->indices));
				val1 = currentVar->value;
				currentVar = getVariable(*(curGate->indices + 1));
				val2 = currentVar->value;
				currentVar = getVariable(*(curGate->indices + 2));
			}
			if(strcmp(curGate->type, "AND") == 0)
			{
				if(val1 == 1 && val2 == 1)
					currentVar->value = 1;
				else
					currentVar->value = 0;
			}
			else if(strcmp(curGate->type, "NAND") == 0)
			{
				if(val1 == 1 && val2 == 1)
					currentVar->value = 0;
				else
					currentVar->value = 1;
			}	
			else if(strcmp(curGate->type, "OR") == 0)
			{
				if(val1 == 1 || val2 == 1)
					currentVar->value = 1;
				else
					currentVar->value = 0;
			}
			if(strcmp(curGate->type, "XOR") == 0)
			{
				if(val1 != val2)
					currentVar->value = 1;
				else
					currentVar->value = 0;
			}
			else if(strcmp(curGate->type, "NOR") == 0)
			{
				if(val1 == 1 || val2 == 1)
					currentVar->value = 0;
				else
					currentVar->value = 1;
			}
			else if(strcmp(curGate->type, "DECODER") == 0)
			{
				int binaryExponent = 0;
				int output = 0;
				int i = curGate->inputNum - 1;
				while(i >= 0)
				{
					currentVar = getVariable(*(curGate->indices + i));
					if(currentVar->value == 1)
						output = output + powerOfTwo(binaryExponent);
					binaryExponent++;
					i--;
				}
				i = 0;
				while(i < powerOfTwo(curGate->inputNum))
				{
						currentVar = getVariable(*(curGate->indices +curGate-> inputNum + i));
						if(i == translateToGrayCode(output))
							currentVar->value = 1;
						else
							currentVar->value = 0;
						i++;
				}
			}
			else if(strcmp(curGate->type, "MULTIPLEXER") == 0)
			{
				int output = 0;
				int binaryExponent = 0;
				
				int i;
				for(i = curGate->inputNum + logBaseTwo(curGate->inputNum) - 1; i >= curGate->inputNum; i--)
				{
					currentVar = getVariable(*(curGate->indices + i));
					if(currentVar->value == 1)
						output = output + powerOfTwo(binaryExponent);
					binaryExponent++;
				}
				currentVar = getVariable(*(curGate->indices + translateToGrayCode(output)));
				variable* outputVar = getVariable(*(curGate->indices + (curGate->inputNum + logBaseTwo(curGate->inputNum))));
				outputVar->value = currentVar->value;
			}
			y++;
		}
		int i = 0;
		while(i < inputNum + outputNum)
		{
			currentVar = getVariable(i);
			if(i == inputNum + outputNum - 1)
				printf("%d\n", currentVar->value);
			else
				printf("%d ", currentVar->value);
			i++;
		}
		x++;
	}
}

int translateToGrayCode(int num){
	int translateToGrayCode = 0;
	while(num > 0)
	{
		translateToGrayCode  ^= num;
		num = num >> 1;
	}
	return translateToGrayCode;
}

void initialInputs(int sequenceNum){
	int translateToGrayCode = sequenceNum ^ (sequenceNum >> 1);
	int binaryNum[1000];

	int i = inputNum - 1 ;
	while(i >= 0)
	{
		binaryNum[i] = 0; 
		i--;
	}
	for(i = 0; translateToGrayCode > 0; i++)
	{
		binaryNum[i] = translateToGrayCode % 2;
		translateToGrayCode = translateToGrayCode / 2;
	}
	
	i = inputNum - 1;
	int inputIndex = 0;
	variable* currentVar;
	while(i >= 0)
	{
		currentVar = getVariable(inputIndex);
		currentVar->value = binaryNum[i];
		inputIndex++;
		i--;
	}
}

variable* getVarName(char name[100]){
	variable* currentVar = varList;
	while(currentVar != NULL)
	{
		if(strcmp(currentVar->name, name) == 0)
			break;
		currentVar = currentVar->next;
	}
	return currentVar;
}

variable* getVariable(int index){
	if(index == -2)
		return falseVar;
	if(index == -1)
		return trueVar;
	variable* currentVar = varList;
	while(currentVar != NULL)
	{
		if(currentVar->index == index)
			break;
		currentVar = currentVar->next;
	}
	return currentVar;
}

gate* getGate(int index){
	gate* curGate = gateList;
	while(curGate != NULL)
	{
		if(curGate->index == index)
			break;
		curGate = curGate->next;
	}
	return curGate;
}

int logBaseTwo(int input) {
	int index = 0;
	while (index < 31)
	 {
		if ((input >> index) == 1)	
			break;
		index++;
	}
	return index;
}

int powerOfTwo(int exponent)  {
    int i = 0;
    int result = 1;
    while (i < exponent) 
	{
        result *= 2;
		i++;
	}
    return result;
}
