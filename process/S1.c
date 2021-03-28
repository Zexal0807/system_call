/// @file S1.c
/// @brief Contiene l'implementazione del sender 1.
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../message_queue.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

node *l;

int initSemId;
int senderSemId;
int sharedMemoryId;
message * sharedMemoryData;
int messageQueueId;
int pipeId;


void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    // Open MSGQ
    messageQueueId = getMessageQueue();
}

int closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("S1", "detachSharedMemory");
    // Close MSGQ

    // Wait S2 end
    printLog("S1", "Wait S2");
    semOp(senderSemId, 2, 0);
    
    // Close PIPE S1 S2
    closePipe(pipeId);

    // Set this process as end
    semOp(senderSemId, 1, -1);

	// Wait for 1 second befor end
    printLog("S1", "Process End");
	sleep(1);
	printLog("S1", "Process Exit");
	return 1;
}

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
        rimuovi(l, l->message);
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
    printf("S1 %d\n", m->id);
    if(m->sender->number == 1){
        if (strcmp(m->comunication, "Q") == 0) {

        }else if (strcmp(m->comunication, "SH") == 0) {

        }
    }else{
        // Send to S2 by pipe
        
    }
}

int main(int argc, char * argv[]) {

	printLog("S1", "Process start with exec");

    // ARGV: initSemId, PIPE_S1S2, shmId,inputFile
    initSemId = atoi(argv[0]);
    pipeId = atoi(argv[1]);
    sharedMemoryId = atoi(argv[2]);
	char *filename = argv[3];

    // Open sender sem
    senderSemId = createSenderSemaphore();

    openResource();

	l = createMessageList(filename);
    char log[50];
	sprintf(log, "Loaded message from file %s", filename);
	printLog("S1", log);
	
	time_t arrival;
	time_t departure;

    signal(SIGUSR1, hacklerIncraseDelayHandle);
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);

    // Set this process as end init
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);
    
    printLog("S1", "End init start");

    time(&arrival);

    node *tmp = l;

    while(isSet(l)){





        sleep(1);
    }


	while(isSet(l)){
		
        //Send message
        sendMessage(l->message);

		sprintf(log, "Elaborated message: %d", l->message->id);
		printLog("S1", log);
		time(&departure);
		trafficInfo *t = createTrafficInfo(l->message, arrival, departure);
		printTrafficInfo(SENDER_1_FILENAME, t);
		l = getNext(l);
	}

    // Send to S2 that msg are end
    
    return closeResource();
}
