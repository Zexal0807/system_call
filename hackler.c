/// @file client.c
/// @brief Contiene l'implementazione del client.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {
  if (argc != 2){
      printf("Error invocation of Hackler, you must pass the input file");
      return 1;
  }

  printLog("HK", "Process start");

  //Aspetta 2 secondi
  sleep(2);

  printLog("HK", "Process end");
}
