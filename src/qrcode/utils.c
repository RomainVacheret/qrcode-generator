
#include "utils.h"

void utils_set_square_ring(
    Array* matrix,
    size_t top_left_index, 
    size_t width, 
    bool value) {
        for(size_t i = 0; i < width - 1; i++) {
            // top left  -> top right - 1
            matrix->values[top_left_index + i] = value; 
            // top->values right -> bottom right - 1
            matrix->values[top_left_index + (width - 1) + (matrix->size * i)] = value;
            // top->values left + 1 -> bottom left
            matrix->values[top_left_index + (matrix->size * (i + 1))] = value;
            // bot->valuestom right -> bottom left - 1
            matrix->values[top_left_index + (width - 1) + ((width - 1) * matrix->size) - i] = value; 
        }
}

void utils_set_vertical_line(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool value) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->size] = value;
        }
}

void utils_set_horizontal_line(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool value) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            matrix->values[row * matrix->size + start_column + i] = value;
        }
}

size_t utils_get_next_idx(size_t current_idx, size_t matrix_size) {
    if(!current_idx) {
        fprintf(stderr, "ERROR: invalid index (0)");
        exit(1);
    } else if(current_idx % 2) {
        return current_idx < matrix_size ? 
            matrix_size * matrix_size - (matrix_size - (current_idx - 1)) : 
            current_idx - matrix_size + 1;
    } else {
            return current_idx - 1;
    }
}

size_t utils_get_next_available_idx(size_t current_idx, Array* matrix) {
    size_t next_idx;
    int is_available;

    do {
        next_idx = utils_get_next_idx(current_idx, matrix->size);
        is_available = matrix->values[next_idx] == false;
    } while(!is_available);

    return next_idx;
}
