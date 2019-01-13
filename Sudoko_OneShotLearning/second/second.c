#include <stdio.h>
#include <stdlib.h>

int solve(char **game);
int isSolveable(char **game);

enum Boolean{TRUE, FALSE};
int boardSize = 9;

int solve(char **game)
{
    if(isSolveable(game) == FALSE)
    {
         return FALSE;
    }
       
    int row;
    int col;
    int foundCell = FALSE;

    for(row = 0; row < boardSize; row++)
    {
       for(col = 0; col < boardSize; col++)
       {
           if(game[row][col] == '-')
           {
               foundCell = TRUE;
               break;
           }
       }
       if(foundCell == TRUE)
       {
           break;
       }
    }

    if(foundCell == FALSE)
        return TRUE;

    int testNum;
    char testString[10];
    for(testNum = 1; testNum <= boardSize; testNum++)
    {
        sprintf(testString, "%d", testNum);
        game[row][col] = testString[0];

        if(solve(game) == TRUE)
            return TRUE;
    }
    game[row][col] = '-';

    return FALSE;
}

int isSolveable(char **game)
{
    int rowOff;
    int colOff;
    int row;
    int col;
    int i;
    int j;

    for(rowOff = 0; rowOff < boardSize; rowOff = rowOff + 3)
    {
        for(colOff = 0; colOff < boardSize; colOff = colOff + 3)
        {
            for(row = 0; row < 3; row++)
            {
                for(col = 0; col < 3; col++)
                {
                    if(game[row + rowOff][col + colOff] != '-')
                    {
                        for(i = 0; i < 3; i++)
                        {
                            for(j = 0; j < 3; j++)
                            {
                                if(i != row || j != col)
                                {
                                    if(game[row + rowOff][col + colOff] == game[i + rowOff][j + colOff])
                                    {
                                        return FALSE;
                                    }
                                }
                                    
                            }
                        }
                    }   
                }     
            }  
        }       
    }
        

    for(row = 0; row < boardSize; row++)
    {
        for(col = 0; col < boardSize; col++)
        {
            if(game[row][col] != '-')
            {
                for(j = col + 1; j < boardSize; j++)
                {
                    if(game[row][col] == game[row][j])
                    {
                        return FALSE;
                    }   
                }   
            }
        }
    }
            
    for(col = 0; col < boardSize; col++)
    {
        for(row = 0; row < boardSize; row++)
        {
            if(game[row][col] != '-')
            {
                for(i = row + 1; i < boardSize; i++)
                {
                    if(game[row][col] == game[i][col])
                    {
                        return FALSE;
                    } 
                } 
            }
        }  
    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    FILE *fptr;
    fptr = fopen(argv[1], "r");
	char **game = malloc (sizeof (char*) * boardSize);

    int row;
    int col;

    for(row = 0; row < boardSize; row++) 
    	*(game + row) = malloc (sizeof (char*) * boardSize);

    char cell[10];

    row = 0;
    for(col = 0; fscanf(fptr, "%s", cell) == 1; col++)
    {
        if(col == boardSize)
        {
            col = 0;
            row++;
        }
        
        game[row][col] = cell[0];
    }

    if(solve(game) == TRUE)
    {
        row = 0;
        while(row < boardSize)
        {
            col = 0;
            while(col < boardSize)
            {
                if(col == boardSize - 1)
                {
                    printf("%c", game[row][col]);
                }
                else
                {
                    printf("%c\t", game[row][col]);
                }
                col++;
            }
            if(row != boardSize - 1)
            {
                printf("\n");
            }
            row++;
        }
	}
    else
        printf("no-solution");

    return 0;
}

