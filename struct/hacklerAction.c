#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hacklerAction.h"

hackletAction *createHackletAction(
	int id,
	int delay,
	process* target,
	char* action
){

	hackletAction *a = (hackletAction*) malloc(sizeof(hackletAction));

	a->id = id;
	a->delay = delay;
	a->target = target;
	a->action = action;

	return a;
}

int countHacklerActionChars(hackletAction *h){
	int chars = 0;

	// Number of digit of the id
	chars += floor(log10(h->id)) + 1;
	// Number of digit of the delay
	chars += floor(log10(h->delay)) + 1;
	// Target process
	chars += strlen(process2string(h->target));
	// Target process
	chars += strlen(h->action);
	// Add the ;
	chars += 3;
	// Add \n
	chars += 1;
	return chars;
}

char *openHackler(char *pathname){
    int file, dim, readed;
    char *buffer;

    file = open(pathname, O_RDONLY);

    ErrOpen(file);

    dim = lseek(file, 0L, SEEK_END);
    lseek(file, 0L, SEEK_SET);

    buffer = (char *)malloc(sizeof(char)*dim);

    readed = read(file, buffer, dim);

    if(readed == -1)
        ErrExit("File non leggibile\n");

    close(file);

    return buffer;
}

int dimAction(char *buffer, int *i){
    int j; //var indice per il conto
	int counter = 0; //var per conto caratteri
    /*
    partendo dal punto in cui sono (indicato da i), avanzo l'indice j fino al prossimo ';' incrementando man mano il counter, che così finisce con il contenere il numero di caratteri
    */
	for(j=*i ; *(buffer + j) != ';'; j++)
		counter++;
    return counter;
}

hackletAction* linetoStruct(
	char *buffer, 
	int *i
){

	//Leggo l'id dal file trasformandolo in intero
	int id = readInt(buffer, i);
	fileAhead(i);
	
	//Leggo il delay dal file trasformandolo in intero
	int delay = readInt(buffer, i);
	fileAhead(i);

	//ricavo la stringa con il contenuto
	char* tar = (char*) malloc(sizeof(char) * dimMessage(buffer, i));
	for (j = 0; *(buffer + *i) != ';'; j++){
		*(tar + j) = *(buffer + *i);
		fileAhead(i);
	}
	fileAhead(i);
	process* target = string2process(tar);

	fileAhead(i);

	//Analizzo l'azione
	//creo la stringa
	char* action = (char*)malloc(sizeof(char) * dimAction(buffer, i));
	for(j = 0; *(buffer+*i) != '\n' && *(buffer + *i) != '\0'; j++){
		*(action + j) = *(buffer + *i);
		fileAhead(i);
	}

	//creo l'azione
	return createHackletAction(id, delay, target, action);
}