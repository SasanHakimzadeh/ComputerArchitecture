#include <stdio.h>
#include <stdlib.h>

void Inverse(double **matrix, double **inverse, int length);
double ** Transpose(double ** matrix, int row, int column);
double ** Multiply(double ** x, double ** y, int row1, int row2, int column1, int column2);
void printMat(double ** matrix, int numRow, int numColumn);
void getXandY(double ** x, double ** y, int att, int num, FILE *file);
void getMatrix(double ** x, int rows, int columns, FILE * file);
void freeMat(double **matrix, int length);

void freeMat(double **matrix, int length)
{
	int i = 0;
	for(i = 0; i < length; i++)
    	free(matrix[i]);
	free(matrix);
}

void printMat(double ** matrix, int numRow, int numColumn){
	int i, j;
	for(i = 0; i < numRow; i++){
		for(j=0; j <numColumn; j++){
			printf("%0.0lf", matrix[i][j]);
		}
		printf("\n");
	}
}

void getXandY(double ** x, double ** y, int att, int num, FILE *file){
	int i, j;
	char curRow[1000];
	char curNum[100];
	int curRowIndex;
	int curNumIndex; 

	for(i=0; i < num; i++)
	{
		curRowIndex = 0;
		fscanf(file, "%s", curRow);
			for(j = 0; j < att + 1; j++){
				if(j == 0){
					x[i][j] = 1;
				}
				else{
				curNumIndex = 0;
				while(curRow[curRowIndex] != ',' && curRow[curRowIndex] != '\0'){
					curNum[curNumIndex] = curRow[curRowIndex];
					curNumIndex++; 
					curRowIndex++;
				}
				
				curNum[curNumIndex] = '\0';
				if(curRow[curRowIndex] == '\0')
				{
					y[i][0] = atof(curNum); 
				}else
				{
					x[i][j] = atof(curNum);			
				}
				curRowIndex++;
			}
		}
	}

}

void getMatrix(double ** x, int rows, int columns, FILE * file){
	int i, j;
	char curRow[1000];
	char curNum[100];
	int curRowIndex;
	int curNumIndex;
	for(i=0; i < rows; i++)
	{
		curRowIndex = 0;
		fscanf(file, "%s", curRow);
			for(j = 0; j < columns; j++){
				if(j == 0){
					x[i][j] = 1;
				}
				else{

				curNumIndex = 0;
				while(curRow[curRowIndex] != ',' && curRow[curRowIndex] != '\0'){
					curNum[curNumIndex] = curRow[curRowIndex];
					curNumIndex++; 
					curRowIndex++;
				}
				
				curNum[curNumIndex] = '\0';
				x[i][j] = atof(curNum);			
				
				curRowIndex++;
			}
		}
	}
}
double ** Multiply(double ** m1, double ** m2, int numRow1, int numRow2, int numCol1, int numCol2){
	int i;
	int j;
	int n;
	double ** prodMatrix =  malloc(sizeof(double) * numRow1);

	for(i = 0; i<numRow1; i++)
		*(prodMatrix + i) =  malloc(sizeof(double)*numCol2);
	

	for(i=0; i<numRow1; i++)
		for(j=0; j<numCol2; j++){
			prodMatrix[i][j] =0;
			for(n=0; n<numCol1; n++)
				prodMatrix[i][j] = prodMatrix[i][j] + m1[i][n]*m2[n][j];
		}

	return prodMatrix;	
}

double ** transpose(double **matrix, int column, int row){
	int i,j;
	double ** transposed = malloc(sizeof(double) * column);

	for (i = 0; i < column; i++)
	{
		*(transposed +i) = malloc(sizeof(double) * row);
	}
	i = row - 1;
	while(i >= 0){
		for (j = 0; j < column; j++)
		{
			transposed[j][i] = matrix[i][j];
		}
		i--;
	}
	return transposed;
}

void Inverse(double **matrix, double **inverse, int length)
{
	int i;
	int j;
	int k;
	double result;
	double constant;
    
    for(i = 0; i < length; i++)
	{
		for(j = 0; j < length; j++)
		{
			if(j == i)
			{
				constant =  matrix[i][j];
				for(k = 0; k < length; k++)
				{
					result = inverse[j][k] / constant;
					matrix[j][k] = matrix[j][k] / constant;
					inverse[j][k] = result;
				}
				
			}
			else if(j < i)
			{
				constant = matrix[i][j];
				for(k = 0; k < length; k++)
				{
					result = inverse[i][k] - (inverse[j][k] * constant);
					matrix[i][k] = matrix[i][k] - (matrix[j][k] * constant);
					inverse[i][k] = result;
				}
			}
		}
	}
	
	for(i = length; i >= 0 ; i--)
	{
		for(j = length; j >= 0; j--)
		{
			if(i < length - 1 && j < length && j > i)
			{
				constant = matrix[i][j];
				for(k = 0; k < length; k++)
				{
					result = inverse[i][k] - (inverse[j][k] * constant);
					matrix[i][k] = matrix[i][k] - (matrix[j][k] * constant);
					inverse[i][k] = result;
				}
			}
		}
	}

    return;
}

int main(int argc, char * argv[])
{
	FILE * file1;
	FILE * file2;
	file1 = fopen(argv[1], "r");
	file2 = fopen(argv[2], "r");
	int att, num, testCount;
	fscanf(file1, "%d", &att);
	att = att+1;
	fscanf(file1, "%d", &num);
	fscanf(file2, "%d", &testCount);
	double ** x = malloc(sizeof(double) * num);
	double ** y = malloc(sizeof(double) * num);
	double **inverse = malloc(sizeof(double) * att);
	double ** test = malloc(sizeof(double) * testCount);
	double ** prodMatrix;
	double ** transposed;
	double **invTimesXtran;
	double **w;
	double **result;

	int i;
	int j;
	
	i = 0;
	while(i < num){
		*(x+i) = malloc(sizeof(double) * att);
		i++;
	}
	i = 0;
	while(i < testCount){
		*(test +i) = malloc(sizeof(double) * att);
		i++;
	}
	i = 0; 
	while(i < num){
		*(y+i) = malloc(sizeof(double));
		i++;
	}
	for(i = 0; i<att; i++) 
	{
    	*(inverse + i) = malloc(sizeof (double) * att);
		for(j = 0; j < att; j++)
		{
			if(i == j)
      			inverse[i][j] = 1;
      		else
      			inverse[i][j] = 0;
		}
    }

	getXandY(x, y, att, num, file1);
	getMatrix(test, testCount, att, file2);
	transposed = transpose(x, att, num);
	prodMatrix = Multiply(transposed,x,att,num, num, att);
	Inverse(prodMatrix,inverse, att);
	invTimesXtran = Multiply(inverse, transposed, att, att, att, num);
	w = Multiply(invTimesXtran, y, att, num, num, 1);
	result = Multiply(test, w, testCount, att, att, 1);
	printMat(result, testCount, 1);

	freeMat(x, att);
	freeMat(y, 1);
	freeMat(transposed, att);
	freeMat(prodMatrix, att);
	freeMat(inverse, att);
	freeMat(invTimesXtran, att);
	freeMat(w, 1);
	freeMat(result, 1);
	
	return 0;
	}


