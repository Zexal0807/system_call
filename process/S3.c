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
    printLog("S3", "Process start with exec");
    
    // Wait for 3 second befor end
    sleep(3);
    printLog("S3", "Process End");
    return 1;
}
