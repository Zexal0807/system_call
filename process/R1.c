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
int initSemId;
int sharedMemoryId;
int thereIsMessage = 1;
int messageQueueId;
int pipeId;
message *sharedMemoryData;

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    // Open MSGQ
    messageQueueId = getMessageQueue();
}

int closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("R1", "Detach shared memory");
    
    // Close MSGQ
    // Not need to be close

    // Wait R2 end
    printLog("R1", "Wait R2");
    semOp(initSemId, SEM_R2_IS_RUNNING, 0);
    
    // Close PIPE R1 R2
    closePipe(pipeId);

    // Set this process as end
    semOp(initSemId, SEM_R1_IS_RUNNING, -1);

	// Wait for 1 second befor end
    printLog("R1", "Process End");
	sleep(1);
	printLog("R1", "Process Exit");
	return 1;
}

void sendMessage(message* m){
    char log[50];

    printf("..%s...\n", message2line(m));

    if(m->receiver->number == 1){
        sprintf(log, "Message %d arrive", m->id);
        printLog("R1", log);
    }else{
        sprintf(log, "Error with message %d", m->id);
        printLog("R1", log);
    }
}

void testShutDown(){
    int s1IsRunning = getValue(initSemId, SEM_S1_IS_RUNNING);
    if(s1IsRunning == 0){
        thereIsMessage = 0;
    }
}

void tryReadMSQ(){
    message * m =  readR1(messageQueueId);
    if(m != NULL){
        time_t arrival;

        char log[50];
        sprintf(log, "Receive %d from MessageQueue", m->id);
        printLog("R1", log);

        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        
        l = inserisciInCoda(l, t);
    }
}

int main(int argc, char * argv[]) {

	printLog("R1", "Process start with exec");

    // ARGV: initSemId, pipeR1R2, sharedMemoryId
    initSemId = atoi(argv[0]);
    pipeId = atoi(argv[1]);
    sharedMemoryId = atoi(argv[2]);

    openResource();

    // Set this process as end init
    semOp(initSemId, SEM_INIT_RECEIVER, -1);

    printLog("R1", "End init");

    // Wait all init end
    semOp(initSemId, SEM_END_INIT, 0);

	time_t departure;
    
    char log[50];

    node *tmp;
    trafficInfo *t;

    while(thereIsMessage || isSet(l)){
        // Check if the sender are still running
        testShutDown();

        // Try to read form msgqueue
        tryReadMSQ();

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
                sendMessage(t->message);
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
}
