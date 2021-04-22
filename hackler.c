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

int pidS1 = 0;
int pidS2 = 0;
int pidS3 = 0;
int pidR1 = 0;
int pidR2 = 0;
int pidR3 = 0;

void string2pid(char *line){
    char *saveptr1;
    char *process = strtok_r(line, ";",&saveptr1);
    char *pid = strtok_r(NULL, ";",&saveptr1);

    if(strcmp(process,"S1") == 0){
        pidS1=atoi(pid);
    }else if(strcmp(process,"S2") == 0){
        pidS2=atoi(pid);
    }else if(strcmp(process,"S3") == 0){
        pidS3=atoi(pid);
    }else if(strcmp(process,"R1") == 0){
        pidR1=atoi(pid);
    }else if(strcmp(process,"R2") == 0){
        pidR2=atoi(pid);
    }else if(strcmp(process,"R3") == 0){
        pidR3=atoi(pid);
    }else{
        ErrExit("Error in File");
    }
};

void readFrom(char * filename){
    int file = openFile(filename);
    ErrOpen(file);
    
    int dim=lseek(file, 0L, SEEK_END);
    lseek(file, 0L, SEEK_SET);
    
    char *buffer = (char*) malloc((dim)*sizeof(char));

    read(file, buffer, dim);

    char *saveptr;
    char *token=strtok_r(buffer,"\n",&saveptr);
    //salto la prima riga
    token = strtok_r(NULL,"\n", &saveptr);
    //analizzo il resto del testo
    while(token != NULL){
        string2pid(token);
        token=strtok_r(NULL,"\n", &saveptr);
    }
}

void readPid(int initSemId){
    // Wait sender init end 
    semOp(initSemId, SEM_INIT_SENDER, 0);

    // Read PID form sender file
    readFrom(SENDER_FILENAME);
    
    // Wait receiver init end 
    semOp(initSemId, SEM_INIT_RECEIVER, 0);

    // Read PID form receiver file
    readFrom(RECEIVER_FILENAME);
	
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

        if(targetPid <= 0){
            ErrExit("Impossibile inviare action");
        }

        int sig = -1;

        if(strcmp(h->action, "IncreaseDelay") == 0){
            sig = SIGUSR1;
        }else if(strcmp(h->action, "RemoveMSG") == 0){
            sig = SIGUSR2;
        }else if(strcmp(h->action, "SendMSG") == 0){
            sig = SIGCONT;
        }else if(strcmp(h->action, "ShutDown") == 0){
            sig = SIGTERM;
        }

        if(sig < 0){
            ErrExit("Impossibile inviare action");
        }

        kill(targetPid, sig);

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

            // TODO : attenzione se il file finisce con \n legge un ! che fa errore

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

    printf("PID: %d, %d, %d, %d, %d, %d\n", pidS1, pidS2, pidS3, pidR1, pidR2, pidR3);

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