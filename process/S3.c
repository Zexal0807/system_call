/// @file S3.c
/// @brief Contiene l'implementazione del sender 3.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>

void sendMessage(message m){
    
}

int main(int argc, char * argv[]) {

	printLog("S3", "Process start with exec");

     // ARGV: initSemId
    int initSemId = atoi(argv[0]);

    // Open SHM
    // Open MSGQ
    // OPEN FIFO
    // OPEN PIPE S2 S3
    
    // Set this process as end init 
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);
	
	time_t arrival;
	time_t departure;

	// Messaggio di test
	message *m = createMessage(
		1, 
		"Ciao come va?",
		SENDER_3(),
		RECEIVER_1(),
		1,
		1,
		1,
		"H"
	);
	time(&arrival);
	
    // sendMessage(l->message);

	char log[50];
	sprintf(log, "Elaborated message: %d", m->id);
	printLog("S3", log);
		
	time(&departure);

	trafficInfo *t = createTrafficInfo(m, arrival, departure);
	printTrafficInfo(SENDER_3_FILENAME, t);

	// Wait for 3 second befor end
	sleep(3);
	printLog("S3", "Process End");
	return 1;
}
