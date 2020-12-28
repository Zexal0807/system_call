/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

int main(int argc, char * argv[]) {

    // Check command line input arguments
    if (argc != 0){
        printf("Error invocation of Receiver Manager");
        return 1;
    }

    // Define the 3 struct process
    process *R1 = null;
    process *R2 = null;
    process *R3 = null;

    // Try to create a child, i
    int pid = fork();
    if(pid == -1){  
        printf("Error invocation of Receiver Manager");
        return 1;
    }else if(pid == 0){
        R1();
        exit(0);
    }
    R1 = createProcess('R', 1, pid);
    
    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        R2();
        exit(0);
    }
    R2 = createProcess('R', 2, pid);

    pid = fork();
    if(pid == -1){  return 0;
    }else if(pid == 0){
        R3();
        exit(0);
    }
    R3 = createProcess('R', 3, pid);

    return 0;
}

void R1(){
    printf("Call Reciver 1");
}

void R2(){
    printf("Call Reciver 2");
}

void R3(){
    printf("Call Reciver 3");
}