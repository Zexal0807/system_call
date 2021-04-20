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
int sharedMemoryId;
message * sharedMemoryData;
int messageQueueId;
int pipeId;
int S2pid;

void hacklerIncraseDelayHandle(int sig){
    // Increase delay of each message in list
    node *tmp = l;
    while(isSet(tmp)){
        tmp->trafficInfo->message->delayS1 += 5;
        tmp = getNext(tmp);
    }
}

void openResource(){
    // Open SHM
    sharedMemoryData = (message *) attachSharedMemory(sharedMemoryId, 0);
    
    // Set signal for incrase delay of all message in list
    if(signal(SIGUSR1, hacklerIncraseDelayHandle) == SIG_ERR){
        ErrExit("Impossibile settare signalIncraseDelayHandle of S1");
    }
    
    /*
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);
    */
}

void closeResource(){
    // Close SHM
    detachSharedMemory(sharedMemoryData);
    printLog("S1", "Detach shared memory");
    
    // Close MSGQ
    // Not need to be close
    
    // Close PIPE S1 S2
    closePipe(pipeId);

    // Set this process as end
    semOp(initSemId, SEM_S1_IS_RUNNING, -1);

    printLog("S1", "Process End");
}


/*
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
}*/

void sendMessage(message* m){
    char log[50];
    if(m->sender->number == 1){
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
            //Controllo che il segmento sia libero
            /*
            semOp(initSemId, SEM_SH, -1);

            switch(m->receiver->number){
                case 1:
                    SHtoR1(sharedMemoryData, m, initSemId);
                    break;
                case 2:
                    SHtoR2(sharedMemoryData, m, initSemId);
                    break;
                case 3:
                    SHtoR3(sharedMemoryData, m, initSemId);
                    break;
                default:
                    ErrExit("receiver not exist");
            }
            sprintf(log, "Message %d send by SharedMemory", m->id); 
            */
        }
    }else{
        // Send to S2 by pipe
        char *message = message2line(m);
        write(pipeId, message, MAX_MESSAGE_LENGTH);
        free(message);

        // Invio segnale a S2 di leggere dalla pipe
        kill(S2pid, SIGPIPE);

        sprintf(log, "Message %d send by PIPE S1S2", m->id);
    }
    printLog("S1", log);
}

int file;
int cursor = 0L;
int filesize = 0;

int tryReadFromFile(int convert){
    char log[50];
    lseek(file, cursor, SEEK_SET);
    char buffer[MAX_MESSAGE_LENGTH];
    if(read(file, buffer, MAX_MESSAGE_LENGTH) == -1){
        ErrExit("Impossibile leggere dal file");
    }

    char * line = strtok(buffer, "\n");
    int length = strlen(line);
    cursor += length + 1;

    if(convert){
        message *m = line2message(line);                
        sprintf(log, "Read %d from file", m->id);
        printLog("S1", log);
        time_t arrival;
        time(&arrival);
        trafficInfo *t = createTrafficInfo(m, arrival, arrival);
        l = inserisciInCoda(l, t);
    }

    if(cursor >= filesize){
        return 0;
    }

    return 1;
}

int main(int argc, char * argv[]) {

	printLog("S1", "Process start with exec");

    // ARGV: initSemId, PIPE_S1S2, shmId, S2pid, messageQueueId, inputFile

    initSemId = atoi(argv[0]);
    pipeId = atoi(argv[1]);
    sharedMemoryId = atoi(argv[2]);
    S2pid = atoi(argv[3]);
    messageQueueId = atoi(argv[4]);
	char *filename = argv[5];

    char log[50];

    openResource();

    file = openFile(filename);
    filesize = lseek(file, 0L, SEEK_END);
    tryReadFromFile(0);


    // Set this process as end init
    semOp(initSemId, SEM_INIT_SENDER, -1);

    printLog("S1", "End init");

    // Wait all init end
    semOp(initSemId, SEM_END_INIT, 0);
    
    node *tmp;
    trafficInfo *t;
	time_t departure;

    int eof = 1;

    while(eof || isSet(l)){
        // try to read from file
        // TODO: aggiungere un max message in list
        if(eof== 1){
            eof = tryReadFromFile(1);
        }

        printf("S1 list: ");
        printList(l);
        printf("\n");

        tmp = l;
        while(isSet(tmp)){
            t = tmp->trafficInfo;
            if(t->message->delayS1 <= 0){
                time(&departure);
                sprintf(log, "Message %d can be send", t->message->id);
		        printLog("S1", log);
                t->departure = departure;
		        printTrafficInfo(SENDER_1_FILENAME, t);
                sendMessage(t->message);
                tmp = getNext(tmp);
                l = rimuovi(l, t);
            }else{
                t->message->delayS1 -=1;
                tmp = getNext(tmp);
            } 
        }
        sleep(1);
    }

    // Send to S2 that msg are end
    semOp(initSemId, SEM_S1_HAVE_MESSAGE_TO_SEND_BY_PIPE, -1);
    kill(S2pid, SIGPIPE);
    
    // Close all resource
    closeResource();

    // Wait ShutDown from HK
    pause();
    return 1;
}

