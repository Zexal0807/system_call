#include "list.h"

node* inserisciInCoda(node* n, message* m){
	node *prec;
	node *tmp;

	tmp = (node*) malloc(sizeof(node));
	if(tmp != NULL){
		tmp->next = NULL;
		tmp->message = m;
		if(n == NULL){
			n = tmp;
		}else{
			for(prec=n;prec->next!=NULL;prec=prec->next);
			prec->next = tmp;
		}
	} else{
		printf("Memoria esaurita!\n");
	}
	return n;
}

node* inserisciInTesta(node* n, message* m){
	node *tmp;

	tmp = (node*) malloc(sizeof(node));
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

int hasNext(node* n ){
	if(n->next == NULL){
		return 0;
	}
	return 1
}

node* createList(
	char* filename,
	process *s1, 
	process *s2, 
	process *s3, 
	process *r1, 
	process *r2, 
	process *r3
){
	node * list = NULL;

	//apro il file    
	int file = open(filename, O_RDONLY);

	if(file == -1){
		perror("File non esistente");
		exit(1);
	}

	//creo il buffer per la lettura del file
	int size = lseek(file, 0L, SEEK_END);   //dimensione file
	char* buffer = (char*) malloc(sizeof(char) * size);
	lseek(file, 0L, SEEK_SET); //riporto l'indicatore a inizio file

	//leggo file
	read(file, buffer, size);

	int i;
	for(i=0;*(buffer+i)!='\n';i++);
	i++;

	while(*(buffer+i) != 0x0){
		message *m = linetoStruct(buffer, &i, s1, s2, s3, r1, r2, r3);
		list = inserisciInCoda(list, m);
		i++;
	}
	
	return list;
}