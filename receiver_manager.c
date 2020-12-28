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
    R1 = createProcess('R', 1, pid);
    R2 = createProcess('R', 2, pid);
    R3 = createProcess('R', 3, pid);
    return 0;
}
