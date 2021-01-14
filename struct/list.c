#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

#include "list.h"
#include "../err_exit.h"

node* inserisciInCoda(node* n, message* m){
	node *prec;
	node *tmp = (node*) malloc(sizeof(node));
	if(tmp != NULL){
		tmp->next = NULL;
		tmp->message = m;
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

node* inserisciInTesta(node* n, message* m){
	node *tmp = (node*) malloc(sizeof(node));
	if(tmp != NULL){
		tmp->message = m;
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

int isEnded(node* n){
	if(n == NULL){
		return 0;
	}
	return 1;
}

node* rimuovi(node* lista, message *m){
  node *curr = lista, 
	  *prec = NULL, 
	  *canc;
  int found = 0;  

  while(curr && !found){
    if(curr->message == m){
      found = 1;
      canc = curr;
      curr = curr->next;     
      if(prec != NULL)
      prec->next = curr;
      else
      lista = curr;
      free(canc);
    }else{
      prec=curr;
      curr = curr->next;     
    }
  }
  return lista;
}

node* createMessageList(
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

	int i;
	for(i=0; *(buffer+i) != '\n'; i++);
	i++;
  
	while(*(buffer+i) != 0x0){
		message *m = line2message(buffer, &i);
		list = inserisciInCoda(list, m);
		i++;
	}
	
	return list;
}