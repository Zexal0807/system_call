/// @file hackler.c
/// @brief Contiene l'implementazione del hackler.
#include <stdio.h>
#include <unistd.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

void readFrom(char * filename){
    openFile(filename);
}

int pidS1 = -1;
int pidS2 = -2;
int pidS3 = -3;
int pidR1 = -4;
int pidR2 = -5;
int pidR3 = -6;

void readPid(int initSemId){
    // Wait sender init end 
    semOp(initSemId, SEM_INIT_SENDER, 0);

    // Read PID form sender file
    
    // Wait receiver init end 
    semOp(initSemId, SEM_INIT_RECEIVER, 0);

    // Read PID form receiver file
	
}

void executeAction(hacklerAction *h){
	char log[50];

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

        sprintf(log, "Send action %d (%s) to ALL", 
            h->id,
            h->action
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
/*
        if(targetPid <= 0){
            ErrExit("Impossibile inviare haction");
        }
*/
        char * sig = "CIAO";

        //kill(targetPid, sig);

        sprintf(log, "Send action %d (%s) to %s (%d)", 
            h->id,
            h->action,
            process2string(target), 
            targetPid
        );
        printLog("HK", log);
    }
}

int main(int argc, char *argv[]) {
	if (argc != 2){
		printf("Error invocation of Hackler, you must pass the input file");
		return 1;
	}
	
	// Start process
	printLog("HK", "Process start");

    int initSemId = createSemaphore();
    semOp(initSemId, SEM_START, -1);

    // Wait all process open sem
    semOp(initSemId, SEM_START, 0);
    
	char *filename = argv[1];

	hacklerAction *data[MAX_HACKLER_ACTION];
	int index = 0;

    // TODO : use dynamic read as S1

	char *buffer = openHackler(filename);
	char *end_buffer;
	char log[50];
	// Divido la stringa al carattere \n
	char *line = strtok_r(buffer, "\n", &end_buffer);
	int firstLine = 1;
	while(line != NULL){
		if(firstLine != 1){
			sprintf(log, "Analize line '%s'", line);
			printLog("HK", log);

			hacklerAction *h = line2hacklerAction(line);

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

	//Esecuzione delle azioni
	printLog("HK", "Start execution of the action");
    
	for(int i = 0; i < index; i++){
		hacklerAction *h = data[i];

        //Wait delay
        sleep(h->delay);

        // Exec action h
        executeAction(h);

        // Print action
		printHacklerAction(HACKLER_FILENAME, h);
	}
	printLog("HK", "End action");

	// Wait for 2 second befor end
	sleep(2);
	printLog("HK", "Process end");
	return 0;
}