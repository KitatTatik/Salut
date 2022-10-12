#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

#define MAX_WORD_SIZE 50


struct treeNode
{
  char data[MAX_WORD_SIZE];
  int count;
  struct treeNode *leftPtr, *rightPtr;
};

int r;

typedef struct treeNode TreeNode;
typedef TreeNode* TreeNodePtr;

typedef struct treeNode TreeNode1;
typedef TreeNode1* TreeNodePtr1;


int cmpres (TreeNodePtr p1, TreeNodePtr1 p2, int res)
{
    if (p1->count > p2->count)  return 1;
    if (p1->count < p2->count) return -1;
    if (p1->count = p2->count)
    {
        res=strcmp(p1->data, p2->data); 
        if (res > 0) return 1;
        if (res < 0) return -1;
    }      
}

TreeNodePtr1 exchange(TreeNodePtr1 *TemPtr, TreeNodePtr treePtr)
{
    TreeNode1 *temp = NULL;
    TreeNodePtr1 TemPtr1=NULL;
    if (*TemPtr == NULL)
    { 
        temp = (TreeNode1 *) malloc(sizeof(TreeNode1));
        temp->leftPtr = NULL;
        temp->rightPtr = NULL;
        strcpy(temp->data, treePtr->data);
        temp->count = treePtr->count;

        *TemPtr = temp;
	if(TemPtr1==NULL) TemPtr1=temp;
    } 
    else if (cmpres(treePtr,*TemPtr,r) < 0)
       {
        exchange(&((*TemPtr)->leftPtr), treePtr);
       }
    else if (cmpres(treePtr,*TemPtr,r) > 0)
       {
        exchange(&((*TemPtr)->rightPtr), treePtr);
       }	
 return(TemPtr1);
}

void insertNode(TreeNodePtr *treePtr, char word[MAX_WORD_SIZE])
{
    TreeNode *temp = NULL;
    if (*treePtr == NULL )
    {
        temp = (TreeNode *) malloc(sizeof(TreeNode));
        temp->leftPtr = NULL;
        temp->rightPtr = NULL;
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

TreeNodePtr1 bypass(TreeNodePtr1 TemPtr, TreeNodePtr treePtr)
{
    TreeNodePtr1 TemPtr1;	
    if (treePtr != NULL)
    {  
	TemPtr1=exchange(&TemPtr,treePtr);    
        bypass(TemPtr,treePtr->leftPtr);
        bypass(TemPtr,treePtr->rightPtr);
	return(TemPtr1);
    }
}

void alphabetic(TreeNodePtr treePtr)
{   
    if (treePtr != NULL )
    {
        alphabetic(treePtr->leftPtr);
        printf(" %s\t\t%d\n", treePtr->data, treePtr->count);
        alphabetic(treePtr->rightPtr);
    }
}

void alphabetic2(TreeNodePtr1 TemPtr)
{   
    if (TemPtr != NULL )
    {
        alphabetic2(TemPtr->leftPtr);
        printf(" %s\t\t%d\n", TemPtr->data, TemPtr->count);
        alphabetic2(TemPtr->rightPtr);
    }
}

int main (int argc, char** argv) {

    Bool in_word = FALSE;
    Bool is_truncated = FALSE;
    char word[MAX_WORD_SIZE];
    TreeNodePtr rootPtr = NULL;
    TreeNodePtr1 rPtr=NULL;
    TreeNodePtr1 rPtr1=NULL;
    int  c = 0,
         count_c = 0,
         count_w = 0i,
         count_l = 0,
         plus = 0,
         i = 0;
    in_word = FALSE;

    do {
        if ((c = getchar()) != EOF && isalpha(c)) {
            if (i >= (MAX_WORD_SIZE - 1)) {
                if (!is_truncated) {
                    fprintf(stderr, "WARN: word buffer overflow, truncating to this: '%s'\n", word);
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
    } while (c != EOF);
    printf("  \n");
    printf("**********************\n");
    printf("%d %d %d\n", count_c, count_w, count_l);
    printf("**********************\n");    
    printf("  \n");
    alphabetic(rootPtr);
    rPtr1=bypass(rPtr,rootPtr);
    printf("**********************\n"); 
    alphabetic2(rPtr1);
}
