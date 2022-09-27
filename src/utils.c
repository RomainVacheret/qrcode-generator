#include <stdio.h>
#include <assert.h>

#include "utils.h"

void set_square_ring(
    Array* matrix,
    size_t top_left_index, 
    size_t width, 
    bool value) {
        for(size_t i = 0; i < width - 1; i++) {
            // top left  -> top right - 1
            matrix->values[top_left_index + i] = value; 
            // top->values right -> bottom right - 1
            matrix->values[top_left_index + (width - 1) + (matrix->capacity * i)] = value;
            // top->values left + 1 -> bottom left
            matrix->values[top_left_index + (matrix->capacity * (i + 1))] = value;
            // bot->valuestom right -> bottom left - 1
            matrix->values[top_left_index + (width - 1) + ((width - 1) * matrix->capacity) - i] = value; 
        }
}

void set_vertical_line(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool value) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->capacity] = value;
        }
}

void set_vertical_line_values(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool* values) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->capacity] = values[i];
        }
}

void set_horizontal_line(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool value) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            matrix->values[row * matrix->capacity + start_column + i] = value;
        }
}

void set_horizontal_line_values(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool* values) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            matrix->values[row * matrix->capacity + start_column + i] = values[i];
        }
}

size_t get_next_idx(size_t current_idx, size_t matrix_size) {
    static bool down = true;
    size_t x;

    if((current_idx % 21) == 6) {
        x = 5;
    } else if( ((((current_idx % 21) % 2 != (current_idx % 21) < 7)) && ((current_idx < 21 && down) || (current_idx > 419 && !down)))) {
        x = current_idx - 1;
        down = !down;
    }
    else if((current_idx % 21) % 2 != (current_idx % 21) < 7){
        x = current_idx + 1 + (matrix_size * (down ? -1 : 1));
    } else {
        x = current_idx - 1;
    }

    return x;
}

size_t get_next_available_idx(size_t current_idx, Array* matrix) {
    size_t next_idx;
    int is_available = matrix->values[current_idx] == false;

    do {
        next_idx = is_available? current_idx : 
            get_next_idx(current_idx, matrix->capacity);
        is_available = matrix->values[next_idx] == false;
        current_idx = next_idx;
    } while(!is_available);

    return next_idx;
}
