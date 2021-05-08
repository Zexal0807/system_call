#include <stdio.h>
#include <stdlib.h>

#include "trafficInfo.h"

typedef struct node_{
	trafficInfo * trafficInfo;
	struct node_ * next;
} node;

node * inserisciInCoda(node * n, trafficInfo * t);
node * inserisciInTesta(node * n, trafficInfo * t);

node * getNext(node * n);
int hasNext(node * n);
int isSet(node * n);

node * rimuovi(node * n, trafficInfo * t);
void printList(node * lista);

node* createTrafficInfoList(char* filename);