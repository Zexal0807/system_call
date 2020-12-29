/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void functionR1(){
    printLog("R1", "Process start");
    
    printLog("R1", "Process End");
    exit(0);
}

void functionR2(){
    printLog("R2", "Process start");
    
    printLog("R2", "Process End");
    exit(0);
}

void functionR3(){
    printLog("R3", "Process start");
    
    printLog("R3", "Process End");
    exit(0);
}

int main(int argc, char * argv[]) {

    // Check command line input arguments
    if (argc != 1){
        printf("Error invocation of Receiver Manager");
        return 1;
    }

    printLog("RM", "Process start");

    // Define the 3 struct process
    process *R1 = NULL;
    process *R2 = NULL;
    process *R3 = NULL;

    // Try to create a child, in each child functione must me an exit
    int pid = fork();
    if(pid == -1){  
        printf("Error invocation of Receiver Manager");
        return 1;
    }else if(pid == 0){
        functionR1();
    }
    R1 = createProcess('R', 1, pid);

    // Try to create a child, in each child functione must me an exit
    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        functionR2();
    }
    R2 = createProcess('R', 2, pid);

    // Try to create a child, in each child functione must me an exit
    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        functionR3();
    }
    R3 = createProcess('R', 3, pid);

    //Save process pid in file
    printProcessList("output/F9.csv", R1, R2, R3);

    // Wait the end of all child
    while(wait() != -1);

    printLog("RM", "Process End");
    return 0;
}
