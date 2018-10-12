#ifndef SIGHANT_H
#define SIGHANT_H

/*
 * Included libraries.
 */
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "parser.h"

typedef	void Sigfunc(int);

/**
 * sigHandler() - Kills all active processes.
 * @sig: Recieved signal
 *
 * Kills all started processes when SIGINT appears.
 *
 */
void sigHandler(int sig);

/**
 * mySignal() - Creates a struct that contains the signal.
 * @signo: Signal.
 * @func: Signal handler function.
 * Ability to choose when the signal will be handled.
 *
 */
Sigfunc * mySignal(int signo, Sigfunc *func);

#endif