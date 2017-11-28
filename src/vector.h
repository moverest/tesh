#ifndef VECTOR_H_IMPORTED
#define VECTOR_H_IMPORTED

#include <stdlib.h>

typedef struct {
    void   *buf;
    size_t cap;
    size_t size;
    size_t e_size;
} vector_t;

#define VECTOR_DEFAULT_CAP    16
#define VECTOR_GROW_FACTOR    2

// make_vector creates a vector in memory with elements of size `e_size`.
// Returns a NULL value on error.
vector_t *make_vector(size_t e_size);

// make_vector_with_cap does the same as `make_vector` but you can specify
// a starting capacity. Ideal when you already have an idea of the number of
// elements you want to add to the vector.
vector_t *make_vector_with_cap(size_t e_size, size_t cap);

// vector_change_cap updates the capacity of the given vector.
// The new capacity must be larger than the current size.
// Returns a non null value on success.
int vector_change_cap(vector_t *vector, size_t new_cap);

// vector_append appends the given element to the given vector.
// It updates the capacity when needed.
// Returns a non null value on success.
int vector_append(vector_t *vector, void *element);

// vector_reset clears the vector.
// All values remain in memory (your should not count on that) but the `size`
// member is updated.
void vector_reset(vector_t *vector);

// vector_free frees the vector in memory.
void vector_free(vector_t *vector);

// vector_extract_buffer frees the vector structure without freeing the buffer.
// It returns a pointer to the buffer.
// When done with it, just free it.
void *vector_extract_buffer(vector_t *vector);


// To access the values in the vector, you can simply cast the buffer into
// another variable.
// Example: my_element_type *my_buffer_pointer = (my_element_type*) vector->buf;
//
// You then use it as a typical table in C.
// Example: my_buffer_pointer[index]


#endif
