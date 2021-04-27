#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "list.h"
#include "../err_exit.h"

node * inserisciInCoda(node * n, trafficInfo * t){
	node * prec;
	node * tmp = (node *) malloc(sizeof(node));
	if(tmp != NULL){
		tmp->next = NULL;
		tmp->trafficInfo = t;
		if(n == NULL){
			n = tmp;
		}else{
			for(prec = n; prec->next != NULL; prec = prec->next);
			prec->next = tmp;
		}
	} else{
		printf("Memoria esaurita!\n");
	}
	return n;
}

node * inserisciInTesta(node * n, trafficInfo * t){
	node * tmp = (node *) malloc(sizeof(node));
	if(tmp != NULL){
		tmp->trafficInfo = t;
		tmp->next = n;
		n = tmp;    
	} else{
		printf("Memoria esaurita!\n");
	}
	return n;
}

node * getNext(node * n){
	return n->next;
}

int hasNext(node * n){
	if(n->next == NULL){
		return 0;
	}
	return 1;
}

int isSet(node * n){
	if(n == NULL){
		return 0;
	}
	return 1;
}

node * rimuovi(node * lista, trafficInfo * t){
	node * curr = lista, 
		* prec = NULL, 
		* canc;
	int found = 0;
	while(curr && !found){
		if(curr->trafficInfo == t){
			found = 1;
			canc = curr;
			if(prec != NULL){
				prec->next = curr->next;
			}else{
				lista = curr->next;
			}
			free(canc);
		}else{
			prec = curr;
			curr = curr->next;     
		}
	}
	return lista;
}

void printList(node * lista){
	if(lista != NULL){
		printf("%d -> ", lista->trafficInfo->message->id);
		printList(lista->next);
	}
}