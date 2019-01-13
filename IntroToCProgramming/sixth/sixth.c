#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    
    int i = 1;
    while(i <= argc - 1)
    {
        char * word = argv[i];
	
        int firstVowelIndex = 0;
		int isVowel = 0;
		char letter = word[firstVowelIndex];
        while(isVowel == 0 && letter != '\0')
		{
				if(letter == 'a' || letter == 'e' ||letter == 'i' || letter == 'o' ||letter == 'u' || letter == 'A' || letter == 'E' ||letter == 'I' || letter == 'O' ||letter == 'U')
				{
					isVowel = 1;
				}
				firstVowelIndex++;
				letter = word[firstVowelIndex];
		}
		firstVowelIndex--;

        int size = 0;
        while(word[size] != '\0')
		{
			size++;
		}

        if(firstVowelIndex == 0)
           printf("%syay ", word);
        else 
        {
            int j = 0;
            while(j < firstVowelIndex)
            {
                char temp = word[0];
                int k = 0;
                while(k < size - 1)
				{
                    word[k] = word[k + 1];
					 k++;
				}
                word[k] = temp;
				j++;
            }
            printf("%say ", word);  
        }
		i++;
    }
        
    return 0;
}

