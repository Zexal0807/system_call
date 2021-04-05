/// @file R1.c
/// @brief Contiene l'implementazione del receiver 1.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../message_queue.h"
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
int pipeId;
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
    printLog("R1", "detachSharedMemory");
    
    // Close MSGQ
    // Not need to be close

    // Wait S2 end
    printLog("S1", "Wait S2");
    semOp(receiverSemId, 2, 0);
    
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

	printLog("R1", "Process start with exec");

    // ARGV: initSemId, pipeR1R2, sharedMemoryId
    int initSemId = atoi(argv[0]);
    pipeId = atoi(argv[1]);
    sharedMemoryId = atoi(argv[2]);

    openResource();

    // Set this process as end init
    semOp(initSemId, 2, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

    printLog("R1", "End init start");

	time_t departure;
    
    char log[50];

    node *tmp;
    trafficInfo *t;

    while(thereIsMessage || isSet(l)){
        // Try to read form msgqueue
        // Try to read form shared memory
        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delayR1 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("R1", log);
                t->departure = departure;
		        printTrafficInfo(RECEIVER_1_FILENAME, t);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delayR1 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
	}
    
    return closeResource();
/*
	// Messaggio di test
	message *m = createMessage(
		1, 
		"Ciao come va?",
		SENDER_1(),
		RECEIVER_1(),
		1,
		1,
		1,
		"H"
	);
	time(&arrival);

	char log[50];
	sprintf(log, "Elaborated message: %d", m->id);
	//printLog("R1", log);
		
	time(&departure);

	trafficInfo *t = createTrafficInfo(m, arrival, departure);
	printTrafficInfo(RECEIVER_1_FILENAME, t);

	// Wait for 1 second befor end
	sleep(1);
	printLog("R1", "Process End");
	return 1;*/
}
