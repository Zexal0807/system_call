/// @file S1.c
/// @brief Contiene l'implementazione del sender 1.
#include <stdio.h>
#include <unistd.h>

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

int main(int argc, char * argv[]) {
	printLog("S1", "Process start with exec");
	
	char *filename = argv[0];
	node *l = createMessageList(filename);
  
	char log[50];
	sprintf(log, "Loaded message from file %s", filename);
	printLog("S1", log);
	
	time_t arrival;
	time_t departure;

	while(l != NULL){
		time(&arrival);

		sprintf(log, "Elaborated message: %d", l->message->id);
		printLog("S1", log);
		
		time(&departure);

		trafficInfo *t = createTrafficInfo(l->message, arrival, departure);
		printTrafficInfo(SENDER_1_FILENAME, t);
    l = getNext(l);
	}
	
	// Wait for 1 second befor end
	sleep(1);
	printLog("S1", "Process End");
	return 1;
}
