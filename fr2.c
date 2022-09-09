    #include <stdio.h>
    #include <string.h>
    #include <ctype.h>
     
    # define Words 50
    # define Text_lenght 1000
    int main()
    {
        char string[Text_lenght], result[Text_lenght], word[Words], q;
        int i = 0, j = 0, count = 0;
        
	printf("Enter your sample text: ");
        do
        {
            q = getchar();
	    if (isalnum(q) || q==' ')
	    {
	    if (isalpha(q))
		    string[i++]=tolower(q);
	    else
	    string[i++] = q;
	    } 
	}
	    while (q != '\n');
        string[i] = '\0';
        for (i = 0; i<strlen(string) ; i++) {
       
	       	if (string[i] == ' ' )
        {
          if (j==0) continue;
          if (string[i+1] == ' ') continue;
        }
		
        result[j] = string[i];
        j++;
    }
        printf("The modified text is: %s\n", result);
         j=0;
	for (i = 0; i < strlen(result); i++)
        {
            while (i < strlen(result) && result[i] != ' ')
            {
              string[j++] = result[i++];
//	      printf("Debug print %d.\n", j);

	    }
	       if (j != 0)
            {
                count++;
                j = 0;
            }
     
	    }
	
        printf("The number of  words in the text is %d.\n", count);
	}
