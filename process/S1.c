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

node * l;
int size = 0;

int initSemId;
int sharedMemoryId;
char * sharedMemoryData;
int messageQueueId;
int pipeId;
int S2pid;

void hacklerIncraseDelayHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_INCREASE_DELAY);
	printLog("S1", log);
	// Increase delay of each message in list
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayS1 += 5;
		tmp = getNext(tmp);
	}
}

void hacklerRemoveMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_REMOVE_MSG);
	printLog("S1", log);
	// Remove each message in list
	while(isSet(l)){
		l = rimuovi(l, l->trafficInfo);
		size--;
		l = getNext(l);
	}
}

void hacklerSendMsgHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SEND_MSG);
	printLog("S1", log);
	// ciclo su tutti i messaggi setta a 0 i tempi d'attesa in modo che vengano inviati a fine sleep
	node * tmp = l;
	while(isSet(tmp)){
		tmp->trafficInfo->message->delayS1 = 0;
		tmp = getNext(tmp);
	}
}

int shutDown = 0;
void hacklerShutDownHandle(int sig){
	char log[50];
	sprintf(log, "Receive signal %s", HK_ACTION_SHUT_DOWN);
	printLog("S1", log);
	shutDown = 1;
}

void openResource(){
	// Open SHM
	sharedMemoryData = (char *) attachSharedMemory(sharedMemoryId, 0);
	
	// Set signal for incrase delay of all message in list
	if(signal(SIGUSR1, hacklerIncraseDelayHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalIncraseDelayHandle of S1");
	}
	// Set signal for remove all message in list
	if(signal(SIGUSR2, hacklerRemoveMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalRemoveMsgHandle of S1");
	}
	// Set signal for send all message in list
	if(signal(SIGCONT, hacklerSendMsgHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalSendMsgHandle of S1");
	}
	// Set signal for shut down the process
	if(signal(SIGTERM, hacklerShutDownHandle) == SIG_ERR){
		ErrExit("Impossibile settare signalShutDownHandle of S1");
	}

	// Init the output file
	printTrafficInfoHeader(SENDER_1_FILENAME);
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

void sendMessage(message * m){
	char log[50];
	if(m->sender->number == 1){
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
		// Send to S2 by pipe
		char * message = message2line(m);
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
		message * m = line2message(line);                
		sprintf(log, "Read %d from file", m->id);
		printLog("S1", log);
		time_t arrival;
		time(&arrival);
		trafficInfo * t = createTrafficInfo(m, arrival, arrival);
		l = inserisciInCoda(l, t);
		size++;
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
	
	node * tmp;
	trafficInfo * t;
	time_t departure;

	int eof = 1;

	while((eof || isSet(l)) && shutDown == 0){
		// try to read from file
		if(eof== 1 && size <= 10){
			eof = tryReadFromFile(1);
		}

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
				size--;
			}else{
				t->message->delayS1 -= 1;
				tmp = getNext(tmp);
			} 
		}
		sleep(1);
	}

	// Send to S2 that msg are end
	semOp(initSemId, SEM_S1_HAVE_MESSAGE_TO_SEND_BY_PIPE, -1);
	kill(S2pid, SIGPIPE);
	
	// Wait ShutDown from HK
	while(shutDown == 0){
		printLog("S1", "Wait ShutDown signal");
		pause();
	}

	// Close all resource
	closeResource();
	return 1;
}

