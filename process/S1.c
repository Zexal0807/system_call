/// @file R1.c
/// @brief Contiene l'implementazione del receiver 1.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    printLog("S1", "Process start with exec");
    
    // Wait for 1 second befor end
    sleep(1);
    printLog("S1", "Process End");
    return 1;
}
