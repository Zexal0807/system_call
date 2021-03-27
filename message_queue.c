
#include "err_exit.h"
#include "message_queue.h"
#include "defines.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

int getMessageQueue(){
    int id = msgget(KEY_MESSAGE_QUEUE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(id == -1)
        ErrExit("Impossible create message queue");

    return id;
}

void deleteMessageQueue(int id){
    if (msgctl(id, IPC_RMID, NULL) == -1)
        ErrExit("msgctl failed");
}