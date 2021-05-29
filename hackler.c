/// @file hackler.c
/// @brief Contiene l'implementazione del hackler.
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"
#include "struct/history.h"

int pidS1 = 0;
int pidS2 = 0;
int pidS3 = 0;
int pidR1 = 0;
int pidR2 = 0;
int pidR3 = 0;

void printIPCHistory(int semId, history * h){
	time_t timeIPC;

	// Set distruction time
	time(&timeIPC);
	h->distruction = timeIPC;

	// Wait can write on file
	semOp(semId, SEM_HISTORY_FILE, -1);

	// Write
	printHistory(IPC_HISTORY_FILENAME, h);

	// Free file
	semOp(semId, SEM_HISTORY_FILE, 1);
}

void string2pid(char * line){
	char * saveptr1;
	char * process = strtok_r(line, ";", &saveptr1);
	char * pid = strtok_r(NULL, ";", &saveptr1);

	if(strcmp(process, "S1") == 0){
		pidS1 = atoi(pid);
	}else if(strcmp(process, "S2") == 0){
		pidS2 = atoi(pid);
	}else if(strcmp(process, "S3") == 0){
		pidS3 = atoi(pid);
	}else if(strcmp(process, "R1") == 0){
		pidR1 = atoi(pid);
	}else if(strcmp(process, "R2") == 0){
		pidR2 = atoi(pid);
	}else if(strcmp(process, "R3") == 0){
		pidR3 = atoi(pid);
	}else{
		ErrExit("Error in File");
	}
};

void readFrom(char * filename){
	int file = openFile(filename);
	ErrOpen(file);
	
	int dim = lseek(file, 0L, SEEK_END);
	lseek(file, 0L, SEEK_SET);
	
	char *buffer = (char *) malloc((dim) * sizeof(char));

	read(file, buffer, dim);

	char * saveptr;
	char * token=strtok_r(buffer, "\n", &saveptr);
	//salto la prima riga
	token = strtok_r(NULL, "\n", &saveptr);
	//analizzo il resto del testo
	while(token != NULL){
		string2pid(token);
		token = strtok_r(NULL, "\n", &saveptr);
	}
}

void readPid(int initSemId){
	char log[50] = "";

	// Wait sender init end 
	semOp(initSemId, SEM_INIT_SENDER, 0);

	// Read PID form sender file
	readFrom(SENDER_FILENAME);
	
	sprintf(log, "Read pid: S1(%d), S2(%d), S3(%d)", pidS1, pidS2, pidS3);
	printLog("HK", log);

	// Wait receiver init end 
	semOp(initSemId, SEM_INIT_RECEIVER, 0);

	// Read PID form receiver file
	readFrom(RECEIVER_FILENAME);

	sprintf(log, "Read pid: R1(%d), R2(%d), R3(%d)", pidR1, pidR2, pidR3);
	printLog("HK", log);
}

void executeAction(hacklerAction * h){
	char log[50] = "";

	process * target = h->target;

	if(target->type == 'Z' && target->number == 0){
		// All
		h->target = SENDER_1();
		executeAction(h);
		h->target = SENDER_2();
		executeAction(h);
		h->target = SENDER_3();
		executeAction(h);
		h->target = RECEIVER_1();
		executeAction(h);
		h->target = RECEIVER_2();
		executeAction(h);
		h->target = RECEIVER_3();
		executeAction(h);

		char action [50] = "";
		strncpy(action, h->action, strlen(h->action) - 1);

		sprintf(log, "Send action %d (%s) to ALL", 
			h->id,
			action
		);
		printLog("HK", log);
	}else{
		int targetPid = -1;

		if(target->type == 'S' && target->number == 1){
			targetPid = pidS1;
		}else if(target->type == 'S' && target->number == 2){
			targetPid = pidS2;
		}else if(target->type == 'S' && target->number == 3){
			targetPid = pidS3;
		}else if(target->type == 'R' && target->number == 1){
			targetPid = pidR1;
		}else if(target->type == 'R' && target->number == 2){
			targetPid = pidR2;
		}else if(target->type == 'R' && target->number == 3){
			targetPid = pidR3;
		}
		
		if(targetPid <= 0){
			ErrExit("Impossibile inviare action (target not found)");
		}

		int sig = -1;
		char action[50] = "";

		if(strncmp(h->action, HK_ACTION_INCREASE_DELAY, strlen(HK_ACTION_INCREASE_DELAY)) == 0){
			sig = SIGUSR1;
		}else if(strncmp(h->action, HK_ACTION_REMOVE_MSG, strlen(HK_ACTION_REMOVE_MSG)) == 0){
			sig = SIGUSR2;
		}else if(strncmp(h->action, HK_ACTION_SEND_MSG, strlen(HK_ACTION_SEND_MSG)) == 0){
			sig = SIGCONT;
		}else if(strncmp(h->action, HK_ACTION_SHUT_DOWN, strlen(HK_ACTION_SHUT_DOWN)) == 0){
			sig = SIGTERM;
		}

		strncpy(action, h->action, strlen(h->action) - 1);
		if(sig < 0){
			ErrExit("Impossibile inviare action (signal undefined)");
		}

		kill(targetPid, sig);

		sprintf(log, "Send action %d (%s) to %s (%d)", 
			h->id,
			action,
			process2string(target), 
			targetPid
		);
		printLog("HK", log);
	}
}

int main(int argc, char * argv[]){

	time_t timeIPC;

	if (argc != 2){
		printf("Error invocation of Hackler, you must pass the input file");
		return 1;
	}
	
	// Start process
	printLog("HK", "Process start");
	
	key_t key = generateKey(KEY_INIT_SEM);
	int initSemId = createSemaphore(key);
	char charKey[10];
	sprintf(charKey, "%x", key);
	time(&timeIPC);
	history * SEM = createHistory("SEM", charKey,  "HK", timeIPC, timeIPC);
	
	semOp(initSemId, SEM_START, -1);

	// Wait all process open sem
	semOp(initSemId, SEM_START, 0);
	
	char log[50] = "";
	char * filename = argv[1];

	char * buffer = openHackler(filename);
	char * end_buffer;

	hacklerAction * data[MAX_HACKLER_ACTION];
	int index = 0;

	// Divido la stringa al carattere \n
	char * line = strtok_r(buffer, "\n", &end_buffer);
	int firstLine = 1;
	while(line != NULL){
		if(firstLine != 1){
			sprintf(log, "Analize line %s", line);
			printLog("HK", log);

			hacklerAction * h = line2hacklerAction(line);

			data[index] = h;
			index++;
		}
		firstLine = 0;

		// Divido la stringa al carattere \n, uso NULL in modo che parta dall'ultima posizione in cui aveva diviso 
		line = strtok_r(NULL, "\n", &end_buffer);
	}
	printLog("HK", "Read file");

	readPid(initSemId);

	// Set this process as end init
	semOp(initSemId, SEM_END_INIT, -1);

	// Wait all init end 
	semOp(initSemId, SEM_END_INIT, 0);

	printLog("HK", "End init start");

	// stampo gli header del file di output
	printHacklerActionHeader(HACKLER_FILENAME);

	//Esecuzione delle azioni
	printLog("HK", "Start execution of the action");
	
	// Si assume che le azioni siano già in ordine cronologico
	int time = 0;
	int endAction = 0;
	int i = 0;

	while(endAction == 0){
		int mustSleep = 1;

		hacklerAction * h = data[i];
		if(h->delay <= time){
			// Exec action h
			executeAction(h);

			// Print action
			printHacklerAction(HACKLER_FILENAME, h);

			// Avendo eseguito l'azione, incremento il numero dell'azione da analizzare, non dormo per controllare se anche essa va eseguita in questo stesso istante
			i++;
			mustSleep = 0;

			// Se abbiamo finito le azioni abbiamo finito
			if(i == index){
				endAction = 1;
			}
		}

		if(mustSleep == 1){
			sleep(1);
			time++;
		}
	}

	printLog("HK", "End actions");

	printIPCHistory(initSemId, SEM);

	semOp(initSemId, SEM_HK_IS_RUNNING, -1);
	printLog("HK", "Process end");	

	// Wait for 2 second befor end
	sleep(2);
	return 0;
}