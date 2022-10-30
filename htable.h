#define HASHTABLE_H_INCLUDED
#include <stdbool.h>

struct node {
    char *key;
    int freq;
    struct node *next;
};

struct hashtable {
    struct node **table;
    int currentsize;
    int tablesize;
};

typedef struct {
    char* word;
    int count;
} count;


typedef struct hashtable hashtable;

struct node * add(hashtable *, char *, int);

void order(hashtable *, count *);

hashtable * createtable(int);

struct node * nalloc(char *, int);

void freetable(hashtable *);
