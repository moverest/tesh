#include <string.h>

#include "vector.h"

vector_t *make_vector(size_t e_size) {
    return make_vector_with_cap(e_size, VECTOR_DEFAULT_CAP);
}


vector_t *make_vector_with_cap(size_t e_size, size_t cap) {
    vector_t *v = malloc(sizeof(vector_t));

    if (v == NULL) {
        return NULL;
    }

    v->size   = 0;
    v->cap    = cap;
    v->e_size = e_size;
    v->buf    = malloc(e_size * cap);

    if (v->buf == NULL) {
        free(v);
        return NULL;
    }

    return v;
}


int vector_change_cap(vector_t *vector, size_t new_cap) {
    // We assume that new_cap >= vector->size

    void *new_buf = malloc(vector->e_size * new_cap);

    if (new_buf == NULL) {
        return 1;
    }

    memcpy(new_buf, vector->buf, vector->size * vector->e_size);
    free(vector->buf);
    vector->buf = new_buf;
    vector->cap = new_cap;

    return 0;
}


int vector_append(vector_t *vector, void *element) {
    if (vector->cap < vector->size + 1) {
        int err = vector_change_cap(vector,
                                    vector->cap * VECTOR_GROW_FACTOR);
        if (err) {
            return err;
        }
    }

    memcpy(vector->buf + vector->size * vector->e_size,
           element, vector->e_size);
    vector->size++;
    return 0;
}


void vector_reset(vector_t *vector) {
    vector->size = 0;
}


void vector_free(vector_t *vector) {
    free(vector->buf);
    free(vector);
}


void *vector_extract_buffer(vector_t *vector) {
    void *buf = vector->buf;

    free(vector);
    return buf;
}
