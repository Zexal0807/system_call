/// @file R1.c
/// @brief Contiene l'implementazione del receiver 1.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>

int main(int argc, char * argv[]) {
    printLog("R1", "Process start with exec");
    
    printf("%s %s", argv[0],argv[1]);
    printLog("R1", "Process End");
    
}
