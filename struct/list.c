#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "list.h"
#include "../err_exit.h"

node* inserisciInCoda(node* n, trafficInfo* t){
	node *prec;
	node *tmp = (node*) malloc(sizeof(node));
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

node* inserisciInTesta(node* n, trafficInfo* t){
	node *tmp = (node*) malloc(sizeof(node));
	if(tmp != NULL){
		tmp->trafficInfo = t;
		tmp->next = n;
		n = tmp;    
	} else{
		printf("Memoria esaurita!\n");
	}
	return n;
}

node* getNext(node* n){
	return n->next;
}

int hasNext(node* n){
	if(n->next == NULL){
		return 0;
	}
	return 1;
}

int isSet(node* n){
	if(n == NULL){
		return 0;
	}
	return 1;
}

node* rimuovi(node* lista, trafficInfo *t){
	node *curr = lista, 
		*prec = NULL, 
		*canc;
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

void printList(node* lista){
    if(lista != NULL){
        printf("%d -> ", lista->trafficInfo->message->id);
        printList(lista->next);
    }
}

node* createTrafficInfoList(
	char* filename
){
	node * list = NULL;
	//apro il file    
	int file = open(filename, O_RDONLY);
	ErrOpen(file);

	//creo il buffer per la lettura del file
	int size = lseek(file, 0L, SEEK_END);   //dimensione file
	char* buffer = (char*) malloc(sizeof(char) * size);
	lseek(file, 0L, SEEK_SET); //riporto l'indicatore a inizio file

	//leggo file
	read(file, buffer, size);

	char *end_buffer;
	char *line = strtok_r(buffer, "\n", &end_buffer);

	int firstline = 1;
    time_t arrival;

	while(line != NULL){
		if(firstline != 1){
			message *m = line2message(line);
            time(&arrival);
            // Set the departure time ad arrival for not have warning
            trafficInfo *t = createTrafficInfo(m, arrival, arrival);

			list = inserisciInCoda(list, t);
		}
		firstline = 0;
		line = strtok_r(NULL, "\n", &end_buffer);
	}
	
	return list;
}