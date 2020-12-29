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
#include <unistd.h>

void functionS1(){
    printf("Start S1");

    printf("End S1");
    exit(0);
}

void functionS2(){
    printf("Start S2");

    printf("End S2");
    exit(0);
}

void functionS3(){
    printf("Start S3");

    printf("End S3");
    exit(0);
}

int main(int argc, char * argv[]) {

    // Check command line input arguments
    if (argc != 2){
        printf("Error invocation of Sender Manager, you must pass the input file");
        return 1;
    }

    // Define the 3 struct process
    process *S1 = NULL;
    process *S2 = NULL;
    process *S3 = NULL;

    // Try to create a child, i
    int pid = fork();
    if(pid == -1){  
        printf("Error invocation of Sender Manager");
        return 1;
    }else if(pid == 0){
        functionS1();
    }
    S1 = createProcess('S', 1, pid);
    
    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        functionS2();
    }
    S2 = createProcess('S', 2, pid);

    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        functionS3();
    }
    S3 = createProcess('S', 3, pid);

    printProcessList("F8.csv", S1, S2, S3);

    return 0;
}