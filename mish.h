#ifndef MISH_H
#define MISH_H

//getline()
#define _GNU_SOURCE

/*
 * Included libraries.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Includes
 */
#include "parser.h"
#include "execute.h"
#include "sighant.h"

#define WRITE O_WRONLY | O_CREAT | O_EXCL
#define READ O_RDONLY

/*
 * All active processes.
 */
extern pid_t pids[MAXCOMMANDS];


int main(void);

/**
 * prompt() - Prompts shell name.
 *
 * Prints shell name to stderr.
 *
 */
void prompt(void);

/**
 * mish_Start() - Starts the shell program.
 *
 * Initiates required variables and starts program loop.
 * 1. Get line from user.
 * 2. Parse line into commands.
 * 3. Check wether command is internal or external.
 */
void mish_Start(void);

/**
 * duplicate_Pipe() - Error handling function for dupPipe.
 * @pipes: Pipe to duplicate.
 * @flags: Which end to duplicate.
 * @dest: Standard file descriptor.
 *
 * Runs dupPipe and checks for error.
 *
 */
void duplicatePipe(int pipes[2], int flags, int dest);

/**
 * redirectPipe() - Error handling function for redirect.
 * @*filename: File the file descriptor should be redirected.
 * @flags: Which end to open.
 * @dest: Standard file descriptor.
 *
 * Runs redirect and checks for error.
 *
 */
void redirectPipe(char *filename, int flags, int dest);

/**
 * execute_external() - Executes external commands on children.
 * @nfOrCommands: Number of commands to execute.
 * @*cmd: Command to execute if possible.
 *
 * Forks, couples togheter stdin and stdout.
 * Executes command on child and waits for children to complete.
 *
 */
void execute_external(int nfOrCommands, command *cmd);

/**
 * internal_command() - Check wether command is internal command or not.
 * @**argv: Command from input.
 *
 * Executes internal command if command exists as internal.
 *
 * Returns: 0 if all gone well or -1 if fail.
 */
int internal_command(char **argv);

/**
 * command_cd() - Internal command CD(Change directory).
 * @**argv: Command from input.
 *
 * Changes directory using chdir.
 * 
 * Returns: 0 if completed ok and -1 for error.
 *
 */
int command_cd(char **argv);

/**
 * command_echo() - Internal command echo).
 * @**argv: Command from input.
 *
 * Echos command to stdout using printf.
 * 
 * Returns: 0 when completed.
 *
 */
int command_echo(char **argv);

#endif