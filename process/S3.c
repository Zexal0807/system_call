/// @file S3.c
/// @brief Contiene l'implementazione del sender 3.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../message_queue.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

trafficInfo * lista;
int initSemId;
int senderSemId;
int sharedMemoryId ;
message * sharedMemoryData;
int messageQueueId;
int pipeId ;
int fifoId ;


// SIGPIPE del S2
void hacklerReadFromPipe(int sig){
/*
    message *m  = read(pipeS1S2Id,....);
    aggiungiInCoda(l, m); 
*/
}

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);

    // Open MSGQ
    messageQueueId = getMessageQueue();

    // OPEN FIFO

    // Seti signal for read from PIPE
    signal(SIGPIPE, hacklerReadFromPipe);
}

int closeResource(){
	// Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("S3", "detachSharedMemory");
    
	// Close MSGQ
    // Not need to be close

	// Close FIFO

	// Set this process as end
	semOp(senderSemId, 3, -1);

	// Wait for 3 second befor end
	printLog("S3", "Process End");
	sleep(3);
	printLog("S3", "Process Exit");
	return 1;
}


void sendMessage(message* m){
    printLog("S3", "Message can be send");
    if (strcmp(m->comunication, "Q") == 0) {

    }else if (strcmp(m->comunication, "SH") == 0) {

    }else if (strcmp(m->comunication, "FIFO") == 0) {

    }
}

int main(int argc, char * argv[]) {

	printLog("S3", "Process start with exec");

    // ARGV: initSemId, PIPE_S2S3
    int initSemId = atoi(argv[0]);
    pipeId = atoi(argv[1]);
    sharedMemoryId = atoi(argv[2]);

    // Open sender sem
    senderSemId = createSenderSemaphore();

	openResource();
/*
	signal(SIGUSR1, hacklerIncraseDelayHandle);
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);
*/
    // Set this process as end init 
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);
    
    printLog("S3", "End init start");
	
	time_t arrival;
	time_t departure;

	char log[50];
	trafficInfo *t;
	message *m;

    int thereMessage = 0;

	while(1){
		// Wait can read from PIPE S2 S3
		semOp(senderSemId, 5, -1);
        
		if(thereMessage){
			// Read message
			//m = ...

			time(&arrival);

			//Send
			// sendMessage(m);
		
			//sprintf(log, "Elaborated message: %d", m->id);
			//printLog("S3", log);
		
			time(&departure);

			//t = createTrafficInfo(m, arrival, departure);
			//printTrafficInfo(SENDER_3_FILENAME, t);
		}else{
			return closeResource();
		}
	}	
}
