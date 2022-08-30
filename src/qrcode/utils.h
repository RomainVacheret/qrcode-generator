#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

typedef struct {
    bool* values;
    size_t capacity;
    size_t size; // current size
} Array;

void utils_set_square_ring(
    Array* matrix,
    size_t top_left_index, 
    size_t width, 
    bool value);


void utils_set_vertical_line(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool value);

void utils_set_vertical_line_values(
    Array* matrix,
    Array* empty,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool* value);

void utils_set_horizontal_line(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool value);

void utils_set_horizontal_line_values(
    Array* matrix,
    Array* empty,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool* value);

size_t utils_get_next_idx(size_t current_idx, size_t matrix_size);
size_t utils_get_next_available_idx(size_t current_idx, Array* matrix);


Array* utils_alloc_array(size_t size);
Array* utils_alloc_array_zeros(size_t size);
Array* utils_alloc_array_values(size_t size, bool* values);
void utils_free_array(Array* self);

Array* utils_concat_arrays_size(Array* arr1, Array* arr2);
Array* utils_concat_arrays(Array* arr1, Array* arr2);
Array* utils_append_arrays(Array* self, Array* other);
Array* utils_append_arrays_full(Array* self, Array* other);
Array* utils_append_array(Array* self, bool value);

void utils_display_array(Array* self);
void utils_display_array_full(Array* self);

#endif // UTILS_H
