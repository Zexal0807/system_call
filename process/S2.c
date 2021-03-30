/// @file S2.c
/// @brief Contiene l'implementazione del sender 2.

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

node *l;
int initSemId;
int senderSemId;
int sharedMemoryId;
message * sharedMemoryData;
int messageQueueId;
int pipeS1S2Id;
int pipeS2S3Id;

// SIGPIPE del S1
void hacklerReadFromPipe(int sig){
/*
    char message [150]; 
    read(pipeS1S2Id, message, 150);
    time_t arrival;
    message * m = line2message(message);
    time(&arrival);
    trafficInfo *t = createTrafficInfo(m, arrival, arrival);
    aggiungiInCoda(l, t); 
*/
}

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    
    // Open MSGQ
    messageQueueId = getMessageQueue();

    // Set signal for read form pipe
    signal(SIGPIPE, hacklerReadFromPipe);
}

int closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("S2", "detachSharedMemory");
    
    // Close MSGQ
    // Not need to be close
    
    // Wait S3 end
    printLog("S2", "Wait S3");
    semOp(senderSemId, 3, 0);

	// Close PIPE S2 S3

    // Close PIPE S1 S2

    // Set this process as end
    semOp(senderSemId, 2, -1);

	// Wait for 2 second befor end
	printLog("S2", "Process End");
	sleep(2);
	printLog("S2", "Process Exit");
	return 1;
}

void sendMessage(message* m){
    printLog("S2", "Message can be send");
    if(m->sender->number == 1){
        if (strcmp(m->comunication, "Q") == 0) {

        }else if (strcmp(m->comunication, "SH") == 0) {

        }
    }else{
        // Send to S3 by pipe
        
    }
}

int main(int argc, char * argv[]) {

    printLog("S2", "Process start with exec");

    // ARGV: initSemId, PIPE_S2S3, PIPE_S2S3, S3pid
    initSemId = atoi(argv[0]);
    pipeS1S2Id = atoi(argv[1]);
    pipeS2S3Id = atoi(argv[2]);
    sharedMemoryId = atoi(argv[3]);
    int S3pid = atoi(argv[4]);

    // Open sender sem
    senderSemId = createSenderSemaphore();

	openResource();
    
    // Set this process as end init 
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);
    
    printLog("S2", "End init start");

	time_t arrival;
	time_t departure;

	char log[50];
    int thereMessage = 0;

	while(thereMessage || isSet(l)){
		
/*

			//Send
			// sendMessage(m);
		
			//sprintf(log, "Elaborated message: %d", m->id);
			//printLog("S2", log);
		
			time(&departure);

			//t = createTrafficInfo(m, arrival, departure);
			//printTrafficInfo(SENDER_2_FILENAME, t);
		}else{
			return closeResource();
		}
        */

	}

    return closeResource();
}
