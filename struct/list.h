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