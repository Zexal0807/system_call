#include <stdio.h>
#include <stdlib.h>

#include "message.h"

typedef struct node_{
  message *message;
  struct node_ *next;
} node;

node* inserisciInCoda(node* n, message* m);
node* inserisciInTesta(node* n, message* m);
node* inserisciOrdinato(node*n , message* m);
node* rimuovi(node* n, message* m);
node* getNext(node* n);
int hasNext(node* n);

node* createList(char* filename,
    process *s1, 
    process *s2, 
    process *s3, 
    process *r1, 
    process *r2, 
    process *r3
);