#pragma once

/*
https://github.com/eteran/c-vector
A dynamic array impl in C99
just use a Type * (or vector(Type) to make it clear it's a vec) and set it to NULL initially

Define LINEAR_GROWTH to add to capacity instead of multiplying it, and define GROWTH_FACTOR to change
the amount the capacity is added/multiplied by (defaults are 1 if LINEAR_GROWTH, 2 otherwise)

If LINEAR_GROWTH is not set, you can set MIN_CAPACITY to change the initial capacity of the vector.
Default 8.

interface:
    v_set_capacity(vec, new_capacity)
	    set the capacity (allocated size) of the vector. Does not realloc, just changes recorded size
	v_set_size(vec, new_size)
	    change recorded number of items in vector
	v_size(vec)
	    get number of items in vector
	v_capacity(vec)
	    get capacity of vector
	v_empty(vec)
	    see if vector is empty
	v_grow(vec, new_capacity)
	    change capacity of the vector. Realloc's
	v_pop_back(vec)
	    deletes the last item of the vector. Does not return it
	v_append(vec)
	    add an item to the vector. Handles realloc
	v_back(vec)
	    get the last item in the vector (as lvalue)
    v_free(vec)
	    free the vector
	v_erase(vec, index)
	    delete an item from the vector
    v_begin(vec)
	    get a pointer to the start of the data
	v_end(vec)
	    get a pointer to the end of the data
*/

#ifndef GROWTH_FACTOR
  #ifdef LINEAR_GROWTH
    #define GROWTH_FACTOR 1
  #else
    #define GROWTH_FACTOR 2
	#ifndef MIN_CAPACITY
	  #define MIN_CAPACITY 8
	#endif
  #endif
#endif

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc/realloc/free */
#include <assert.h> /* for assert */

#define vector(t) t*

/**
 * @brief v_set_capacity - For internal use, sets the capacity variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define v_set_capacity(vec, size)   \
do {                                     \
	if(vec) {                            \
		((size_t *)(vec))[-1] = (size);  \
	}                                    \
} while(0)

/**
 * @brief v_set_size - For internal use, sets the size variable of the vector
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define v_set_size(vec, size)      \
do {                                    \
	if(vec) {                           \
		((size_t *)(vec))[-2] = (size); \
	}                                   \
} while(0)

/**
 * @brief v_capacity - gets the current capacity of the vector
 * @param vec - the vector
 * @return the capacity as a size_t
 */
#define v_capacity(vec) \
	((vec) ? ((size_t *)(vec))[-1] : (size_t)0)

/**
 * @brief v_size - gets the current size of the vector
 * @param vec - the vector
 * @return the size as a size_t
 */
#define v_size(vec) \
	((vec) ? ((size_t *)(vec))[-2] : (size_t)0)

/**
 * @brief v_empty - returns non-zero if the vector is empty
 * @param vec - the vector
 * @return non-zero if empty, zero if non-empty
 */
#define v_empty(vec) \
	((vec) ? v_size(vec) == 0 : 0)

/**
 * @brief v_grow - For internal use, ensures that the vector is at least <count> elements big
 * @param vec - the vector
 * @param size - the new capacity to set
 * @return void
 */
#define v_grow(vec, count) \
do {                                                                                    \
	if(!(vec)) {                                                                        \
		size_t *__p = malloc((count) * sizeof(*(vec)) + (sizeof(size_t) * 2));          \
		assert(__p);                                                                    \
		(vec) = (void *)(&__p[2]);                                                      \
		v_set_capacity((vec), (count));                                            \
		v_set_size((vec), 0);                                                      \
	} else {                                                                            \
		size_t *__p1 = &((size_t *)(vec))[-2];                                          \
		size_t *__p2 = realloc(__p1, ((count) * sizeof(*(vec))+ (sizeof(size_t) * 2))); \
		assert(__p2);                                                                   \
		(vec) = (void *)(&__p2[2]);                                                     \
		v_set_capacity((vec), (count));                                            \
	}                                                                                   \
} while(0)

/**
 * @brief v_pop_back - removes the last element from the vector
 * @param vec - the vector
 * @return void
 */
#define v_pop_back(vec) \
do {                                              \
	v_set_size((vec), v_size(vec) - 1); \
} while(0)

/**
 * @brief v_back - get the last element of the vector
 * @param vec - the vector
 * @return the array type
 */
#define v_back(vec) \
	vec[v_size(vec)-1]

/**
 * @brief v_erase - removes the element at index i from the vector
 * @param vec - the vector
 * @param i - index of element to remove
 * @return void
 */
#define v_erase(vec, i) \
do {                                                   \
	if (vec) {                                         \
		const size_t __sz = v_size(vec);          \
		if ((i) < __sz) {                              \
			v_set_size((vec), __sz - 1);          \
			size_t __x;                                \
			for (__x = (i); __x < (__sz - 1); ++__x) { \
				(vec)[__x] = (vec)[__x + 1];           \
			}                                          \
		}                                              \
   }                                                   \
} while(0)

/**
 * @brief v_free - frees all memory associated with the vector
 * @param vec - the vector
 * @return void
 */
#define v_free(vec) \
do { \
	if(vec) {                                \
		size_t *p1 = &((size_t *)(vec))[-2]; \
		free(p1);                            \
	}                                        \
} while(0)

/**
 * @brief v_begin - returns an iterator to first element of the vector
 * @param vec - the vector
 * @return a pointer to the first element (or NULL)
 */
#define v_begin(vec) \
	(vec)

/**
 * @brief v_end - returns an iterator to one past the last element of the vector
 * @param vec - the vector
 * @return a pointer to one past the last element (or NULL)
 */
#define v_end(vec) \
	((vec) ? &((vec)[v_size(vec)]) : NULL)


/**
 * @brief v_append - adds an element to the end of the vector
 * @param vec - the vector
 * @param value - the value to add
 * @return void
 */
#ifdef LINEAR_GROWTH

#define v_append(vec, value) \
do {                                                         			\
	size_t __cap = v_capacity(vec);                    			\
	if(__cap <= v_size(vec)) {                       		        \
		v_grow((vec), !__cap ? __cap + 1 : __cap + GROWTH_FACTOR); \
	}                                                       			\
	vec[v_size(vec)] = (value);                        			\
	v_set_size((vec), v_size(vec) + 1);           			\
} while(0)

#else

#define v_append(vec, value) \
do {                                              		   \
	size_t __cap = v_capacity(vec);             	   \
	if(__cap <= v_size(vec)) {        			       \
		v_grow((vec), __cap ? __cap * GROWTH_FACTOR : \
		 						   MIN_CAPACITY);          \
	}                                            		   \
	vec[v_size(vec)] = (value);          			   \
	v_set_size((vec), v_size(vec) + 1);    	   \
} while(0)

#endif
