/// @file R3.c
/// @brief Contiene l'implementazione del receiver 3.

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
int initSemId;
int sharedMemoryId;
int thereIsMessage = 1;
int messageQueueId;
int pipeId;
message *sharedMemoryData;
int R2pid;
int fifoId;

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    
    // Open MSGQ
    messageQueueId = getMessageQueue();

    // Open FIFO
    fifoId = openReceiverFIFO();
}

int closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("R3", "detachSharedMemory");
    
    // Close MSGQ
    // Not need to be close
    
    // Close PIPE R2 R3
    closePipe(pipeId);

    // Set this process as end
    semOp(initSemId, SEM_R3_IS_RUNNING, -1);

	// Wait for 1 second befor end
    printLog("R3", "Process End");
	sleep(3);
	printLog("R3", "Process Exit");
	return 1;
}


void testShutDown(){
    int s1IsRunning = getValue(initSemId, SEM_S1_IS_RUNNING);
    if(s1IsRunning == 0){
        thereIsMessage = 0;
    }
}

void tryReadFIFO(){
    int thereMessageInFIFO = getValue(initSemId, SEM_MESSAGE_IN_FIFO);
    if(thereMessageInFIFO == 0){
        char msg [MAX_MESSAGE_LENGTH];
        readFIFO(fifoId, msg);

        // Free the fifo
        semOp(initSemId, SEM_MESSAGE_IN_FIFO, 1);

        time_t arrival;
        message *m = line2message(msg);

        char log[50];
        sprintf(log, "Receive %d from FIFO", m->id);
        printLog("R3", log);

        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        
        l = inserisciInCoda(l, t);
    } 
}

void tryReadMSQ(){
    message * m =  readR3(messageQueueId);
    if(m != NULL){
        time_t arrival;

        char log[50];
        sprintf(log, "Receive %d from MessageQueue", m->id);
        printLog("R3", log);

        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        
        l = inserisciInCoda(l, t);
    }
}

void sendMessage(message* m){
    if(m->receiver->number == 3){
        char log[50];
        sprintf(log, "Message %d arrive", m->id);
        printLog("R3", log);
    }else{
        // Send to R2 by pipe
        char *message = message2line(m);
        write(pipeId, message, MAX_MESSAGE_LENGTH);
        free(message);

        // Invio segnale a R1 di leggere dalla pipe
        kill(R2pid, SIGPIPE);

        printLog("R3", "Message send by PIPE R2R3");
    }
}

int main(int argc, char * argv[]) {

	printLog("R3", "Process start with exec");
	
    // ARGV: initSemId
    initSemId = atoi(argv[0]);
    pipeId = atoi(argv[1]);
    sharedMemoryId = atoi(argv[2]);
    R2pid = atoi(argv[3]);

    openResource();
    // Set this process as end init
    semOp(initSemId, SEM_INIT_RECEIVER, -1);

    // Wait all init end
    semOp(initSemId, SEM_END_INIT, 0);

    printLog("R3", "End init start");

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

        // Try to read from FIFO
        tryReadFIFO();

        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delayR3 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("R3", log);
                t->departure = departure;
		        printTrafficInfo(RECEIVER_3_FILENAME, t);
                sendMessage(t->message);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delayR3 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
	}
    
    return closeResource();
}
