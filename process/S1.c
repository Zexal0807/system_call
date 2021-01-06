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
    
    process *s1 = createProcess('S', 1, 0);
    process *s2 = createProcess('S', 2, 0);
    process *s3 = createProcess('S', 3, 0);

    process *r1 = createProcess('R', 1, 0);
    process *r2 = createProcess('R', 2, 0);
    process *r3 = createProcess('R', 3, 0);

    node *l = createList(
      "input/F0.csv", s1, s2, s3, r1, r2, r3
    );
    
    while(hasNext(l)){
      printf("%s\n", l->message->content);
      l = getNext(l);
    }

    // Wait for 1 second befor end
    sleep(1);
    printLog("S1", "Process End");
    return 1;
}
