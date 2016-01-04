#ifndef __VECTOR_H
#define __VECTOR_H

/** @defgroup vector vector
 * @{
 *
 * Functions related to the vector-like container we implemented.
 */


/** @name Vector */
/**@{
 *
 * Vector like dynamic container
 */ 

typedef struct vector {
	int size; /*!< number of elements in the vector */
	void** items; /*!< elements of the vector */
} vector;

/*! \brief Initializes a vector with size 0.
*
*	\param v vector to be initialized.
*/
void vector_init(vector *v);

/*! \brief Adds item to the end of the vector.
*
*	Also updates size.
*
*	\param v vector to be altered.
*	\param item item to add.
*/
void vector_push_back(vector *v, void *item);

/*! \brief Removes element from the index position of the vector.
*
*	Also updates size.
*
*	\param v vector to be altered.
*	\param index position of the element of the vector to erase.
*/
void vector_erase(vector *v, int index);



#endif
