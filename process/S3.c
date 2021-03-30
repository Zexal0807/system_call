/// @file S3.c
/// @brief Contiene l'implementazione del sender 3.

#include "../err_exit.h"
#include "../defines.h"
#include "../struct/message.h"
#include "../shared_memory.h"
#include "../message_queue.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

node * l;
int initSemId;
int senderSemId;
int sharedMemoryId;
message * sharedMemoryData;
int messageQueueId;
int pipeId ;
int fifoId ;

// SIGPIPE del S2
void readFromPipeHandle(int sig){
    char msg [150]; 
    read(pipeId, msg, 150);

    time_t arrival;
    message *m = line2message(msg);

    char log[50];
    sprintf(log, "Receive %d from PIPE S2S3", m->id);
    printLog("S3", log);

    time(&arrival);
    trafficInfo *t = createTrafficInfo(m, arrival, arrival);
    inserisciInCoda(l, t);
}

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);

    // Open MSGQ
    messageQueueId = getMessageQueue();

    // OPEN FIFO
    fifoId = openSenderFIFO();

    // Seti signal for read from PIPE
    signal(SIGPIPE, readFromPipeHandle);
}

int closeResource(){
	// Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("S3", "detachSharedMemory");
    
	// Close MSGQ
    // Not need to be close

	// Close FIFO
    close(fifoId);

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
        printLog("S3", "Message send by MessageQueue");
    }else if (strcmp(m->comunication, "SH") == 0) {
        printLog("S3", "Message send by SharedMemory");
    }else if (strcmp(m->comunication, "FIFO") == 0) {
        printLog("S3", "Message send by FIFO");
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

	time_t departure;

	char log[50];
    node *tmp;
	trafficInfo *t;
    int thereMessage = 0;

	while(thereMessage || isSet(l)){
        /*
        printf("Message in list: ");
        printList(l);
        printf("\n");
        */
        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delay3 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("S3", log);
                
                t->departure = departure;

		        printTrafficInfo(SENDER_3_FILENAME, t);
                sendMessage(t->message);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delay3 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
	}	
    return closeResource();
}
