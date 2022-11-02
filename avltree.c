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
typedef int Status;
#define LH +1
#define EH 0
#define RH -1

struct treeNode
{
    char data[MAX_WORD_SIZE];
    int count;
    int bf; // balance
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

void R_Rotate(TreeNodePtr *p){
    TreeNodePtr L;
    L = (*p)->leftPtr;       
    (*p)->leftPtr = L->rightPtr;
    L->rightPtr = (*p);
    *p = L; 
}

void L_Rotate(TreeNodePtr *p){
    TreeNodePtr R;
    R = (*p)->rightPtr;       
    (*p)->rightPtr = R->leftPtr;
    R->leftPtr = (*p);
    *p = R;
}

void LeftBalance(TreeNodePtr *T){
    TreeNodePtr L, Lr;
    L = (*T)->leftPtr;        
    switch(L->bf){  
        case LH:        
            (*T)->bf = L->bf = EH;
            R_Rotate(T);
            break;
        case RH:       
            Lr = L->rightPtr; //Lr points to the right subtree root of the left child of T
            switch(Lr->bf){ //Modify the balance factor of T and its left child
                case LH:
                    (*T)->bf = RH;
                    L->bf = EH;
                    break;
                case EH:
                    (*T)->bf = L->bf = EH;
                    break;
                case RH:
                    (*T)->bf = EH;
                    L->bf = LH;
                    break;
            }
            Lr->bf = EH;    //Balance factor is set to 0 
            L_Rotate(&(*T)->leftPtr);  
            R_Rotate(T);    
        }
}

void RightBalance(TreeNodePtr *T){
    TreeNodePtr R, Rl;
    R = (*T)->rightPtr;   
    switch(R->bf){  
        case RH:              
            (*T)->bf = R->bf = EH;
            L_Rotate(T);
            break;
        case LH:        
            Rl = R->leftPtr; //Lr points to the right subtree root of the left child of T
            switch(Rl->bf){  
                case LH:
                    (*T)->bf = RH;
                    R->bf = EH;
                    break;
                case EH:
                    (*T)->bf = R->bf = EH;
                    break;
                case RH:
                    (*T)->bf = EH;
                    R->bf = LH;
                    break;
            }
            Rl->bf = EH;    //Balance factor is set to 0 
            R_Rotate(&(*T)->rightPtr);        
            L_Rotate(T);    
    }
}
                 
Status InsertAVL(TreeNodePtr *T, char e[MAX_WORD_SIZE], Status *taller){
    if(!*T){        //Insert a new node, the tree "grows taller", set taller to TRUE
        *T = (TreeNode*)malloc(sizeof(TreeNode));
        strcpy((*T)->data, e);
	(*T)->count++;
        (*T)->leftPtr = (*T)->rightPtr = NULL;      
        (*T)->bf = EH;  //The new node balance factor is 0 
        *taller = TRUE;  
        }else{
            if(strcmp(e, (*T)->data)==0){    
                *taller = FALSE;         
                (*T)->count++;
		return FALSE;
            }
            if(strcmp(e, (*T)->data)<0){    
                if(!InsertAVL(&(*T)->leftPtr, e, taller))         
                    return FALSE;
                        if(taller){
                            switch((*T)->bf){       //Check the balance of T
                                case LH:
                                    LeftBalance(T);          
                                    *taller = FALSE;         
                                    break;
                                case EH:        
                                    (*T)->bf = LH;
                                    *taller = TRUE;  
                                    break;
                                case RH:        
                                    (*T)->bf = EH;
                                    *taller = FALSE;
                                    break;
                            }
                        }
                }else{  //Continue to search in the right subtree of T
                    if(!InsertAVL(&(*T)->rightPtr, e, taller))        //Not inserted
                        return FALSE;
                            if(*taller){    
                                switch((*T)->bf){       //Check the balance of T
                                    case LH:        
                                        (*T)->bf = EH;
                                        *taller = FALSE;
                                        break;
                                    case EH:       
                                        (*T)->bf = RH;
                                        *taller = TRUE;
                                        break;
                                    case RH:        
                                        RightBalance(T);
                                        *taller = FALSE;
                                        break;
                                }
                            }
                        }
            }
    return TRUE;
}

void alphabetic(TreeNodePtr treePtr, count* ordered)
{   
    if (treePtr != NULL )
    {
        alphabetic(treePtr->leftPtr, ordered);
        ordered[num_unique].word=treePtr->data;
        ordered[num_unique++].count=treePtr->count;
        alphabetic(treePtr->rightPtr,ordered);
    }
}


int main (int argc, char** argv) {

    Bool in_word = FALSE;
    Bool is_truncated = FALSE;
    char word[MAX_WORD_SIZE];
    Status taller;

    TreeNodePtr rootPtr = NULL;
    
    int  c = 0,
         count_c = 0,
         count_w = 0,
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
                InsertAVL(&rootPtr, word, &taller);   
                i = 0;
                  }   
        }
        if (c == '\n')
            count_l++;

        count_c++;
    } while (c != EOF);
    
    count* ordered = calloc(count_w, sizeof(count));
    printf("  \n");
    printf("**********************\n");
    printf("%d %d %d\n", count_c, count_w, count_l);
    printf("**********************\n");    
    num_unique=0;
    alphabetic(rootPtr,ordered);
    qsort(ordered, num_unique, sizeof(count), cmp_count);
    for (int i=0; i<num_unique; i++) {
        if (ordered[i].count >=num_unique/50)
            printf(" %s\t\t %d\n", ordered[i].word, ordered[i].count);
    }
}
