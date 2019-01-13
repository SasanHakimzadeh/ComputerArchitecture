#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fileptr;
    //this reads the file from the user input which is the filename
    fileptr = fopen(argv[1], "r");
  
    int **matrix;
    int n, i, j ;
    //This line gets the first number which is the size of the nxn matrix
   fscanf(fileptr,"%d",&n);
  
   //This dynamically allocates memory for the 2d array based on the size n
    matrix = malloc (sizeof (int *) * n);

    i=0;

    while(i<n) 
   {
    *(matrix + i) = malloc (sizeof (int) * n);
    i++;
   }
  
   //takes values from file and puts them in the array
   for(i=0;i<n;i++){
       for(j=0;j<n;j++){
           fscanf(fileptr,"%d",&matrix[i][j]);
          }
      }


int firstcolumn=0;
   for (j = 0; j < n; j++) 
   {
         firstcolumn +=  matrix[j][0];
      }

int firstrow = 0;
   for(j=0;j<n;j++)
   {
    firstrow += matrix[0][j];
   }

int determinant=0;
for(i = 0; i<n; i++){
    for(j = 0; j<n;j++){
      int temporary = matrix[i][j];
	  int k;
      for(k=j+1; k<n; k++){
        if (temporary == matrix[i][k]){
          determinant =1;
        }
      }
    }

  }

   //sums diagonal elements to get the diagonal sum
   int diagonal1 = 0;
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         if (i == j)
            diagonal1 += matrix[i][j];
      }
   }
   //printf("%d\n", diagonalsum);

   int diagonal2 = 0;
   for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
         if (j == n-i-1)
            diagonal2 += matrix[i][j];
      }
   }
   //printf("%d\n", diagonalsum2 );

    //This checks that the columns all have the same sums 
   int column;
   for (i = 0; i < n; i++) {
       column = 0;
      for (j = 0; j < n; j++) {
         column +=  matrix[j][i];
      }
      if(column != firstcolumn){
		determinant = 1;
		}
      
   }

   // This checks that the rows all have the same sum
   int row;
   for (i = 0; i < n; i++) {
      row = 0;
      for (j = 0; j < n; j++) {
         row += matrix[i][j];
      }
      if(firstrow != row){
		determinant = 1;
	}
   }

   
/*printf("%d\n", rowsum);
printf("%d\n", columnsum );
printf("%d\n", diagonalsum2 );
printf("%d\n", diagonalsum);*/

   if (determinant != 1 && diagonal1 == diagonal2 && diagonal2==column && column==row){
    //printf("\n");
    printf("magic\n");
  }
   else 
      printf("not-magic\n");
   


  //free the memory that we malloced for the array 
   for (i = 0; i < n; i++)
   {
       free (matrix[i]);
   }
   free (matrix);  
   return 0;
  
}

