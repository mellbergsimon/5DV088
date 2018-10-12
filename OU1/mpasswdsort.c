/**
 * Systemnära Programmering
 * Fall 18
 * Assignment 1
 *
 * File:		mpasswdsort.c
 * Description:	Sorterar användare utifrån deras UID från given fil.
 * Author:		Simon Mellberg
 * CS username:	hed16smg
 * Version:	2018-09-13 v1.0
 * Limitations:	
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpasswdsort.h"

struct user {
	unsigned int id;
	char *name;
};

//Global int
int failed;

/**
 * isEmpty() - Checks if file is empty
 * @*f: opened file to check.
 *
 * Checks an open file if it is empty.
 *
 * Returns: true or false if file is empty or not
 */
bool isEmpty(FILE *f){
	//Check if file is empty
	
	fseek(f, 0, SEEK_END); // goto end of file
		if (ftell(f) == 0){
			return true;
		}
	fseek(f, 0, SEEK_SET);
	return false;
}

/**
 * createUser() - Allocates memory for an user.
 * 
 * Allocates memory for an user and returns the pointer.
 *
 * Returns: Pointer to the user.
 */
user *createUser(void) {
	user *u = malloc(sizeof(user));
	if(u == NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	u->name = NULL;
	
	return u;
}

/**
 * myAtoi() - Convert a string to integer.
 * @*str: String to be converted.
 *
 * Converts a string to an integer and handles negative numbers.
 *
 * Source: https://www.geeksforgeeks.org/write-your-own-atoi/
 *
 * Returns: Converted integer.
 */
int myAtoi(char *str) {
    int res = 0;  // Initialize result
    int sign = 1;  // Initialize sign as positive
    int i = 0;  // Initialize index of first digit
      
    // If number is negative, then update sign
    if (str[0] == '-'){
        sign = -1;  
        i++;  // Also update index of first digit
    }
      
    // Iterate through all digits and update the result
    for (; str[i] != '\0'; ++i){
        res = res*10 + str[i] - '0';
    }
    // Return result with sign
    return sign*res;
}

/**
 * strtok() - Split a string into tokens.
 * @*str: String to split.
 * @*delim: Characters to split string at.
 *
 * Splits a string in several tokens if called consecutively.
 *
 * Returns: Split token.
 */
char *strtok(char *str, const char *delim) {
	static char *start = NULL; /* stores string str for consecutive calls */
	char *token = NULL; /* found token */
	
	/* assign new start in case */
	if (str) {
		start = str;
	}
	/* check whether text to parse left */
	if (start == NULL) {
		return NULL;
	}
	/* remember current start as found token */
	token = start;
	
	/* find next occurrence of delim */
	start = strpbrk(start, delim);
	
	/* replace delim with terminator and move start to follower */
	if (start){
		*start++ = '\0';
	}
	return token;
}

/**
 * isDigits() - Check if a string is numeric
 * @*str: String to check for numbers.
 *
 * Checks wether a string is numeric or not.
 *
 * Returns: true incase all characters are numbers.
 */
bool isDigits(const char * str) {
	//Return false is string is null or blank.
	if(str == NULL || *str == '\0'){
		return false;
	}
	unsigned int count = 0;
	for(unsigned int i = 0; i < strlen(str); i++){
		if((str[i] >= '0' && str[i] <= '9' )|| str[i] == '-'){
			count++;
		}
	}
	if(count == strlen(str)){
		return true;
	} else {
		return false;
	}
}

/**
 * readFile() - Read file and check for errors.
 * @*file: Opened file to read.
 *
 * Reads an opened file and checks for errors and reports these.
 *
 * Returns: List with important info from file.
 */
list *readFile(FILE *file) {
	char Buffer[MAXLINESIZE];
	list *l = list_empty();
	list_pos p = list_first(l);
	char * p_error;
	
	for (int i = 1; (p_error = fgets(Buffer, sizeof(Buffer), file)); i++) {
		if(p_error == NULL){
			perror("fgets");
			exit(EXIT_FAILURE);
		} else {
			if(Buffer[0] == '\r' && Buffer[1] == '\n'){
				fprintf(stderr, "Line %d: Encounterad a <BLANKLINE>\n", i);
				failed++;
			} else {
				char * fullString = strndup(Buffer, MAXLINESIZE);
				//Take out data from buffer.
				char *uname = strtok(Buffer, ":");
				/*char *pword =*/ strtok(NULL, ":");
				char *UID = strtok(NULL, ":");
				char *GID = strtok(NULL, ":");
				/*char *GECOS =*/ strtok(NULL, ":");
				char *directory = strtok(NULL, ":");
				char *shell = strtok(NULL, ":");
				
				if (!uname || !UID || !GID || !directory || !shell){
					fprintf(stderr, "Line %d: Invalid format: %s", i, fullString);
					failed++;
				}	
				else {
					
					//Convert UID and GID to int.
					int id = myAtoi(UID);
					int gi = myAtoi(GID);
					
					//Save failNr before testing
					int saveFailed = failed;
					
					//Test username
					if (uname[0] == '\0'){
						fprintf(stderr,
						"Line %d: The username field cannot be empty.\n", i);
						failed++;
					} else if (strlen(uname) > 32){
						fprintf(stderr,
						"Line %d: The username cannot be longer "
							"than 32 characters. Got: \"%s\"\n", i, uname);
						failed++;
					}

					//Test UID
					if (UID[0] == '\0'){
						fprintf(stderr,
						"Line %d: The 'uid' field cannot be empty.\n", i);
						failed++;
					} else if(!isDigits(UID)){
						fprintf(stderr,
						"Line %d: The 'uid' field has to be numeric. "
							"Got: \"%s\"\n", i, UID);
						failed++;
					} else if( id<0 ){
						fprintf(stderr,
						"Line %d: The 'uid' field has to be a positive number."
						" Got: '%d'\n",i, id);
						failed++;
					}
					
					//Test GID
					if (GID[0] == '\0'){
						fprintf(stderr,
						"Line %d: The 'gid' field cannot be empty.\n", i);
						failed++;
					} else if(!isDigits(GID)){
						fprintf(stderr, 
						"Line %d: The 'gid' field has to be numeric: "
							"Got: \"%s\"\n", i, GID);
						failed++;
					} else if( gi<0 ){
						fprintf(stderr,
						"Line %d: The 'gid' field has to be a positive number."
						" Got: '%d'\n",i, gi);
						failed++;
					}
									
					//Test directory
					if (directory[0] == '\0'){
						fprintf(stderr, 
						"Line %d: The Home field cannot be empty.\n", i);
						failed++;
					}
					
					//Test shell
					if (shell[0] == '\r'){
						fprintf(stderr, 
						"Line %d: The shell field cannot be empty.\n", i);
						failed++;
					}
					
					
					//If no fails then save data to list.
					if(failed == saveFailed) {
						//Create user and save id and name.
						user *a = createUser();
						if(a == NULL){
							return NULL;
						}
						
						a->id = id;
						a->name = malloc(strlen(uname) + 1);
						
						if (a->name == NULL) {
							perror("malloc");
							exit(EXIT_FAILURE);
						}

						strcpy(a->name, uname);
						
						//Saving user in node.
						p = list_insert(a, p);
					}
				}
				free(fullString);
			}
		}
	}
	return l;
}

/**
 * swap() - Swap two users place. 
 * @*a: User a.
 * @*b: User b.
 *
 * Swaps two users place. Sorting algorithm helper.
 *
 */
void swap(user *a, user *b) {
	
	user *t = createUser();
	if(t == NULL){
		exit(EXIT_FAILURE);
	}
	
	*t  = *b;
	*b = *a;
	*a = *t;
	free(t);
}

/**
 * selectionSort() - Sort a list using selectionSort.
 * @*l: list to sort.
 *
 * Sorts a list using the algorithm selectionSort.
 *
 */
void selectionSort(list *l) {
	
	list_pos first_pos = list_first(l);
	list_pos second_pos;
	
	//Incase list is only 1 node.
	if (!(list_end(first_pos))){
		second_pos = list_next(first_pos);
	} else {
		//This is probably not necessary.
		second_pos = first_pos;
	}
	
	while(!(list_end(first_pos))){
		
		user *a;
		list_pos min = first_pos;
		
		while(!(list_end(second_pos))){
			
			a = (user *)list_inspect(min);
			user *b = (user *)list_inspect(second_pos);
			
			if (b->id < a->id){
				min = second_pos;
			}
			
			second_pos = list_next(second_pos);
		}
		
		//Swap the lowest saved ID.
		if(min != first_pos){
			swap((user *)list_inspect(first_pos),
			(user *)list_inspect(min));
		}
		
		first_pos = list_next(first_pos);
		if (!(list_end(first_pos))){
			second_pos = list_next(first_pos);
		}
	}
}

/**
 * kill() - Frees memory stored in list.
 * @*l: list to kill.
 *
 * Frees allocated memory list uses.
 *
 */
void kill(list *l) {
	list_pos p = list_first(l);
	
	while(!(list_end(p))){
		//Free values.
		user *u = (user *)list_inspect(p);
		free(u->name);
		free(u);
		
		p = list_next(p);
	}
	list_kill(l);
}

int main(int argc, char **argv) {
	//Default is stdin.
	FILE* file = stdin;
			
	//In other cases the file comes from user input.
	if (argc == 2){
		
		file = fopen(argv[1], "r");
		if(!file){
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
		
	} 
	
	//Check if file is empty
	if (isEmpty(file)){
		fprintf(stderr, "File is empty\n");
		fclose(file);
		return EXIT_SUCCESS;
	}
	
	//Read file
	list *l = readFile(file);
	
	//list could not be created.
	if(l == NULL){
		fclose(file);
		exit(EXIT_FAILURE);
	}
	
	//Close the file
	fclose(file);
	
	//Sort list
	(selectionSort(l));
	
	//Traverse the list and print it.
	list_pos p = list_first(l);
	while(!(list_end(p))){
		
		user *u = (user *)list_inspect(p);
		printf("%d:%s\n", u->id, u->name);
		
		p = list_next(p);
	}
	
	if(failed > 0 ){
        kill(l);
        exit(EXIT_FAILURE);
    }
	
	//Free the list and data.
	kill(l);
	return EXIT_SUCCESS;
}