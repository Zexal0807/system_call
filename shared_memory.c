/// @file shared_memory.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "struct/message.h"

#include <sys/shm.h>
#include <sys/stat.h>

int createSharedMemory(){
    int shmid = shmget(KEY_SHARED_MEMORY, sizeof(message), IPC_CREAT | S_IRUSR | S_IWUSR);
    if(shmid == -1){
        ErrExit("Impossible create shared memory");
    }
    return shmid;
}

void * attachSharedMemory(int shmid, int shmflg) {
    void *ptr_sh = shmat(shmid, NULL, shmflg);
    if (ptr_sh == (void *)-1)
        ErrExit("shmat failed");
    return ptr_sh;
}

void detachSharedMemory(void *ptr_sh) {
    if(shmdt(ptr_sh) == -1){
        ErrExit("shmdt failed");
    }
}

void removeSharedMemory(int shmid) {
    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        ErrExit("shmctl failed");
    }
}

void SHtoR1(message *ptr_sh, message *m, int semid){
    //Avverto R1 della presenza del messaggio
    semOp(semid, SEM_R1_SH, 1);
    //salvo il messaggio nel segmento
    ptr_sh=m;
}

void SHtoR2(void *ptr_sh, message *m, int semid){
    //Avverto R2 della presenza del messaggio
    semOp(semid, SEM_R2_SH, 1);
    //salvo il messaggio nel segmento
    ptr_sh=m;
}

void SHtoR3(void *ptr_sh, message *m, int semid){
    //Avverto R3 della presenza del messaggio
    semOp(semid, SEM_R3_SH, 1);
    //salvo il messaggio nel segmento
    ptr_sh=m;
}