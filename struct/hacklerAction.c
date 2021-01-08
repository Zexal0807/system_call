#include <stdlib.h>
#include <math.h>
#include <string.h>

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

    file=open(pathname, O_RDONLY);

    ErrOpen(file);

    dim=lseek(file, 0L, SEEK_END);
    lseek(file, 0L, SEEK_SET);

    buffer=(char *)malloc(sizeof(char)*dim);

    readed=read(file, buffer, dim);

    if(readed==-1)
        ErrExit("File non leggibile\n");

    close(file);

    return buffer;
}