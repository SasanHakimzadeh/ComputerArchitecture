#include <stdio.h>
#include <stdlib.h>

struct Node 
{ 
    int data; 
    struct Node * next;
};

struct Node *array[10000];

struct Node * search(int num);
int hashFunction(int num);
void insert(int num);

int main(int argc, char** argv){
    FILE *fileptr;
    
     if (argc != 2)
    {
        printf("not enough arguments\n");
        return 1;
    }
    if ((fileptr = fopen(argv[1], "r")) == NULL)
    {
        printf("error - file read nothing\n");
        return 1;
    }

    
    struct Node * node;
    char action;
    int num;
    while(fscanf(fileptr, "%c\t%i\n", &action, &num)!=EOF)
    {
        node = search(num);
        if(action == 'i')
        {
            if(node == NULL)
            {
				insert(num); 
                printf("inserted\n");
            }
            else{
                
                printf("duplicate\n");
            }
        }
        else
        {
            if(node != NULL){
                printf("present\n");
            }
            else{
                printf("absent\n");
            }
                
        }
    } 
    return 0;
}

struct Node * search(int num) { 
    int hashIndex = hashFunction(num);
    struct Node * node;
    for(node = array[hashIndex]; node != NULL; node = node->next)
    {
        if(node->data == num)
            return node;
    }
    return NULL;
} 

int hashFunction(int num){
    if (0 > num ){
        num = num / -1;
    }
    num = num % 10000;
    return num;
}

void insert(int num){ 
    struct Node * newNode =  (struct Node *) malloc(sizeof(struct Node)); 
    newNode->data = num; 
    newNode->next = NULL;
    int hashIndex = hashFunction(num);
    newNode->next = array[hashIndex];
    array[hashIndex] = newNode;
} 

