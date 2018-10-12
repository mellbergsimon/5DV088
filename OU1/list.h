#ifndef __LIST_H
#define __LIST_H

#include <stdbool.h>

// List type.
typedef struct list list;

// List position type.
typedef struct node *list_pos;

/**
 * list_empty() - Create an empty list.
 *
 * Returns: A pointer to the new list.
 */
list *list_empty(void);

/**
 * list_isempty() - Check if a list is empty.
 * @l: List to check.
 *
 * Returns: True if the list is empty, otherwise false.
 */
bool list_isempty(const list *l);

/**
 * list_first() - Return the first position of a list.
 * @l: List to inspect.
 *
 * Returns: The first position in the list.
 */
list_pos list_first(const list *l);

/**
 * list_next() - Return the next position in a list.
 * @p: Position to return the next from.
 *
 * Returns: The position in the list after the given position.
 */
list_pos list_next(const list_pos p);

/**
 * list_end() - Check if a given position is at the end of a list.
 * @p: Any valid position in the list.
 *
 * Returns: True if p is at the end of the list.
 */
bool list_end(const list_pos p);

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
list_pos list_insert(void *v, const list_pos p);

/**
 * list_inspect() - Return the value of the element at a given
 *		     position in a list.
 * @p: Any valid position in the list, except the last.
 *
 * Returns: Returns the value at the given position as a void pointer.
 */
void *list_inspect(const list_pos p);

/**
 * list_kill() - Destroy a given list.
 * @l: List to destroy.
 *
 * Return all allocated memory used by the list.
 *
 */
void list_kill(list *l);

#endif
