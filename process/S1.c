/// @file S1.c
/// @brief Contiene l'implementazione del sender 1.
#include <stdio.h>
#include <unistd.h>

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

node *l;

// SIGPIPE For sync to next PIPE

void hacklerIncraseDelayHandle(int sig){
// SIGUSR1 del IncraseDelay dell HK (ciclo su tutti i messagi e aumenta il time)
    node *tmp = l;
    while(isSet(tmp)){
        tmp->message->delay1 += 5;
        tmp = getNext(tmp);
    }
}

void hacklerRemoveMsgHandle(int sig){
// SIGUSR2 del RemoveMsg del HK (ciclo su tutti imessaggi e remove, poi se serve mandare SIGPIPE)
}
void hacklerSendMsgHandle(int sig){
// SIGCONT  risveglia (annulla la sleep) + ciclo su tutti imessaggi setta a 0 i tempi d'attesa del SendMessage del HK
    node *tmp = l;
    while(isSet(tmp)){
        tmp->message->delay1 = 0;
        tmp = getNext(tmp);
    }
}
void hacklerShutDownHandle(int sig){
// SIGTERM ShutDown del HK

}

void sendMessage(message* m){
    sleep(m->delay1);
    printLog("S1", "Message can be send");
    if(m->sender->number == 1){
        // Send message whit correct channel

    }else{
        // Send to S2 by pipe
        
    }
}

int main(int argc, char * argv[]) {

	printLog("S1", "Process start with exec");

    // ARGV: initSemId, inputFile
    int initSemId = atoi(argv[0]);
	char *filename = argv[1];

    // Open sender sem
    int senderSemId = createSenderSemaphore();

    // Open SHM
    // Open MSGQ
    // Open PIPE S1 S2

	*l = createMessageList(filename);
    
    signal(SIGUSR1, hacklerIncraseDelayHandle);
    signal(SIGUSR2, hacklerRemoveMsgHandle);
    signal(SIGCONT, hacklerSendMsgHandle);
    signal(SIGTERM, hacklerShutDownHandle);


    // Set this process as end init
    semOp(initSemId, 1, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

	char log[50];
	sprintf(log, "Loaded message from file %s", filename);
	printLog("S1", log);
	
	time_t arrival;
	time_t departure;

	while(isSet(l)){
		time(&arrival);
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
    //kill()
    

    // Close SHM
    // Close MSGQ

    // Wait S2 end
    semOp(senderSemId, 2, 0);

    // Close PIPE S1 S2

    // Set this process as end
    semOp(senderSemId, 1, -1);

	// Wait for 1 second befor end
	sleep(1);
	printLog("S1", "Process End");
	return 1;
}
