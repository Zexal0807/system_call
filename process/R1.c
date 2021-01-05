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
    printLog("R1", "Process start with exec");
    
    time_t a;
    time_t d;
    time(&a);
    time(&d);

    trafficInfo *t = createTrafficInfo(
      createMessage(
        1, 
        "Ciao come va?",
        createProcess('S', 1, 1),
        createProcess('R', 1, 1),
        1,
        1,
        1,
        "H"
      ),
      a,
      d
    );

    printTrafficInfo("output/F1.csv",t);

    // Wait for 1 second befor end
    sleep(1);
    printLog("R1", "Process End");
    return 1;
}
