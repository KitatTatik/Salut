#include <stdio.h>
#include <string.h>
#include <ctype.h>
 
# define Words 50
# define Text_length 1000

int main()
{
    char string[Text_length],
         result[Text_length],
         word[Words];
    char q;
    int  i = 0,
         j = 0,
         count = 0;
    
    printf("Enter your sample text: ");
    do
    {
        q = getchar();
        if (isalnum(q) || ' '==q)
        {
            if (isalpha(q))
                string[i++]=tolower(q);
            else
                string[i++] = q;
        } 
    }
    while (q != '\n'); // TODO: deal with endless strings

    string[i] = '\0';

    for (i = 0, j = 0; i<strlen(string); ) {
        if (string[i] == ' ' )
        {
            if (!j) continue;
            if (string[i+1] == ' ') continue;
        }
            
        result[j++] = string[i++];
    }
    printf("The modified text is: %s\n", result);

    for (i = 0, j = 0; i < strlen(result); ) // mb res_len = strlen(result); and so on?
    {
        while (result[i] != ' ')
        {
            string[j++] = result[i++];
//          printf("Debug print %d.\n", j);

        }

        if (j != 0)
        {
            count++;
            j = 0;
        }
    }
    
    printf("The number of  words in the text is %d.\n", count);
}

