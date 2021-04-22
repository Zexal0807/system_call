#include <stdlib.h>

#include "history.h"

#ifndef IPC_HISTORY_FILE_HEADER
#define IPC_HISTORY_FILE_HEADER "IPC;IDKey;Creator;CreationTime;DestructionTime\n"
#endif

history *createHistory(
    char * ipc, 
    char * key, 
    char * creator, 
    time_t creation, 
    time_t distruction
){

	history *h = (history*) malloc(sizeof(history));

	h->ipc = ipc;
    h->key = key;
    h->creator = creator;
    h->creation = creation;
    h->distruction = distruction;

	return h;
}
