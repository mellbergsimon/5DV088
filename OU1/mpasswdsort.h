/**
 * Systemnära Programmering
 * Fall 18
 * Assignment 1
 *
 * File:		mpasswdsort.h
 * Description:	Headerfil till mpasswdsort.c
 * Author:		Simon Mellberg
 * CS username:	hed16smg
 * Version:	2018-09-13 v1.0
 * Limitations:	
 */
#include <stdbool.h>

#include "list.h"

#define MAXLINESIZE 1023

typedef struct user user;

/**
 * isEmpty() - Checks if file is empty
 * @*f: opened file to check.
 *
 * Checks an open file if it is empty.
 *
 * Returns: true or false if file is empty or not
 */
bool isEmpty(FILE *f);

/**
 * createUser() - Allocates memory for an user.
 * 
 * Allocates memory for an user and returns the pointer.
 *
 * Returns: Pointer to the user.
 */
user *createUser(void);

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
int myAtoi(char *str);

/**
 * strtok() - Split a string into tokens.
 * @*str: String to split.
 * @*delim: Characters to split string at.
 *
 * Splits a string in several tokens if called consecutively.
 *
 * Returns: Split token.
 */
char *strtok(char *str, const char *delim);

/**
 * isDigits() - Check if a string is numeric
 * @*str: String to check for numbers.
 *
 * Checks wether a string is numeric or not.
 *
 * Returns: true incase all characters are numbers.
 */
bool isDigits(const char * str);

/**
 * readFile() - Read file and check for errors.
 * @*file: Opened file to read.
 *
 * Reads an opened file and checks for errors and reports these.
 *
 * Returns: List with important info from file.
 */
list *readFile(FILE *file);

/**
 * swap() - Swap two users place. 
 * @*a: User a.
 * @*b: User b.
 *
 * Swaps two users place. Sorting algorithm helper.
 *
 */
void swap(user *a, user *b);

/**
 * selectionSort() - Sort a list using selectionSort.
 * @*l: list to sort.
 *
 * Sorts a list using the algorithm selectionSort.
 *
 */
void selectionSort(list *l);

/**
 * kill() - Frees memory stored in list.
 * @*l: list to kill.
 *
 * Frees allocated memory list uses.
 *
 */
void kill(list *l);