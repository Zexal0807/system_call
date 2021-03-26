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

void sendMessage(message* m){
    sleep(m->delay1);
    printLog("S1", "Message can be send");
    if(m->sender->number == 1){
        // Send message whit correct channel

    }else{
        // Send to S2 by pipe
        
    }
}

int main(int argc, char * argv[]) {

	printLog("S1", "Process start with exec");

    // ARGV: initSemId, inputFile
    int initSemId = atoi(argv[0]);
	char *filename = argv[1];

    // Open SHM
    // Open MSGQ
    // OPEN PIPE S1 S2

	node *l = createMessageList(filename);
    
    // Set this process as end init
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

	char log[50];
	sprintf(log, "Loaded message from file %s", filename);
	printLog("S1", log);
	
	time_t arrival;
	time_t departure;

	while(isSet(l)){
		time(&arrival);
        //Send message
        sendMessage(l->message);

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
