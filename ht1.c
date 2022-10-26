#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "htable.h"

typedef enum {
    FALSE = 0,
    TRUE = 1
} Bool;

#define MAX_WORD_SIZE 50

int main (int argc, char** argv) {
    Bool in_word = FALSE;
    Bool is_truncated = FALSE;
    char word[MAX_WORD_SIZE];
    int  c = 0,
         count_c = 0,
         count_w = 0,
         count_l = 0,
         i = 0;
    hashtable *hash = createtable(5000);
    count* ordered = calloc(5000, sizeof(count));
    struct node *node = NULL;

    in_word = FALSE;
    i = 0;

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
              node = add(hash, word, 0);
              node->freq++;
                i = 0;
            }
        }

        if (c == '\n')
            count_l++;

        count_c++;
    } while (c != EOF);

    printf("\n\t%d %d %d\n", count_c, count_w, count_l);

    order(hash,ordered);
    freetable(hash);
    return (0);
}

