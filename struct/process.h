#pragma once

/*
	Struttura di un processo
	 - type : Tipo del processo [S: Sender, R: Reciver]
	 - number : Numero del processo
*/
typedef struct {
	char type;
	int number;
} process;

process *createProcess(char type, int number);

process *SENDER_1();
process *SENDER_2();
process *SENDER_3();
process *RECEIVER_1();
process *RECEIVER_2();
process *RECEIVER_3();

process *string2process(char *str);

char *process2string(process *p);
