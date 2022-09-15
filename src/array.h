#include <stdlib.h>
#include <stdbool.h>

#ifndef ARRAY_H
#define ARRAY_H


typedef struct {
    bool* values;
    size_t capacity;
    size_t size; // current size
} Array;

Array* array_alloc(size_t size);
Array* array_alloc_zeros(size_t size);
Array* array_alloc_values(size_t size, bool* values);

Array* array_concat(Array* arr1, Array* arr2);
Array* array_concat_full(Array* arr1, Array* arr2);

Array* array_append(Array* self, Array* other);
Array* array_append_full(Array* self, Array* other);
Array* array_append_value(Array* self, bool value);

void array_free(Array* self);
void array_display(Array* self);
void array_display_full(Array* self);
char* array_as_string(Array* self);
Array* array_copy(Array* self);

#endif // ARRAY_H
