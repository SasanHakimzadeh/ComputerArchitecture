#include <stdlib.h>

#include <stdio.h>

//here are all of my function prototypes

void sort (int arr[], int dim);
void evenOddSplit(int arr[], int dim);
void swap(int arr[], int a, int b);

int main(int argc, char ** argv){

    int arr[200]; 
    
    FILE *fileptr;
    fileptr = fopen(argv[1], "r");
    int dim = 0;
    //scan the file and get the dimension of the array, which should be the first number in the file
    fscanf(fileptr, "%d", &dim);

    //scan the file and put values into the array arr
    int  i=0;
    for (i = 0; i < dim; i++)

    {
        fscanf(fileptr, "%d", &arr[i]);
    }
    fclose(fileptr);


     int minIndex, j; 
    for (i = 0; i < dim-1; i++) 
    { 
        minIndex = i; 
        for (j = i+1; j < dim; j++) {
          if (arr[j] < arr[minIndex]) 
          {
            minIndex = j; 
        }
        }
        
        swap(arr, minIndex, i);
    } 

    evenOddSplit(arr, dim);

    for (i = 0; i < dim; i++)
    {
        printf("%d\t", arr[i]);
    }
    return 0;
}

void swap(int arr[], int a, int b){
        int temp = arr[a];
        arr[a] = arr[b];
        arr[b] = temp;
}
    
    void evenOddSplit(int arr[], int dim)
    {

    int i, l = 0, r = 0;

    int numEvens = 0;

    int tempArray[200];

    for (i = 0; i < dim; i++)
    {
        if (arr[i] % 2 == 0)

            numEvens++;
    }
    r = numEvens;
    for (i = 0; i < dim; i++)
    {
        if (arr[i] % 2 == 0)
        {
            tempArray[l] = arr[i];
            l++;
        }
        else
        {
            tempArray[r] = arr[i];

            r++;
        }
    }
    for (i = 0; i < dim; i++)

    {
        arr[i] = tempArray[i];
    }
    }


