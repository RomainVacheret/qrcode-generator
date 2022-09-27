#include "pattern.h"

#define _VERSION 1
#define _SIZE 21

void set_finder_pattern(Array* matrix, size_t initial_idx, bool value) {
    set_square_ring(matrix, initial_idx, 7, value);
    set_square_ring(matrix, initial_idx + 1 + 1 * matrix->size, 5, !value);
    set_square_ring(matrix, initial_idx + 2 + 2 * matrix->size, 3, value);
    matrix->values[initial_idx + 3 + 3 * matrix->size] = value;
}

void set_finder(Array* matrix, size_t initial_idx) {
    set_finder_pattern(matrix, initial_idx, true);
}

void set_all_finders(Array* matrix) {
    set_finder(matrix, 0);
    set_finder(matrix, matrix->size * 14);
    set_finder(matrix, matrix->size - 7);

    set_horizontal_line(matrix,7, 0, 7, false); 
    set_horizontal_line(matrix, 7, 13, 20, false); 
    set_horizontal_line(matrix, 13, 0, 7, false); 
    set_vertical_line(matrix, 7, 0, 6, false); 
    set_vertical_line(matrix, 7, 13, 20, false); 
    set_vertical_line(matrix, 13, 0, 6, false); 
}

void set_timings_n_dark(Array* matrix) {
    bool timing[13] = {
        true, false, true, false, true, false,
        true, false, true, false, true, false, true};
    
    set_horizontal_line_values(matrix, 6, 8, 21 - 1 - 8, timing);
    set_vertical_line_values(matrix, 6, 8, 21 - 1 - 8, timing);

    //Note: dark module postion: ([4 * V + 9], 8)
    // TODO: does not seem to appear
    matrix->values[(4 * _VERSION + 9) * _SIZE + 8] = true;
}

void reserve_finder(Array* matrix, size_t initial_idx) {
    set_finder_pattern(matrix, initial_idx, true);
    set_square_ring(matrix, initial_idx + 1 + matrix->size, 5, true);
}

void reserve_all_finders(Array* matrix) {
    reserve_finder(matrix, 0);
    reserve_finder(matrix, matrix->size * 14);
    reserve_finder(matrix, matrix->size - 7);

    set_horizontal_line(matrix,7, 0, 7, false); 
    set_horizontal_line(matrix, 7, 13, 20, false); 
    set_horizontal_line(matrix, 13, 0, 7, false); 
    set_vertical_line(matrix, 7, 0, 6, false); 
    set_vertical_line(matrix, 7, 13, 20, false); 
    set_vertical_line(matrix, 13, 0, 6, false); 
}

void reserve_all_patterns(Array* matrix) {
    reserve_all_finders(matrix);
    
    // separators for finder patterns
    set_horizontal_line(matrix,7, 0, 7, true); 
    set_horizontal_line(matrix, 7, 13, 20, true); 
    set_horizontal_line(matrix, 13, 0, 7, true); 
    set_vertical_line(matrix, 7, 0, 6, true); 
    set_vertical_line(matrix, 7, 13, 20, true); 
    set_vertical_line(matrix, 13, 0, 6, true); 

    // format information
    set_vertical_line(matrix, 8, 0, 8, true);
    set_vertical_line(matrix, 8, 13, 20, true);
    set_horizontal_line(matrix, 8, 0, 7, true);
    set_horizontal_line(matrix, 8, 13, 20, true);

    // timing patterns
    set_horizontal_line(matrix, 6, 9, 12, true);
    set_vertical_line(matrix, 6, 8, 12, true);
}

