/// @file R1.c
/// @brief Contiene l'implementazione del receiver 1.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"
//#include "../struct/list.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
    printLog("S1", "Process start with exec");
    
    char *filename = argv[0];
    node *l = createList(filename);

    char log[50];
    sprintf(log, "Loaded message from file %s", filename);
    printLog("S1", log);
    
    while(l != NULL){
      
      sprintf(log, "Elaborated message: %d", l->message->id);
      printLog("S1", log);
      
      l = getNext(l);
    }
    
    // Wait for 1 second befor end
    sleep(1);
    printLog("S1", "Process End");
    return 1;
}
