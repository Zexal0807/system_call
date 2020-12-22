/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"

const process SENDER_1 = {'S', 1};
const process SENDER_2 = {'S', 2};
const process SENDER_3 = {'S', 3};

const process RECIVER_1 = {'R', 1};
const process RECIVER_2 = {'R', 2};
const process RECIVER_2 = {'R', 3};

const process ALL_PROCESS = {'Z', 0};

const int MESSAGE_QUEUE = 1;
const int SHARED_MEMORY = 2;
const int FIFO = 3;