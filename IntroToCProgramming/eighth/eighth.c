#include <stdio.h>
#include <stdlib.h>

struct Node 
{ 
    int data; 
    struct Node * left;
    struct Node * right;
};

struct Node * Insert(int data, struct Node * node);
struct Node * GetNode(struct Node * node,int data);
int NodeHeight(int data, struct Node * node);
void FreeNodes(struct Node * node);
void printtree(struct Node * node);

int main(int argc, char** argv)
{
    FILE *fptr;

    if (argc != 2)
    {
        printf("not enough arguments\n");
        return 1;
    }
    if ((fptr = fopen(argv[1], "r")) == NULL)
    {
        printf("error - nothing read in\n");
        return 1;
    }
	char action;
    int num, height;
    struct Node *root= NULL;
    struct Node *curr = NULL;

    while(fscanf(fptr, "%c\t%i\n", &action, &num) != -1)
    {
        if(action == 's')
        {
            curr = GetNode(root, num);
            if(curr == NULL)
            {
                printf("absent\n");
            }
            else
            {
				height = NodeHeight(num, root);
                printf("present %d\n", height);
                
            }
        }
        else if(action == 'i')
        {
            curr = GetNode(root, num);
            if(curr != NULL)
            {
                printf("duplicate\n");
            }
            else 
            {
                curr = Insert(num, root);
                if(root == NULL)
                {
                    root = curr;
                }
                height = NodeHeight(num, root);
                printf("inserted %d\n", height);
            }
        }
        //printtree(root);
    }
    free(curr);
    return 0;
}

struct Node * Insert(int data, struct Node * node)
{ 
    if (node == NULL) 
    {
        struct Node * newNode =  (struct Node *) malloc(sizeof(struct Node)); 
        newNode->right = NULL;
        newNode->left = NULL; 
        newNode->data = data;
        return newNode;
    }
    else if (data > node->data) 
    {
        node->right = Insert(data, node->right);  
        return node; 
    }
    else if (data < node->data) 
    {
         node->left = Insert(data, node->left); 
         return node; 
    }
    return node;
}

struct Node * GetNode(struct Node * node,int data) 
{ 
    if(node == NULL)
    {
        return node;
    }
    while(node->data != data)
    {
        if(node->data < data)
        {
            node = node->right;
        }
        else if(node->data > data)
        {
            node = node->left;
        }
        if(node == NULL)
        {
            break;
        }
    }
    return node;
}

int NodeHeight(int data, struct Node * node)
{ 
    if (node == NULL) 
        return 0;
    int NodeHeight;
    for(NodeHeight = 1; node != NULL && node->data != data; NodeHeight++)
    {
        
        if(node->data > data)
        {
            node = node->left;
        }
		else if(node->data < data)
        {
            node = node->right;
        }
    }
    return NodeHeight;
} 



void FreeNodes(struct Node * node)
{
    if (node != NULL) 
    { 
        free(node->right); 
        free(node->left); 
        free(node);
    }   
}

void printtree(struct Node * node)
{
    if (node != NULL) 
    { 
        printtree(node->right); 
        printtree(node->left); 
        printf("%d\n", node->data);
    }  
}

