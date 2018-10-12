/**
 * Systemnära Programmering
 * Fall 18
 * Assignment 1
 *
 * File:		list.c
 * Description:	Listimplimentation
 * Author:		Simon Mellberg
 * CS username:	hed16smg
 * Version:	2018-09-13 v1.0
 * Limitations:	
 */
#include <stdio.h>
#include <stdlib.h> 

#include "list.h"

struct node {
	struct node *next;
	void *value;
};

struct list {
	struct node *head;
};

/**
 * list_empty() - Create an empty list.
 *
 * Returns: A pointer to the new list.
 */
list *list_empty(void) {
	list *l = malloc(sizeof(list));
	if (l == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

	l->head = malloc(sizeof(struct node));
	if (l->head == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
	
	l->head->next = NULL;
	
	return l;
}

/**
 * list_isempty() - Check if a list is empty.
 *
 * Returns: True if the list is empty, otherwise false.
 */
bool list_isempty(const list *l){
	return (l->head->next == NULL);
}

/**
 * list_first() - Return the first position of a list.
 * @l: List to inspect.
 *
 * Returns: The first position in the list.
 */
list_pos list_first(const list *l){
	return (l->head);
}

/**
 * list_next() - Return the next position in a list.
 * @p: Position to return the next from.
 *
 * Returns: The position in the list after the given position.
 */
list_pos list_next(const list_pos p){
	if (list_end(p)) {
		// This should really throw an error.
		fprintf(stderr,"list_next: Warning: Trying to navigate "
			"past end of list!\n");
	}
	return p->next;
}

/**
 * list_end() - Check if a given position is at the end of a list.
 * @p: Any valid position in the list.
 *
 * Returns: True if p is at the end of the list.
 */
bool list_end(const list_pos p){
	return p->next == NULL;
}

/**
 * list_insert() - Insert a new element with a given value into a list.
 * @v: Value to be inserted into the list.
 * @p: Position in the list before which the value should be inserted.
 *
 * Creates a new element and inserts it into the list before p.
 * Stores data in the new element.
 *
 * Returns: The position of the newly created element.
 */
list_pos list_insert(void *v, const list_pos p)
{
	// Create new element.
	list_pos new_node = malloc(sizeof(struct node));
	
	if (new_node == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
	
	// Set value.
	new_node->value = v;

	// Set pointers
	new_node->next = p->next;
	p->next = new_node;
	
	return p;
}

/**
 * list_inspect() - Return the value of the element at a given
 *		     position in a list.
 * @p: Any valid position in the list, except the last.
 *
 * Returns: Returns the value at the given position.
 */
void *list_inspect(const list_pos p){
	if (list_end(p)) {
		// This should really throw an error.
		fprintf(stderr,"list_inspect: Warning: Trying to inspect "
			"position at end of list!\n");
	}
	return p->next->value;
}

/**
 * list_kill() - Kill the list.
 * @l: List to kill.
 *
 * Return all allocated memory used by the list.
 *
 */
void list_kill(list *l)
{
	// Use public functions to traverse the list.

	// Start with the first element (will be defined even for an
	// empty list).
	list_pos p = list_first(l);

	// Remove first element until list is empty.
	while(!list_isempty(l)) {
		// Cell to remove.
		list_pos temp=p->next;

		// Point past cell to remove.
		p->next=temp->next;

		// Free the memory allocated to the cell itself.
		free(temp);
	}

	// Free the head and the list itself.
	free(l->head);
	free(l);    
}