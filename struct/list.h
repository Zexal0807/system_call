#include <stdio.h>
#include <stdlib.h>

#include "trafficInfo.h"

typedef struct node_{
	trafficInfo *trafficInfo;
	struct node_ *next;
} node;

node* inserisciInCoda(node* n, trafficInfo* m);
node* inserisciInTesta(node* n, trafficInfo* m);

node* getNext(node* n);
int hasNext(node* n);
int isSet(node* n);

node* rimuovi(node* n, trafficInfo* m);
void printList(node* lista);

node* createTrafficInfoList(char* filename);