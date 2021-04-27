/// @file S3.c
/// @brief Contiene l'implementazione del sender 3.

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "../err_exit.h"
#include "../defines.h"
#include "../struct/message.h"
#include "../shared_memory.h"
#include "../message_queue.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

node * l;
int initSemId;
int sharedMemoryId;
char * sharedMemoryData;
int messageQueueId;
int pipeId ;
int fifoId ;
int thereIsMessage = 1;

// SIGPIPE del S2
void readFromPipeHandle(int sig){
	int s2HaveMsg = getValue(initSemId, SEM_S2_HAVE_MESSAGE_TO_SEND_BY_PIPE);
	if(s2HaveMsg == 0){
		thereIsMessage = 0;
	}else{
		char msg [MAX_MESSAGE_LENGTH];
		read(pipeId, msg, MAX_MESSAGE_LENGTH);

		time_t arrival;
		message * m = line2message(msg);

		char log[50];
		sprintf(log, "Receive %d from PIPE S2S3", m->id);
		printLog("S3", log);

		time(&arrival);
		trafficInfo * t = createTrafficInfo(m, arrival, arrival);
		
		l = inserisciInCoda(l, t);
	}
}

void hacklerIncraseDelayHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_INCREASE_DELAY);
	printLog("S3", log);
	// Increase delay of each message in list
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayS3 += 5;
		tmp = getNext(tmp);
	}
}

void hacklerRemoveMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_REMOVE_MSG);
	printLog("S3", log);
	// Remove each message in list
	while(isSet(l)){
		l = rimuovi(l, l->trafficInfo);
		l = getNext(l);
	}
}

void hacklerSendMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SEND_MSG);
	printLog("S3", log);
	// ciclo su tutti i messaggi setta a 0 i tempi d'attesa in modo che vengano inviati a fine sleep
	node *tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayS3 = 0;
		tmp = getNext(tmp);
	}
}

int shutDown = 0;
void hacklerShutDownHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SHUT_DOWN);
	printLog("S3", log);
	shutDown = 1;
}

void openResource(){
	// Open SHM
	sharedMemoryData = (char *) attachSharedMemory(sharedMemoryId, 0);
	
	// OPEN FIFO
	fifoId = openSenderFIFO();

	// Set signal for read from PIPE
	signal(SIGPIPE, readFromPipeHandle);

	// Set signal for incrase delay of all message in list
	if(signal(SIGUSR1, hacklerIncraseDelayHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalIncraseDelayHandle of S3");
	}
	// Set signal for remove all message in list
	if(signal(SIGUSR2, hacklerRemoveMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalRemoveMsgHandle of S3");
	}
	// Set signal for send all message in list
	if(signal(SIGCONT, hacklerSendMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalSendMsgHandle of S3");
	}
	// Set signal for shut down the process
	if(signal(SIGTERM, hacklerShutDownHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalShutDownHandle of S3");
	}
}

void closeResource(){
	// Close SHM
	detachSharedMemory(sharedMemoryData);
	printLog("S3", "Detach shared memory");
	
	// Close MSGQ
	// Not need to be close

	// Close FIFO
	close(fifoId);

	// Set this process as end
	semOp(initSemId, SEM_S3_IS_RUNNING, -1);

	printLog("S3", "Process End");
}

void sendMessage(message * m){
	char log[50];
	if (strcmp(m->comunication, "Q") == 0) {
		switch(m->receiver->number){
			case 1:
				Q_writeForR1(messageQueueId, m);
				break;
			case 2:
				Q_writeForR2(messageQueueId, m);
				break;
			case 3:
				Q_writeForR3(messageQueueId, m);
				break;
			default:
				ErrExit("receiver not exist");
		}
		sprintf(log, "Message %d send by MessageQueue", m->id);
	}else if (strcmp(m->comunication, "SH") == 0) {
		switch(m->receiver->number){
			case 1:
				SH_writeForR1(sharedMemoryData, m, initSemId);
				break;
			case 2:
				SH_writeForR2(sharedMemoryData, m, initSemId);
				break;
			case 3:
				SH_writeForR3(sharedMemoryData, m, initSemId);
				break;
			default:
				ErrExit("receiver not exist");
		}
		sprintf(log, "Message %d send by SharedMemory", m->id);
	}else if (strcmp(m->comunication, "FIFO") == 0) {
		char * msg = message2line(m);
		writeFIFO(fifoId, msg);

		sprintf(log, "Message %d send by FIFO", m->id);
		
		// Set that FIFO have a message
		semOp(initSemId, SEM_MESSAGE_IN_FIFO, -1);
	}
	printLog("S3", log);
}

int main(int argc, char * argv[]) {

	printLog("S3", "Process start with exec");

	// ARGV: initSemId, PIPE_S2S3, messageQueueId
	initSemId = atoi(argv[0]);
	pipeId = atoi(argv[1]);
	sharedMemoryId = atoi(argv[2]);
	messageQueueId = atoi(argv[3]);

	openResource();

	// Set this process as end init 
	semOp(initSemId, SEM_INIT_SENDER, -1);

	printLog("S3", "End init");

	// Wait all init end
	semOp(initSemId, SEM_END_INIT, 0);

	time_t departure;

	char log[50];
	node * tmp;
	trafficInfo * t;

	while((thereIsMessage || isSet(l)) && shutDown == 0){
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
				t->message->delayS3 -= 1;
				tmp = getNext(tmp);
			} 
		}
		sleep(1);
	}
	
	// Wait ShutDown from HK
	while(shutDown == 0){
		printLog("S3", "Wait ShutDown signal");
		pause();
	}

	// Close all resource
	closeResource();
	return 1;
}
