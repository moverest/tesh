#include <stdio.h>

#include "test.h"
#include "vector.h"
#include "tools.h"

static void test_vector_creation(test_t *t) {
    vector_t *vector = make_vector(sizeof(int));

    if (vector == NULL) {
        printf("%s:%d: make_vector should not return NULL\n",
               __FILE__, __LINE__);
        test_fail(t);
    }

    if (vector->size != 0) {
        printf("%s:%d: vector->size = %ld != 0\n",
               __FILE__, __LINE__, vector->size);
    }

#define TEST_CAPACITY    30
    vector = make_vector_with_capacity(sizeof(int), TEST_CAPACITY);
    if (vector == NULL) {
        printf("%s:%d: make_vector should not return NULL\n",
               __FILE__, __LINE__);
        test_fail(t);
    }

    if (vector->cap != TEST_CAPACITY) {
        printf("%s:%d: vector->cap = %ld not %d\n",
               __FILE__, __LINE__,
               vector->cap, TEST_CAPACITY);
        test_fail(t);
    }
}


#define TEST_VECTOR_APPEND_NUM_ELEMENTS    2048
static void test_vector_append(test_t *t) {
    vector_t *vector = make_vector(sizeof(int));

    if (vector == NULL) {
        printf("%s:%d: make_vector should not return NULL\n",
               __FILE__, __LINE__);
        test_fail(t);
    }

    for (int i = 0; i < TEST_VECTOR_APPEND_NUM_ELEMENTS; i++) {
        int err = vector_append(vector, &i);
        if (err) {
            printf("%s:%d: vector_append returned non null value (%d)\n",
                   __FILE__, __LINE__, err);
            test_fail(t);
        }
    }


    if (vector->size != TEST_VECTOR_APPEND_NUM_ELEMENTS) {
        printf("%s:%d: vector->size = %ld != %d\n",
               __FILE__, __LINE__, vector->size,
               TEST_VECTOR_APPEND_NUM_ELEMENTS);
        test_fail(t);
    }

    int *buf = (int *)vector->buf;
    for (int i = 0; i < TEST_VECTOR_APPEND_NUM_ELEMENTS; i++) {
        if (buf[i] != i) {
            printf("%s:%d: buf[%d] = %d != %d\n",
                   __FILE__, __LINE__, i, buf[i], i);
            test_fail(t);
        }
    }
}


int main() {
    test_function_t tests[] = {
        TEST_FUNCTION(test_vector_creation),
        TEST_FUNCTION(test_vector_append)
    };

    return test_run(tests, ARRAY_LEN(tests));
}
