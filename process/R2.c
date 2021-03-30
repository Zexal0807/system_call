/// @file R2.c
/// @brief Contiene l'implementazione del receiver 2.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>

node * l;
int receiverSemId;
int sharedMemoryId;
int thereIsMessage = 1;
int messageQueueId;
int pipeR1R2Id;
int pipeR2R3Id;
message *sharedMemoryData;

void openResource(){
    // Open receiver sem
    receiverSemId = createReceiverSemaphore();

    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    // Open MSGQ
    messageQueueId = getMessageQueue();
}

int closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("R2", "detachSharedMemory");
    
    // Close MSGQ
    // Not need to be close

    // Wait S3 end
    printLog("S2", "Wait S1");
    semOp(receiverSemId, 1, 0);
    
    // Close PIPE R1 R2
    closePipe(pipeId);

    // Set this process as end
    semOp(receiverSemId, 1, -1);

	// Wait for 1 second befor end
    printLog("R1", "Process End");
	sleep(1);
	printLog("R1", "Process Exit");
	return 1;
}


int main(int argc, char * argv[]) {

	printLog("R2", "Process start with exec");
	
    // ARGV: initSemId
    int initSemId = atoi(argv[0]);

    // Open SHM
    // Open MSGQ
    // OPEN PIPE R1 R2
    // OPEN PIPE R2 R3

    // Set this process as end init
    semOp(initSemId, 2, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

    printLog("R2", "End init start");

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
	//printLog("R2", log);
		
	time(&departure);

	trafficInfo *t = createTrafficInfo(m, arrival, departure);
	printTrafficInfo(RECEIVER_2_FILENAME, t);

	// Wait for 2 second befor end
	sleep(2);
	printLog("R2", "Process End");
	return 1;
}
