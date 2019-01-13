#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *next;
};

struct node *insertNode(struct node *front, int num);
struct node *deleteNode (struct node *front, int num);

int main(int argc, char *argv[])
{
    FILE *fptr = NULL;

    if (argc != 2)
    {
        printf("not enough arguments\n");
        return 1;
    }
    if ((fptr = fopen(argv[1], "r")) == NULL)
    {
        printf("error - nothing read in file\n");
        return 1;
    }
 
    struct node *front = NULL;
    int num;
    char action;
    while (fscanf(fptr, "%c %d", &action, &num) != EOF)
    {
        if (action == 'd')
        {
            if(front == NULL){
                continue;
            }
            if(front->data == num){
                front = deleteNode(front, num);
            }
            else{
                deleteNode(front, num);
            }
        }
        else if (action == 'i') 
        {
                if(front == NULL)
                {
                    struct node *newNode = (struct node *)malloc(sizeof(struct node));
                    newNode->data = num;
                    newNode->next = NULL;
                    front = newNode;
                }
                else if(front->data > num)
                {
                    struct node *newNode = (struct node *)malloc(sizeof(struct node));
                    newNode->data = num;
                    newNode->next = front;
                    front = newNode;
                }
                else
                    front = insertNode(front, num);
        }
    } 
        
    struct node *counter = front;
    int listLength = 0;
    while (counter != NULL){
        counter = counter->next;
        listLength++;
    }
    printf("%d\n", listLength);

    fclose(fptr);

    struct node *printer;
    if (front != NULL)
    {
        printf("%d", front->data);
        printer = front->next;
    }
    else{
        printer = front;
    }

    while (printer != NULL)
    {
        printf("\t%d", printer->data);
        printer = printer->next;
    }

    if (front != NULL) {
        printf("\n");
    }

    return 0;
}

struct node *insertNode(struct node *front, int num)
{
    if(front == NULL)
    {
        struct node *newNode = (struct node *)malloc(sizeof(struct node));
        newNode->data = num;
        newNode->next = NULL;
        return newNode;
    }
    if(front->data > num)
    {
        struct node *nextNode = front->next;
        if(nextNode == NULL)
        {
            if(front->data < num)
            {
                struct node *newNode = (struct node *)malloc(sizeof(struct node));
                newNode->data = num;
                newNode->next = NULL;
                front->next = newNode;
            }
            else
            {
                    struct node *newNode = (struct node *)malloc(sizeof(struct node));
                    newNode->data = num;
                    newNode->next = NULL;
                    newNode->next = front;
                    front = newNode;
            }
        }
        else if(nextNode->data > num)
        {
                struct node *newNode = (struct node *)malloc(sizeof(struct node));
                newNode->data = num;
                newNode->next = NULL;
                front->next = newNode;
                newNode->next = nextNode;
        }
    }
    else if(front->data < num)
    {
        front->next = insertNode(front->next, num);
    }
    else if(front->data == num)
    {
        return front;
    }
    return front;
}

struct node *deleteNode (struct node *front, int num)
{
    struct node *cur;
    struct node *prev = NULL;

    for (cur = front; cur != NULL; cur = cur->next)
    {
        if (num == cur->data)
        {
                break;
        }
        if (num < cur->data)
        {
            return front;
        }
        prev = cur;
    }
    if (cur != NULL) 
    {
            if (prev == NULL) 
            {
                front = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }
    }
    
    return front;
}

