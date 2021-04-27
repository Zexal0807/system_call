/// @file R2.c
/// @brief Contiene l'implementazione del receiver 2.

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
int pipeR1R2Id;
int pipeR2R3Id;
int R1pid;
char *sharedMemoryData;

// SIGPIPE del R3
void readFromPipeHandle(int sig){
	char msg [MAX_MESSAGE_LENGTH];
	read(pipeR2R3Id, msg, MAX_MESSAGE_LENGTH);

	time_t arrival;
	message * m = line2message(msg);

	char log[50];
	sprintf(log, "Receive %d from PIPE R2R3", m->id);
	printLog("R2", log);
	time(&arrival);
	trafficInfo *t = createTrafficInfo(m, arrival, arrival);
	
	l = inserisciInCoda(l, t);
}

void hacklerIncraseDelayHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_INCREASE_DELAY);
	printLog("R2", log);
	// Increase delay of each message in list
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayR2 += 5;
		tmp = getNext(tmp);
	}
}

void hacklerRemoveMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_REMOVE_MSG);
	printLog("R2", log);
	// Remove each message in list
	while(isSet(l)){
		l = rimuovi(l, l->trafficInfo);
		l = getNext(l);
	}
}

void hacklerSendMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SEND_MSG);
	printLog("R2", log);
	// ciclo su tutti i messaggi setta a 0 i tempi d'attesa in modo che vengano inviati a fine sleep
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayR2 = 0;
		tmp = getNext(tmp);
	}
}

int shutDown = 0;
void hacklerShutDownHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SHUT_DOWN);
	printLog("R2", log);
	shutDown = 1;
}

void openResource(){
	// Open SHM
	sharedMemoryData = (char *) attachSharedMemory(sharedMemoryId, 0);
	
	// Set signal for read form pipe
	signal(SIGPIPE, readFromPipeHandle);

	// Set signal for incrase delay of all message in list
	if(signal(SIGUSR1, hacklerIncraseDelayHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalIncraseDelayHandle of R2");
	}
	// Set signal for remove all message in list
	if(signal(SIGUSR2, hacklerRemoveMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalRemoveMsgHandle of R2");
	}
	// Set signal for send all message in list
	if(signal(SIGCONT, hacklerSendMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalSendMsgHandle of R2");
	}
	// Set signal for shut down the process
	if(signal(SIGTERM, hacklerShutDownHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalShutDownHandle of R2");
	}
}

void closeResource(){
	// Close SHM
	detachSharedMemory(sharedMemoryData);
	printLog("R2", "Detach shared memory");
	
	// Close MSGQ
	// Not need to be close

	// Close PIPE R1 R2
	closePipe(pipeR1R2Id);
	// Close PIPE R2 R3
	closePipe(pipeR2R3Id);

	// Set this process as end
	semOp(initSemId, SEM_R2_IS_RUNNING, -1);

	printLog("R2", "Process End");
}

void testShutDown(){
	int s1IsRunning = getValue(initSemId, SEM_S1_IS_RUNNING);
	int s2IsRunning = getValue(initSemId, SEM_S2_IS_RUNNING);
	int s3IsRunning = getValue(initSemId, SEM_S3_IS_RUNNING);
	int r3IsRunning = getValue(initSemId, SEM_R3_IS_RUNNING);
	if(s1IsRunning == 0 && s2IsRunning == 0 && s3IsRunning == 0 && r3IsRunning == 0){
		thereIsMessage = 0;
	}
}

void tryReadSH(){
	int messageInSH = getValue(initSemId, SEM_SH);
	int messageForMe = getValue(initSemId, SEM_R2_SH);
	
	if(messageInSH == 0 && messageForMe == 1){
		message * m = line2message(sharedMemoryData);
		// Free the SH
		semOp(initSemId, SEM_SH, 1);
		time_t arrival;

		char log[50];
		sprintf(log, "Receive %d from Shared Memory", m->id);
		printLog("R2", log);

		time(&arrival);
		trafficInfo *t = createTrafficInfo(m, arrival, arrival);
	
		l = inserisciInCoda(l, t);
	}
}

void tryReadMSQ(){
	message * m =  readR2(messageQueueId);
	if(m != NULL){
		time_t arrival;

		char log[50];
		sprintf(log, "Receive %d from MessageQueue", m->id);
		printLog("R2", log);

		time(&arrival);
		trafficInfo * t = createTrafficInfo(m, arrival, arrival);
		
		l = inserisciInCoda(l, t);
	}
}

void sendMessage(message * m){
	char log[50];
	sprintf(log, "Message %d send by PIPE R1R2", m->id);

	// Send to R1 by pipe
	char * message = message2line(m);
	write(pipeR1R2Id, message, MAX_MESSAGE_LENGTH);
	free(message);

	// Invio segnale a R1 di leggere dalla pipe
	kill(R1pid, SIGPIPE);

	printLog("R2", log);
}

int main(int argc, char * argv[]) {
	printLog("R2", "Process start with exec");
	
	// ARGV: initSemId, pipeR1R2, pipeR2R3, SHMid, R1Pid, messageQueueId
	initSemId = atoi(argv[0]);
	pipeR1R2Id = atoi(argv[1]);
	pipeR2R3Id = atoi(argv[2]);
	sharedMemoryId = atoi(argv[3]);
	R1pid = atoi(argv[4]);
	messageQueueId = atoi(argv[5]);

	openResource();

	// Set this process as end init
	semOp(initSemId, SEM_INIT_RECEIVER, -1);

	printLog("R2", "End init");

	// Wait all init end
	semOp(initSemId, SEM_END_INIT, 0);

	time_t departure;
	
	char log[50];

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
	
	// Wait ShutDown from HK
	while(shutDown == 0){
		printLog("R2", "Wait ShutDown signal");
		pause();
	}

	// Close all resource
	closeResource();
	return 1;
}
