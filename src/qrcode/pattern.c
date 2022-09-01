#include "pattern.h"

#define _VERSION 1
#define _SIZE 21

void pattern_set_finder_pattern(Array* matrix, size_t initial_idx, bool value) {
    utils_set_square_ring(matrix, initial_idx, 7, value);
    utils_set_square_ring(matrix, initial_idx + 1 + 1 * matrix->size, 5, !value);
    utils_set_square_ring(matrix, initial_idx + 2 + 2 * matrix->size, 3, value);
    matrix->values[initial_idx + 3 + 3 * matrix->size] = value;
}

void pattern_set_finder(Array* matrix, size_t initial_idx) {
    pattern_set_finder_pattern(matrix, initial_idx, true);
}

void pattern_set_all_finders(Array* matrix) {
    pattern_set_finder(matrix, 0);
    pattern_set_finder(matrix, matrix->size * 14);
    pattern_set_finder(matrix, matrix->size - 7);

    utils_set_horizontal_line(matrix,7, 0, 7, false); 
    utils_set_horizontal_line(matrix, 7, 13, 20, false); 
    utils_set_horizontal_line(matrix, 13, 0, 7, false); 
    utils_set_vertical_line(matrix, 7, 0, 6, false); 
    utils_set_vertical_line(matrix, 7, 13, 20, false); 
    utils_set_vertical_line(matrix, 13, 0, 6, false); 
}

void pattern_set_timings_n_dark(Array* matrix) {
    bool timing[13] = {
        true, false, true, false, true, false,
        true, false, true, false, true, false, true};
    
    utils_set_horizontal_line_values(matrix, 6, 8, 21 - 1 - 8, timing);
    utils_set_vertical_line_values(matrix, 6, 8, 21 - 1 - 8, timing);

    //Note: dark module postion: ([4 * V + 9], 8)
    // TODO: does not seem to appear
    matrix->values[(4 * _VERSION + 9) * _SIZE + 8] = true;
}

void pattern_reserve_finder(Array* matrix, size_t initial_idx) {
    pattern_set_finder_pattern(matrix, initial_idx, true);
    utils_set_square_ring(matrix, initial_idx + 1 + matrix->size, 5, true);
}

void pattern_reserve_all_finders(Array* matrix) {
    pattern_reserve_finder(matrix, 0);
    pattern_reserve_finder(matrix, matrix->size * 14);
    pattern_reserve_finder(matrix, matrix->size - 7);

    utils_set_horizontal_line(matrix,7, 0, 7, false); 
    utils_set_horizontal_line(matrix, 7, 13, 20, false); 
    utils_set_horizontal_line(matrix, 13, 0, 7, false); 
    utils_set_vertical_line(matrix, 7, 0, 6, false); 
    utils_set_vertical_line(matrix, 7, 13, 20, false); 
    utils_set_vertical_line(matrix, 13, 0, 6, false); 
}

void pattern_reserve_all_patterns(Array* matrix) {
    pattern_reserve_all_finders(matrix);
    
    // separators for finder patterns
    utils_set_horizontal_line(matrix,7, 0, 7, true); 
    utils_set_horizontal_line(matrix, 7, 13, 20, true); 
    utils_set_horizontal_line(matrix, 13, 0, 7, true); 
    utils_set_vertical_line(matrix, 7, 0, 6, true); 
    utils_set_vertical_line(matrix, 7, 13, 20, true); 
    utils_set_vertical_line(matrix, 13, 0, 6, true); 

    // format information
    utils_set_vertical_line(matrix, 8, 0, 8, true);
    utils_set_vertical_line(matrix, 8, 13, 20, true);
    utils_set_horizontal_line(matrix, 8, 0, 7, true);
    utils_set_horizontal_line(matrix, 8, 13, 20, true);

    // timing patterns
    utils_set_horizontal_line(matrix, 6, 9, 12, true);
    utils_set_vertical_line(matrix, 6, 8, 12, true);
}

