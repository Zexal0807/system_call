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

history * createHistory(char * ipc, char * key,  char * creator, time_t creation, time_t distruction);

int countHistoryChars(history * h);

/*
	Crea il file se non essite, troncalo se esite e stampa gli header
*/
void printHistoryHeader(char * filename);

void printHistory(char *filename, history * data);