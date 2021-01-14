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

int main(int argc, char *argv[]) {
	if (argc != 2){
		printf("Error invocation of Hackler, you must pass the input file");
		return 1;
	}
  char log[100];
	// Start process
	printLog("HK", "Process start");

    char *filename = argv[1];

    hacklerAction *data[MAX_HACKLER_ACTION];
    int index = 0;

	  char *buffer = openHackler(filename);
    char *end_buffer;
    
    // Divido la stringa al carattere \n
	  char *line = strtok_r(buffer, "\n", &end_buffer);
    int firstLine = 1;
    while(line != NULL){
      if(firstLine != 1){
        sprintf(log, "Analize line '%s'", line);
        printLog("HK", log);

        hacklerAction *h = line2hacklerAction(line);

        data[index] = h;
        index++;
      }
      firstLine = 0;

      // Divido la stringa al carattere \n, uso NULL in modo che parta dall'ultima posizione in cui aveva diviso 
      line = strtok_r(NULL, "\n", &end_buffer);
    }
  printLog("HK", "End file");
  
  //Esecuzione delle azioni
  printLog("HK", "Start execution of the action");
  for(int j = index - 1; j >= 0; j--){
    hacklerAction *h = data[j];
    printHacklerAction(HACKLER_FILENAME, data[j]);
  }
  printLog("HK", "End action");

	// Wait for 2 second befor end
	sleep(2);
	printLog("HK", "Process end");
	return 0;
}