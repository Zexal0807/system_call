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
int thereIsMessage = 1;

// SIGPIPE del S2
void readFromPipeHandle(int sig){
    int s2HaveMsg = getValue(senderSemId, 5);
    if(s2HaveMsg == 0){
        thereIsMessage = 0;
    }else{
        char msg [MAX_MESSAGE_LENGTH];
        read(pipeId, msg, MAX_MESSAGE_LENGTH);

        time_t arrival;
        message *m = line2message(msg);

        char log[50];
        sprintf(log, "Receive %d from PIPE S2S3", m->id);
        printLog("S3", log);

        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        
        l = inserisciInCoda(l, t);
    }
}

void openResource(){
    // Open sender sem
    senderSemId = createSenderSemaphore();

    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);

    // Open MSGQ
    messageQueueId = getMessageQueue();

    // OPEN FIFO
    fifoId = openSenderFIFO();

    // Set signal for read from PIPE
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
        switch(m->receiver->number){
            case 1:
                sendToR1(messageQueueId, m);
                break;
            case 2:
                sendToR1(messageQueueId, m);
                break;
            case 3:
                sendToR1(messageQueueId, m);
                break;
            default:
                ErrExit("receiver not exist");
        }
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

	openResource();
/*
	signal(SIGUSR1, hacklerIncraseDelayHandle);
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);
*/
    // Set this process as end init 
    semOp(initSemId, SEM_INIT_SENDER, -1);

    // Wait all init end
    semOp(initSemId, SEM_END_INIT, 0);
    
    printLog("S3", "End init start");

	time_t departure;

	char log[50];
    node *tmp;
	trafficInfo *t;

	while(thereIsMessage || isSet(l)){
        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delayS3 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("S3", log);
                t->departure = departure;
		        printTrafficInfo(SENDER_3_FILENAME, t);
                sendMessage(t->message);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delayS3 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
	}	
    return closeResource();
}
