/// @file err_exit.h
/// @brief Contiene la definizione della funzione di stampa degli errori.

#pragma once

void ErrExit(const char *msg);
/*
    Specifica per gli errori di open
*/
void ErrOpen(int file);