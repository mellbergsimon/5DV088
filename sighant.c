/**
 * Systemnära Programmering
 * Fall 18
 * Assignment 3
 *
 * File:		sighant.c
 * Description:	Signalhanterare
 * Author:		Simon Mellberg
 * CS username:	hed16smg
 * Version:		2018-10-12 v2.0
 * Limitations:	
 */
//Include header
#include "sighant.h"
//Sigaction
#define _POSIX_C_SOURCE 200809L

/*
 * All active processes.
 */
pid_t pids[MAXCOMMANDS];

/**
 * sigHandler() - Kills all active processes.
 * @sig: Recieved signal
 *
 * Kills all started processes when SIGINT appears.
 *
 */
void sigHandler(int sig) {
    if (sig == SIGINT) {
        for (int i = 0; i < MAXCOMMANDS; i++) {
			if (pids[i] == -1) {
				continue;
			}

			if (kill(pids[i], sig) < 0) {
				fprintf(stderr, "kill: %d %s\n",pids[i], strerror(errno));
				fflush(stderr);
			}
			pids[i] = -1;
		}
    }
    return;
}

/**
 * mySignal() - Creates a struct that contains the signal.
 * @signo: Signal.
 * @func: Signal handler function.
 * Ability to choose when the signal will be handled.
 *
 */
Sigfunc *
mySignal(int signo, Sigfunc *func)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}