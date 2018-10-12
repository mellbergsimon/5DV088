/**
 * Systemnära Programmering
 * Fall 18
 * Assignment 3
 *
 * File:		mish.c
 * Description:	Minimalt shell (mish).
 * Author:		Simon Mellberg
 * CS username:	hed16smg
 * Version:		2018-10-12 v2.0
 * Limitations:	Error checking in internal_echo.
 *				Returns 0 for both success and failure in command_cd.
 *				Cannot execute both internal and external
 *				commands at the same time.
 */

#include "mish.h"

/*
 * All active processes.
 */
pid_t pids[MAXCOMMANDS];

/**
 * Main
 */
int main(void){
	
	mish_Start();

	return EXIT_SUCCESS;
}

/**
 * prompt() - Prompts shell name.
 *
 * Prints shell name to stderr.
 *
 */
void prompt(void){
	
	fprintf(stderr,"mish %% ");
	if(fflush(stderr)<0){
		perror("fflush\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * mish_Start() - Starts the shell program.
 *
 * Initiates required variables and starts program loop.
 * 1. Get line from user.
 * 2. Parse line into commands.
 * 3. Check wether command is internal or external.
 */
void mish_Start(void){
	
	size_t bufferSize = MAXLINELEN;
	char *buffer = malloc(bufferSize);
	command cmd[MAXCOMMANDS];
	int nrOfCommands;
	int status = 0;

	while (status >= 0) {
		//Ignore SIGINT while running
		mySignal(SIGINT, SIG_IGN);
		
		//Print prompt
		prompt();

		//Get line from user
		if (getline(&buffer, &bufferSize, stdin) == EOF) {
			fprintf(stderr, "\n");
			fflush(stderr);
			break;
		}
		
		//Parse input
		if ((nrOfCommands = parse(buffer, cmd)) == 0) {
			continue;
		}

		//Test if internal command otherwise external
		if ((status = internal_command(cmd[0].argv)) == 0 ){
			continue;
		} else {
			execute_external(nrOfCommands, cmd);
		}
	}
	
	//Free buffer and exit
	free(buffer);
}

/**
 * duplicatePipe() - Error handling function for dupPipe.
 * @pipes: Pipe to duplicate.
 * @flags: Which end to duplicate.
 * @dest: Standard file descriptor.
 *
 * Runs dupPipe and checks for error.
 *
 */
void duplicatePipe(int pipes[2], int flags, int dest){
	if(dupPipe(pipes, flags, dest) < 0){
		fprintf(stderr, "%d: %s\n", dest, strerror(errno));
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
}

/**
 * redirectPipe() - Error handling function for redirect.
 * @*filename: File the file descriptor should be redirected.
 * @flags: Which end to open.
 * @dest: Standard file descriptor.
 *
 * Runs redirect and checks for error.
 *
 */
void redirectPipe(char *filename, int flags, int dest){
	if(redirect(filename, flags, dest) < 0){
		fprintf(stderr, "%s: %s\n", filename, strerror(errno));
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
}

/**
 * execute_external() - Executes external commands on children.
 * @nrOfCommands: Number of commands to execute.
 * @*cmd: Command to execute if possible.
 *
 * Forks, couples togheter stdin and stdout.
 * Executes command on child and waits for children to complete.
 *
 */
void execute_external(int nrOfCommands, command *cmd) {
	//Receive sigint and run sigHandler
	mySignal(SIGINT, sigHandler);
	//Pipes between children
    int pipes[2][2];

	for (int i = 0; i < nrOfCommands; i++) {
		bool is_first = (i == 0);
		bool is_last = (i == nrOfCommands - 1);

		//File redirection is only allowed at first and last command.
		if ((!is_first && cmd[i].infile) || (!is_last && cmd[i].outfile)) {
			fprintf(stderr, "Mish: Syntax error\n");
			fflush(stderr);
			return;
		}
		
		//Dont pipe last command
		if (!is_last && pipe(pipes[i%2]) < 0) {
			fprintf(stderr, "Pipe fail: %s", strerror(errno));
			fflush(stderr);
		}

		//Fork
		if ((pids[i] = fork()) < 0) {
			fprintf(stderr, "Fork fail: %s", strerror(errno));
			fflush(stderr);
		} else if (pids[i] == 0) {
			//Child
			if (nrOfCommands > 1) {
				//Previous command. Duplicate write end
				if (!is_last) {
					duplicatePipe(pipes[i%2], WRITE_END, STDOUT_FILENO);
				}
				//Next command. Duplicate read end
				if (!is_first) {
					duplicatePipe(pipes[(i+1)%2], READ_END, STDIN_FILENO);
				}
			}

			//Redirect infile when first command and infile exists
			if (is_first && cmd[i].infile) {
				redirectPipe(cmd[i].infile, READ, STDIN_FILENO);
			}
			//Redirect outfile when last command and outfile exists
			if (is_last && cmd[i].outfile) {
				redirectPipe(cmd[i].outfile, WRITE, STDOUT_FILENO);
			}
	
			//Execute command on child
			if (execvp(cmd[i].argv[0], cmd[i].argv) < 0) {
				fprintf(stderr, "%s: %s\n", cmd[i].argv[0], strerror(errno));
				fflush(stderr);
				exit(EXIT_FAILURE);
			}
		} else {
			//Parent
			if (is_first) {
				continue;
			}
			//Close both read and write end
			if (close(pipes[(i+1)%2][WRITE_END]) < 0 ||
				close(pipes[(i+1)%2][READ_END]) < 0) {
				fprintf(stderr, "Close pipe fail: %s", strerror(errno));
				fflush(stderr);
			}
		}
	}
	//Wait for children to complete
	for (int i = 0; i < nrOfCommands; i++) {
		int status;

		do {
			waitpid(pids[i], &status, 0);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		pids[i] = -1;
	}
}

/**
 * internal_command() - Check wether command is internal command or not.
 * @**argv: Command from input.
 *
 * Executes internal command if command exists as internal.
 *
 * Returns: 0 if all gone well or -1 if exit.
 */
int internal_command(char **argv){
	if (strcmp(argv[0], "cd") == 0){
		return command_cd(argv);
	} else if (strcmp(argv[0], "echo") == 0){
		return command_echo(argv);
	} else if (strcmp(argv[0], "exit") == 0){
		return -1;
	} 
	//No internal command
	return 1;
}

/**
 * command_cd() - Internal command CD(Change directory).
 * @**argv: Command from input.
 *
 * Changes directory using chdir.
 * 
 * Returns: 0.
 *
 */
int command_cd(char **argv){
	char *dir = argv[1] == NULL ? getenv("HOME") : argv[1];

	if (chdir(dir) != 0) {
		fprintf(stderr, "%s: No such file or directory\n", *argv);
		fflush(stderr);
		return 0;
	}

	return 0;
}

/**
 * command_echo() - Internal command echo).
 * @**argv: Command from input.
 *
 * Echos command to stdout using printf.
 * 
 * Returns: 0 when completed.
 *
 */
int command_echo(char **argv) {
	for (int i = 1; argv[i] != NULL; i++) {
		char *fmt = argv[i+1] == NULL ? "%s" : "%s ";
		printf(fmt, argv[i]);
	}
	fputs("\n", stdout);
	fflush(stdout);

	return 0;
}