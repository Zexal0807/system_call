/// @file R1.c
/// @brief Contiene l'implementazione del receiver 1.

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

node * l;
int initSemId;
int sharedMemoryId;
int thereIsMessage = 1;
int messageQueueId;
int pipeId;
char * sharedMemoryData;

// SIGPIPE del R2
void readFromPipeHandle(int sig){
	char msg [MAX_MESSAGE_LENGTH];
	read(pipeId, msg, MAX_MESSAGE_LENGTH);

	time_t arrival;
	message * m = line2message(msg);

	char log[50] = "";
	sprintf(log, "Receive %d from PIPE R1R2", m->id);
	printLog("R1", log);
	time(&arrival);
	trafficInfo * t = createTrafficInfo(m, arrival, arrival);
	
	l = inserisciInCoda(l, t);
}

void hacklerIncraseDelayHandle(int sig){
	char log[50] = "";
	sprintf(log, "Receive signal %s", HK_ACTION_INCREASE_DELAY);
	printLog("R1", log);
	// Increase delay of each message in list
	node *tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayR1 += 5;
		tmp = getNext(tmp);
	}
}

void hacklerRemoveMsgHandle(int sig){
	char log[50] = "";
	sprintf(log, "Receive signal %s", HK_ACTION_REMOVE_MSG);
	printLog("R1", log);
	// Remove each message in list
	while(isSet(l)){
		l = rimuovi(l, l->trafficInfo);
		l = getNext(l);
	}
}

void hacklerSendMsgHandle(int sig){
	char log[50] = "";
	sprintf(log, "Receive signal %s", HK_ACTION_SEND_MSG);
	printLog("R1", log);
	// ciclo su tutti i messaggi setta a 0 i tempi d'attesa in modo che vengano inviati a fine sleep
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayR1 = 0;
		tmp = getNext(tmp);
	}
}

int shutDown = 0;
void hacklerShutDownHandle(int sig){
	char log[50] = "";
	sprintf(log, "Receive signal %s", HK_ACTION_SHUT_DOWN);
	printLog("R1", log);
	shutDown = 1;
}

void openResource(){
	// Open SHM
	sharedMemoryData = (char *) attachSharedMemory(sharedMemoryId, 0);
	
	// Set signal for read form pipe
	signal(SIGPIPE, readFromPipeHandle);

	// Set signal for incrase delay of all message in list
	if(signal(SIGUSR1, hacklerIncraseDelayHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalIncraseDelayHandle of R1");
	}
	// Set signal for remove all message in list
	if(signal(SIGUSR2, hacklerRemoveMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalRemoveMsgHandle of R1");
	}
	// Set signal for send all message in list
	if(signal(SIGCONT, hacklerSendMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalSendMsgHandle of R1");
	}
	// Set signal for shut down the process
	if(signal(SIGTERM, hacklerShutDownHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalShutDownHandle of R1");
	}

	// Init the output file
	printTrafficInfoHeader(RECEIVER_1_FILENAME);
}

void closeResource(){
	// Close SHM
	detachSharedMemory(sharedMemoryData);
	printLog("R1", "Detach shared memory");
	
	// Close MSGQ
	// Not need to be close

	// Close PIPE R1 R2
	closePipe(pipeId);

	// Set this process as end
	semOp(initSemId, SEM_R1_IS_RUNNING, -1);

	printLog("R1", "Process End");
}

void sendMessage(message * m){
	char log[50] = "";
	sprintf(log, "Message %d arrive", m->id);
	printLog("R1", log);
}

void testShutDown(){
	int s1IsRunning = getValue(initSemId, SEM_S1_IS_RUNNING);
	int s2IsRunning = getValue(initSemId, SEM_S2_IS_RUNNING);
	int s3IsRunning = getValue(initSemId, SEM_S3_IS_RUNNING);
	int r3IsRunning = getValue(initSemId, SEM_R3_IS_RUNNING);
	int r2IsRunning = getValue(initSemId, SEM_R2_IS_RUNNING);
	if(s1IsRunning == 0 && s2IsRunning == 0 && s3IsRunning == 0 && r3IsRunning == 0 && r2IsRunning == 0){
		thereIsMessage = 0;
	}
}

void tryReadSH(){
	int messageInSH = getValue(initSemId, SEM_SH);
	int messageForMe = getValue(initSemId, SEM_R1_SH);
	
	if(messageInSH == 0 && messageForMe == 1){
		message * m = line2message(sharedMemoryData);
		// Free the SH
		semOp(initSemId, SEM_SH, 1);
		time_t arrival;

		char log[50] = "";
		sprintf(log, "Receive %d from Shared Memory", m->id);
		printLog("R1", log);

		time(&arrival);
		trafficInfo * t = createTrafficInfo(m, arrival, arrival);
	
		l = inserisciInCoda(l, t);
	}
}

void tryReadMSQ(){
	message * m =  readR1(messageQueueId);
	if(m != NULL){
		time_t arrival;

		char log[50] = "";
		sprintf(log, "Receive %d from MessageQueue", m->id);
		printLog("R1", log);

		time(&arrival);
		trafficInfo * t = createTrafficInfo(m, arrival, arrival);
		
		l = inserisciInCoda(l, t);
	}
}

int main(int argc, char * argv[]) {

	printLog("R1", "Process start with exec");

	// ARGV: initSemId, pipeR1R2, sharedMemoryId, messageQueueId
	initSemId = atoi(argv[0]);
	pipeId = atoi(argv[1]);
	sharedMemoryId = atoi(argv[2]);
	messageQueueId = atoi(argv[3]);

	openResource();

	// Set this process as end init
	semOp(initSemId, SEM_INIT_RECEIVER, -1);

	printLog("R1", "End init");

	// Wait all init end
	semOp(initSemId, SEM_END_INIT, 0);

	time_t departure;
	
	node * tmp;
	trafficInfo * t;

	while((thereIsMessage || isSet(l)) && shutDown == 0){
		// Check if the sender are still running
		testShutDown();

		// Try to read form msgqueue
		tryReadMSQ();

		// Try to read form shared memory
		tryReadSH();

		tmp = l;
		while(isSet(tmp)){
			t = tmp->trafficInfo;
			if(t->message->delayR1 <= 0){
				time(&departure);
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
	
	// Wait ShutDown from HK
	while(shutDown == 0){
		printLog("R1", "Wait ShutDown signal");
		pause();
	}

	// Close all resource
	closeResource();
	return 1;
}
