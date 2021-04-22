#include <time.h>


/*
	Struttura di una IPC history
*/
typedef struct {
	char * ipc;
    char * key;
	char * creator;
	time_t creation;
	time_t distruction;
} history;

history *createHistory(char * ipc, char * key,  char * creator, time_t creation, time_t distruction);

