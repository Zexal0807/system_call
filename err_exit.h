/// @file err_exit.h
/// @brief Contiene la definizione della funzione di stampa degli errori.

#pragma once

/// @brief Prints the error message of the last failed
///         system call and terminates the calling process.
void ErrExit(const char *msg);
//specifica per gli errori di open
void ErrOpen(int file);