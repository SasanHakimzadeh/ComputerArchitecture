#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv){
    
    int i = 1;
    while(i <= argc - 1)
    {
        char * word = argv[i];

        int length = 0;
        while(word[length] != '\0')
    {
      length++;
    }
    
    char newWord[2];
        newWord[0] = word[length-1];
        newWord[1] = '\0';
        printf("%s", newWord);
    i++;
    }
    return 0;
}

