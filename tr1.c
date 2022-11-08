#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

#define MAX_WORD_SIZE 25

struct treeNode
{
  char data[MAX_WORD_SIZE];
  int count;
  struct treeNode *leftPtr, *rightPtr;
};

typedef struct {
    char* word;
    int count;
} count;

typedef struct treeNode TreeNode;
typedef TreeNode* TreeNodePtr;

int num_unique = 0;

// Comparison function for qsort() ordering by count descending.
int cmp_count(const void* p1, const void* p2) {
    int c1 = ((count*)p1)->count;
    int c2 = ((count*)p2)->count;
    if (c1 == c2){
        const char* d1=((count*)p1)->word;
        const char* d2=((count*)p2)->word;
        return *d1-*d2;
    }
    if (c1 < c2) return 1;
    return -1;
}

void insertNode(TreeNodePtr *treePtr, char word[MAX_WORD_SIZE])
{
    TreeNode *temp = NULL;
    if (*treePtr == NULL )
    {
        temp = (TreeNode *) malloc(sizeof(TreeNode));
        temp->leftPtr = NULL;
        temp->rightPtr = NULL;
	temp->count=0;
        strcpy(temp->data, word);
        temp->count+=1;

        *treePtr = temp;
    }
    else if (strcmp(word, (*treePtr)->data) < 0)
    {
        insertNode(&((*treePtr)->leftPtr), word);
    }
    else if (strcmp(word, (*treePtr)->data) > 0)
    {
        insertNode(&((*treePtr)->rightPtr), word);
    }
    else if (0 == strcmp(word,(*treePtr)->data)) 
    {
        (*treePtr)->count++;
    }
}

void alphabetic(TreeNodePtr treePtr, count* ordered)
{   
    if (treePtr != NULL )
    {
        alphabetic(treePtr->leftPtr, ordered);
        ordered[num_unique].word=treePtr->data;
        ordered[num_unique].count=treePtr->count;
//	printf("in tree  %s\t\t %d\n", ordered[num_unique].word, ordered[num_unique].count);
	num_unique++;
        alphabetic(treePtr->rightPtr,ordered);
    }
}


int main (int argc, char** argv) {

    Bool in_word = FALSE;
    Bool is_truncated = FALSE;
    char word[MAX_WORD_SIZE];
    FILE *mf;

    TreeNodePtr rootPtr = NULL;
    int  c = 0,
         count_c = 0,
         count_w = 0,
         count_l = 0,
         i = 0;

    in_word = FALSE;

    mf = fopen ( "alefph.txt", "r" );
    fseek( mf, 0, SEEK_END );
    long file_size = ftell( mf );
    char * buffer = (char*) malloc( file_size );
    
    if( !buffer )
    {
        fclose( mf );
        fputs( "Could not allocate memory for file buffer", stderr );
        return -1;
    }
    
    fseek ( mf , 0, SEEK_SET );
    if( file_size != fread( buffer, 1, file_size, mf ) )
    {
        free( buffer );
        fclose( mf );
        fputs( "Read data size is not equal to actual file size.", stderr );
        return -1;
    }
    fclose( mf );
    
    int j = 0;
    buffer[file_size] ='\0';
    
    while ( j <= file_size ) {
        c = buffer[j];
        j++;	
        if (isalpha(c)) {
            if (i >= (MAX_WORD_SIZE - 1)) {
                if (!is_truncated) {
                    fprintf(stderr, "WARN: word buffer overflow trunc. : '%s'\n", word);
                    is_truncated = TRUE;
                }
            } else {
                word[i++] = tolower(c);
                word[i] = '\0';
            }

            if (!in_word) {
                in_word = TRUE;
                  count_w++;
            }
        } else {
            in_word = FALSE;
            is_truncated = FALSE;

            if(i) {
                insertNode(&rootPtr, word);  
                i = 0;
                  }   
        }
        if (c == '\n')
            count_l++;

        count_c++;
    }

    free (buffer );
    
    count* ordered = calloc(count_w, sizeof(count));
    printf("  \n");
    printf("**********************\n");
    printf("%d %d %d\n", count_c, count_w, count_l); 
    printf("**********************\n");    
    
    num_unique=0;
    alphabetic(rootPtr,ordered);
    printf("**********************\n");
    printf("%d \n", num_unique);
    
    qsort(ordered, num_unique, sizeof(count), cmp_count);
    for (int i=0; i < num_unique; i++) {
        if (ordered[i].count >= num_unique/50)   
            printf(" %s\t\t %d\n", ordered[i].word, ordered[i].count); 
    }
}
