#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "array.h"

#ifndef UTILS_H
#define UTILS_H

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
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool* value);

size_t utils_get_next_idx(size_t current_idx, size_t matrix_size);
size_t utils_get_next_available_idx(size_t current_idx, Array* matrix);

#endif // UTILS_H
