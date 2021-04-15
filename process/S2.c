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

node *l = NULL;
int initSemId;
int sharedMemoryId;
message * sharedMemoryData;
int messageQueueId;
int S3pid;
int pipeS1S2Id;
int pipeS2S3Id;
int thereIsMessage = 1;

// SIGPIPE del S1
void readFromPipeHandle(int sig){
    int s1HaveMsg = getValue(initSemId, SEM_S1_HAVE_MESSAGE_TO_SEND_BY_PIPE);
    if(s1HaveMsg == 0){
        thereIsMessage = 0;
    }else{
        char msg [MAX_MESSAGE_LENGTH];
        read(pipeS1S2Id, msg, MAX_MESSAGE_LENGTH);

        time_t arrival;
        message *m = line2message(msg);

        char log[50];
        sprintf(log, "Receive %d from PIPE S1S2", m->id);
        printLog("S2", log);

        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        
        l = inserisciInCoda(l, t);
    }
}

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    
    // Open MSGQ
    messageQueueId = getMessageQueue();

    // Set signal for read form pipe
    signal(SIGPIPE, readFromPipeHandle);
}

void closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("S2", "Detach shared memory");
    
    // Close MSGQ
    // Not need to be close

	// Close PIPE S2 S3
    closePipe(pipeS2S3Id);

    // Close PIPE S1 S2
    closePipe(pipeS1S2Id);

    // Set this process as end
    semOp(initSemId, SEM_S2_IS_RUNNING, -1);

	printLog("S2", "Process End");
}

void sendMessage(message* m){
    char log[50];
    if(m->sender->number == 2){
        if (strcmp(m->comunication, "Q") == 0) {
            switch(m->receiver->number){
                case 1:
                    sendToR1(messageQueueId, m);
                    break;
                case 2:
                    sendToR2(messageQueueId, m);
                    break;
                case 3:
                    sendToR3(messageQueueId, m);
                    break;
                default:
                    ErrExit("receiver not exist");
            }
            sprintf(log, "Message %d send by MessageQueue", m->id);
        }else if (strcmp(m->comunication, "SH") == 0) {
            sprintf(log, "Message %d send by SharedMemory", m->id);
        }
    }else{
        // Send to S3 by pipe
        char *message = message2line(m);
        write(pipeS2S3Id, message, MAX_MESSAGE_LENGTH);
        free(message);

        // Invio segnale a S3 di leggere dalla pipe
        kill(S3pid, SIGPIPE);

        sprintf(log, "Message %d send by PIPE S2S3", m->id);
    }
    printLog("S2", log);
}

int main(int argc, char * argv[]) {

    printLog("S2", "Process start with exec");

    // ARGV: initSemId, PIPE_S2S3, PIPE_S2S3, S3pid
    initSemId = atoi(argv[0]);
    pipeS1S2Id = atoi(argv[1]);
    pipeS2S3Id = atoi(argv[2]);
    sharedMemoryId = atoi(argv[3]);
    S3pid = atoi(argv[4]);

	openResource();

/*
	signal(SIGUSR1, hacklerIncraseDelayHandle);
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);
*/

    // Set this process as end init 
    semOp(initSemId, SEM_INIT_SENDER, -1);

    printLog("S2", "End init");

    // Wait all init end
    semOp(initSemId, SEM_END_INIT, 0);

	time_t departure;
    node *tmp;
    trafficInfo *t;
	char log[50];

	while(thereIsMessage || isSet(l)){
        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delayS2 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("S2", log);
                t->departure = departure;
		        printTrafficInfo(SENDER_2_FILENAME, t);
                sendMessage(t->message);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delayS2 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
	}

    // Send to S3 that msg are end
    semOp(initSemId, SEM_S2_HAVE_MESSAGE_TO_SEND_BY_PIPE, -1);
    kill(S3pid, SIGPIPE);

    // Close all resource
    closeResource();

    // Wait ShutDown from HK
    pause();
    return 1;
}
