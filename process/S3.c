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

node *l;

// SIGUSR1 del IncraseDelay dell HK
void hacklerIncraseDelayHandle(int sig){
    // ciclo su tutti i messagi e aumenta il time
    node *tmp = l;
    while(isSet(tmp)){
        tmp->message->delay1 += 5;
        tmp = getNext(tmp);
    }
}

// SIGUSR2 del RemoveMsg del HK 
void hacklerRemoveMsgHandle(int sig){
    // ciclo su tutti i messaggi e rimuovo tutti eccetto il primo che verrà inviato a fine sleep
    node *tmp = l;
    tmp = getNext(tmp);
    while(isSet(l)){
        rimuovi(l, l->message)
        tmp = getNext(tmp);
    }
}

// SIGCONT del SendMessage del HK
void hacklerSendMsgHandle(int sig){
    //ciclo su tutti i messaggi setta a 0 i tempi d'attesa
    node *tmp = l;
    while(isSet(tmp)){
        tmp->message->delay1 = 0;
        tmp = getNext(tmp);
    }
}

// SIGTERM ShutDown del HK
void hacklerShutDownHandle(int sig){
    closeResource();
}

void sendMessage(message* m){
    sleep(m->delay1);
    printLog("S1", "Message can be send");
    if(m->sender->number == 1){
        if (strcmp(m->comunication, "Q") == 0) {

        }else if (strcmp(m->comunication, "SH") == 0) {

        }
    }else{
        // Send to S2 by pipe
        
    }
}

int initSemId;
int senderSemId;
int sharedMemoryId;
int messageQueueId;
int pipeId;
int fifoId;

void openResource(){
    // Open SHM
    // Open MSGQ
    // OPEN FIFO
    // OPEN PIPE S2 S3
}

int closeResource(){
    // Close PIPE S2 S3
	// Close SHM
	// Close MSGQ
	// Close FIFO

	// Set this process as end
	semOp(senderSemId, 3, -1);

	// Wait for 3 second befor end
	sleep(3);
	printLog("S3", "Process End");
	return 1;
}

int main(int argc, char * argv[]) {

	printLog("S3", "Process start with exec");

     // ARGV: initSemId
    int initSemId = atoi(argv[0]);

    // Open sender sem
    int senderSemId = createSenderSemaphore();
    
	openResource();
    
	signal(SIGUSR1, hacklerIncraseDelayHandle);
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);

    // Set this process as end init 
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);
	
	time_t arrival;
	time_t departure;

	char log[50];
	trafficInfo *t;
	message *m;

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
