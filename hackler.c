/// @file client.c
/// @brief Contiene l'implementazione del client.
#include <stdio.h>
#include <unistd.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

int main(int argc, char * argv[]) {
	if (argc != 2){
		printf("Error invocation of Hackler, you must pass the input file");
		return 1;
	}
	// Start process
	printLog("HK", "Process start");

  char *filename = argv[0];
	char *buffer = openHackler(filename);

  hacklerAction *data[10];
  int cursor = 0;
  int i = 0;

  while(*(buffer+cursor) != 0x0){
    data[i] = line2hacklerAction(buffer, &cursor);
    i++;
  }

  for(int j = i; j >= 0; j++){
    printHacklerAction("output/F7_out.csv", data[j]);
  }

	// Wait for 2 second befor end
	sleep(2);
	printLog("HK", "Process end");
	return 0;
}
