#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

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

struct node * add(hashtable *h, char *key, int freq) {
    struct node *newnode;
    int index = hashcode(key) % h->tablesize;
    struct node *current = h->table[index];

    while(current != NULL) {
        if(strcmp(key, current->key) == 0)
            return current;
        current = current->next;
    }
    newnode = nalloc(key, freq);
    newnode->next = h->table[index];
    h->table[index] = newnode;
    h->currentsize = h->currentsize + 1;
    return newnode;
}

void order(hashtable *h, count* ordered) {
    struct node *current = NULL;
    int i,
	j=0;
    printf("      --------------------\n");
    for(i = 0; i < h->tablesize; i++) {
        current = h->table[i];
        while(current != NULL) {
            ordered[j].word=current->key;
            ordered[j++].count=current->freq;	
            current = current->next;
        }
    }
    qsort(ordered, j, sizeof(count), cmp_count);
    for (int i=0; i<j; i++) {
        if (ordered[i].count >=j/50)
            printf("\t%s\t\t %d\n", ordered[i].word, ordered[i].count);
    }
}

int hashcode(char *key) {
    int i, hash = 7;
    int length = strlen(key);

    for(i = 0; i < length; i++)
        hash = (hash * 31) + *(key + i);
    return hash & 0x7FFFFFFF;  /* https://www.cs.princeton.edu/courses/archive/spring19/cos226/lectures/study/34HashTables.html , https://stackoverflow.com/questions/9380670/why-does-java-use-hash-0x7fffffff-tab-length-to-decide-the-index-of-a-key*/
}

struct hashtable * createtable(int size) {
    int i;
    if(size < 1)
        return NULL;
    hashtable *table = (hashtable *) malloc(sizeof(hashtable));
    table->table = (struct node **) malloc(sizeof(struct node *) * size);

    if(table != NULL) {
        table->currentsize = 0;
        table->tablesize = size;
    }
    for(i = 0; i < size; i++)
        table->table[i] = NULL;
    return table;
}

struct node * nalloc(char *key, int freq) {
    struct node *p = (struct node *) malloc(sizeof(struct node));

    if(p != NULL) {
        p->key = strdup(key);
        p->freq = freq;
        p->next = NULL;
    }
    return p;
}

void freenode(struct node *node) {
    free(node->key);
    free(node);
}

void freetable(hashtable *h) {
    struct node *current = NULL;
    int i;

    for(i = 0; i < h->tablesize; i++) {
        current = h->table[i];
        if(current == NULL)
            continue;
        while(current->next != NULL) {
            h->table[i] = h->table[i]->next ;
            freenode(current);
            current = h->table[i];
        }
        freenode(current);
    }
    free(h->table);
    free(h);
}

