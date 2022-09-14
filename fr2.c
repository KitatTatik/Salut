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
        printf("DEBUG q = %c, i = %d\n", q, i);
        if (isalnum(q) || ' '==q)
        {
            if (isalpha(q))
                string[i++]=tolower(q);
            else
                string[i++] = q;
        } 
    }
    while (q != '\n' && i < sizeof(string));
    string[i] = '\0';

    for (i = 0, j = 0; i<strlen(string); i++) {
        if (string[i] == ' ' )
        {
            if (!j) continue;
            if (string[i+1] == ' ') continue;
        }
            
        result[j++] = string[i];
    }
    result[j] = '\0';
    
    printf("The modified text is: %s\n", result);

    for (i = 0, j = 0; i < strlen(result); i++) // mb res_len = strlen(result); and so on?
    {
        if (result[i] != ' ')
        {
            string[j++] = result[i];
//            printf("DEBUG %d.\n", j);
        }
        else if (j)
        {
            count++;
            j = 0;
        }
    }
    if (j) count++;
    
    printf("The number of  words in the text is %d.\n", count);
}

