/// @file S2.c
/// @brief Contiene l'implementazione del sender 2.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]) {

    printLog("S2", "Process start with exec");

    // ARGV: initSemId
    int initSemId = atoi(argv[0]);

    // Open SHM
    // Open MSGQ
    // OPEN PIPE S1 S2
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
		SENDER_1(),
		RECEIVER_2(),
		1,
		1,
		1,
		"H"
	);
	time(&arrival);
	
	char log[50];
	sprintf(log, "Elaborated message: %d", m->id);
	printLog("S2", log);
		
	time(&departure);

	trafficInfo *t = createTrafficInfo(m, arrival, departure);
	printTrafficInfo(SENDER_2_FILENAME, t);

	// Wait for 2 second befor end
	sleep(2);
	printLog("S2", "Process End");
	return 1;
}
