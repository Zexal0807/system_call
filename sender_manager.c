/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void functionS1(){
    printLog("S1", "Process start");
    
    printLog("S1", "Process End");
    exit(0);
}

void functionS2(){
    printLog("S2", "Process start");
    
    printLog("S2", "Process End");
    exit(0);
}

void functionS3(){
    printLog("S3", "Process start");
    
    printLog("S3", "Process End");
    exit(0);
}

int main(int argc, char * argv[]) {

    // Check command line input arguments
    if (argc != 2){
        printf("Error invocation of Sender Manager, you must pass the input file");
        return 1;
    }

    printLog("SM", "Process start");

    // Define the 3 struct process
    process *S1 = NULL;
    process *S2 = NULL;
    process *S3 = NULL;

    // Try to create a child, in each child functione must me an exit
    int pid = fork();
    if(pid == -1){  
        printf("Error invocation of Sender Manager");
        return 1;
    }else if(pid == 0){
        functionS1();
    }
    S1 = createProcess('S', 1, pid);

    // Try to create a child, in each child functione must me an exit
    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        functionS2();
    }
    S2 = createProcess('S', 2, pid);

    // Try to create a child, in each child functione must me an exit
    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        functionS3();
    }
    S3 = createProcess('S', 3, pid);

    //Save process pid in file
    printProcessList("output/F8.csv", S1, S2, S3);

    // Wait the end of all child
    pid_t child;
    int status;
    while ((child = wait(&status)) != -1){
        printf("returned child %d with status %d\n", child, status);
    }
    
    printLog("SM", "Process End");
    return 0;
}