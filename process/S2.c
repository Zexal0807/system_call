/// @file S2.c
/// @brief Contiene l'implementazione del sender 2.

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

node * l = NULL;
int initSemId;
int sharedMemoryId;
char * sharedMemoryData;
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
		message * m = line2message(msg);

		char log[50];
		sprintf(log, "Receive %d from PIPE S1S2", m->id);
		printLog("S2", log);

		time(&arrival);
		trafficInfo * t = createTrafficInfo(m, arrival, arrival);
		
		l = inserisciInCoda(l, t);
	}
}

void hacklerIncraseDelayHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_INCREASE_DELAY);
	printLog("S2", log);
	// Increase delay of each message in list
	node *tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayS2 += 5;
		tmp = getNext(tmp);
	}
}

void hacklerRemoveMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_REMOVE_MSG);
	printLog("S2", log);
	// Remove each message in list
	while(isSet(l)){
		l = rimuovi(l, l->trafficInfo);
		l = getNext(l);
	}
}

void hacklerSendMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SEND_MSG);
	printLog("S2", log);
	// ciclo su tutti i messaggi setta a 0 i tempi d'attesa in modo che vengano inviati a fine sleep
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayS2 = 0;
		tmp = getNext(tmp);
	}
}

int shutDown = 0;
void hacklerShutDownHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SHUT_DOWN);
	printLog("S2", log);
	shutDown = 1;
}

void openResource(){
	// Open SHM
	sharedMemoryData = (char *) attachSharedMemory(sharedMemoryId, 0);
	
	// Set signal for read form pipe
	signal(SIGPIPE, readFromPipeHandle);

	// Set signal for incrase delay of all message in list
	if(signal(SIGUSR1, hacklerIncraseDelayHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalIncraseDelayHandle of S2");
	}
	// Set signal for remove all message in list
	if(signal(SIGUSR2, hacklerRemoveMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalRemoveMsgHandle of S2");
	}
	// Set signal for send all message in list
	if(signal(SIGCONT, hacklerSendMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalSendMsgHandle of S2");
	}
	// Set signal for shut down the process
	if(signal(SIGTERM, hacklerShutDownHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalShutDownHandle of S2");
	}
	
	// Init the output file
	printHeader(SENDER_2_FILENAME);
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

void sendMessage(message * m){
	char log[50];
	if(m->sender->number == 2){
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
		}
	}else{
		// Send to S3 by pipe
		char * message = message2line(m);
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

	// ARGV: initSemId, PIPE_S2S3, PIPE_S2S3, S3pid,  messageQueueId
	initSemId = atoi(argv[0]);
	pipeS1S2Id = atoi(argv[1]);
	pipeS2S3Id = atoi(argv[2]);
	sharedMemoryId = atoi(argv[3]);
	S3pid = atoi(argv[4]);
	messageQueueId = atoi(argv[5]);

	openResource();

	// Set this process as end init 
	semOp(initSemId, SEM_INIT_SENDER, -1);

	printLog("S2", "End init");

	// Wait all init end
	semOp(initSemId, SEM_END_INIT, 0);

	time_t departure;
	node * tmp;
	trafficInfo * t;
	char log[50];

	while((thereIsMessage || isSet(l)) && shutDown == 0){
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
				t->message->delayS2 -= 1;
				tmp = getNext(tmp);
			} 
		}
		sleep(1);
	}

	// Send to S3 that msg are end
	semOp(initSemId, SEM_S2_HAVE_MESSAGE_TO_SEND_BY_PIPE, -1);
	kill(S3pid, SIGPIPE);

	
	// Wait ShutDown from HK
	while(shutDown == 0){
		printLog("S2", "Wait ShutDown signal");
		pause();
	}

	// Close all resource
	closeResource();
	return 1;
}
