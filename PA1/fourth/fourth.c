#include <stdio.h>
#include <stdlib.h>
int * multiply(int* first, int* second, int r1, int c1, int r2, int c2);
int * readMatrix(FILE * fileptr, int rows, int cols);
void printMatrix(int * matrix,int row, int col);

int main(int argc, char * argv[])
{
    FILE *fileptr = NULL;
    int *first;
    int *second;
    int row1, col1, row2, col2;
    
    if (argc != 2)
    {
        printf("not enough arguments\n");
        return 1;
    }
    if ((fileptr = fopen(argv[1], "r")) == NULL)
    {
        printf("error - nothing read in");
        return 1;
    }    
    
    fscanf(fileptr, "%d %d", &row1, &col1);

    first = readMatrix(fileptr, row1, col1);
    
    fscanf(fileptr, "%d %d", &row2, &col2);

    second = readMatrix(fileptr, row2, col2);

    if (col1 != row2) 
    {
        printf("bad-matrices");
        return 0;
    }

    int * product = multiply(first, second, row1, col1, row2, col2);
    
    printMatrix(product, row1, col2);

    free(first);
    free(second);
    free(product);
    return 0;
}

int * readMatrix(FILE * fileptr, int rows, int cols)
{
    int * newMatrix = (int *)malloc(rows * cols * sizeof(int));
    int i;
    int j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            fscanf(fileptr, "%d", (newMatrix + i*cols +j));
        }
    }
    return newMatrix;
}
int * multiply(int* first, int* second, int row1, int col1, int row2, int col2)
{
    int i, j, k;
    int * newMatrix;
    newMatrix = (int *)malloc(row1 * col2 * sizeof(int));

    for (i = 0; i < row1; i++)
    {
        for (j = 0; j < col2; j++)
        {
            for (k = 0; k < col1; k++)
            {
               *(newMatrix + i*col2+ j) += *(first + i*col1 +k) * *(second + k*col2 +j);
            }
        }
    }

    return newMatrix;
}

void printMatrix(int * matrix, int row, int col)
{
    int i;
    int j;
    for (i = 0; i < row; i++)
        {
            for (j = 0; j < col; j++)
            {
                printf("%d\t", *(matrix + i*col +j));
            }
            printf("\n");
        }
}

