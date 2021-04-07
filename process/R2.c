/// @file R2.c
/// @brief Contiene l'implementazione del receiver 2.

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

node * l;
int receiverSemId;
int sharedMemoryId;
int thereIsMessage = 1;
int messageQueueId;
int pipeR1R2Id;
int pipeR2R3Id;
int R1pid;
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
    closePipe(pipeR1R2Id);
    // Close PIPE R2 R3
    closePipe(pipeR2R3Id);

    // Set this process as end
    semOp(receiverSemId, 1, -1);

	// Wait for 1 second befor end
    printLog("R1", "Process End");
	sleep(1);
	printLog("R1", "Process Exit");
	return 1;
}

void tryReadMSQ(){
    message * m =  readR2(messageQueueId);
    if(m != NULL){
        time_t arrival;

        char log[50];
        sprintf(log, "Receive %d from MessageQueue", m->id);
        printLog("R2", log);

        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        
        l = inserisciInCoda(l, t);
    }
}

void sendMessage(message* m){
    if(m->receiver->number == 2){
        char log[50];
        sprintf(log, "Message %d arrive", m->id);
        printLog("R2", log);
    }else{
        // Send to R1 by pipe
        char *message = message2line(m);
        write(pipeR1R2Id, message, MAX_MESSAGE_LENGTH);
        free(message);

        // Invio segnale a R1 di leggere dalla pipe
        kill(R1pid, SIGPIPE);

        printLog("R2", "Message send by PIPE R1R2");
    }
}

int main(int argc, char * argv[]) {
	printLog("R2", "Process start with exec");
	
    // ARGV: initSemId, pipeR1R2, pipeR2R3, SHMid, R1Pid
    int initSemId = atoi(argv[0]);
    pipeR1R2Id = atoi(argv[1]);
    pipeR2R3Id = atoi(argv[2]);
    sharedMemoryId = atoi(argv[3]);
    R1pid = atoi(argv[4]);

    openResource();

    // Set this process as end init
    semOp(initSemId, 2, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

    printLog("R2", "End init start");

    time_t departure;
    
    char log[50];

    node *tmp;
    trafficInfo *t;

    while(thereIsMessage || isSet(l)){
        // Try to read form msgqueue
        tryReadMSQ();

        // Try to read form shared memory

        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delayR2 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("R2", log);
                t->departure = departure;
		        printTrafficInfo(RECEIVER_2_FILENAME, t);
                sendMessage(t->message);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delayR2 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
	}
    
    return closeResource();
}
