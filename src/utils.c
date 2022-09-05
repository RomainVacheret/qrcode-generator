#include <stdio.h>
#include <assert.h>

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
            matrix->values[top_left_index + (width - 1) + (matrix->capacity * i)] = value;
            // top->values left + 1 -> bottom left
            matrix->values[top_left_index + (matrix->capacity * (i + 1))] = value;
            // bot->valuestom right -> bottom left - 1
            matrix->values[top_left_index + (width - 1) + ((width - 1) * matrix->capacity) - i] = value; 
        }
}

void utils_set_vertical_line(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool value) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->capacity] = value;
        }
}

void utils_set_vertical_line_values(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool* values) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->capacity] = values[i];
        }
}

void utils_set_horizontal_line(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool value) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            matrix->values[row * matrix->capacity + start_column + i] = value;
        }
}

void utils_set_horizontal_line_values(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool* values) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            printf("--> %zu\n", row * matrix->capacity + start_column + i);
            matrix->values[row * matrix->capacity + start_column + i] = values[i];
        }
}
//
//
// size_t utils_get_next_idx(size_t current_idx, size_t matrix_size) {
//     printf("utils_get_next_idx - %zu\n", current_idx);
//     if(!current_idx) {
//         fprintf(stderr, "ERROR: invalid index (0)");
//         exit(1);
//     // Note: there is a column for which all the cells are already reserved 
//     // (vertical timing);
//     } else if((current_idx % 21) == 6) {
//         return 425;// 21 * 6 + 21 - 1;
//     } else if((current_idx % 21) % 2 != (current_idx % 21) < 7) {
//         return current_idx < matrix_size ? 
//             matrix_size * matrix_size - (matrix_size - (current_idx - 1)) : 
//             current_idx - matrix_size + 1;
//     } else {
//             return current_idx - 1;
//     }
// }

size_t utils_get_next_idx(size_t current_idx, size_t matrix_size) {
    printf("utils_get_next_idx - %zu\n", current_idx);
    static bool down = true;
    size_t x;
    // if(!current_idx) {
    //     fprintf(stderr, "ERROR: invalid index (0)");
    //     exit(1);
    // Note: there is a column for which all the cells are already reserved 
    // (vertical timing);
    // }
    if((current_idx % 21) == 6) {
        x = 5;// 21 * 6 + 21 - 1;
        // down = !down;
        // printf("A");
    } else if( ((((current_idx % 21) % 2 != (current_idx % 21) < 7)) && ((current_idx < 21 && down) || (current_idx > 419 && !down)))) {
        x = current_idx - 1;
        down = !down;
        printf("B");
    }
    else if((current_idx % 21) % 2 != (current_idx % 21) < 7){
    // else if((current_idx % 21) % 2 ){
        // x = current_idx < matrix_size ? 
        //     matrix_size * matrix_size - (matrix_size - (current_idx - 1)) : 
        //     current_idx - matrix_size + 1;
        x = current_idx + 1 + (matrix_size * (down ? -1 : 1));
        if(current_idx == 4) printf("§§%zu %d\n", (matrix_size), down);
        printf("C");
    } else {
            x = current_idx - 1;
        // printf("D");
    }

    return x;
}

size_t utils_get_next_available_idx(size_t current_idx, Array* matrix) {
    size_t next_idx;
    int is_available = matrix->values[current_idx] == false;
    size_t debug = current_idx;

    do {
        next_idx = is_available? current_idx : 
            utils_get_next_idx(current_idx, matrix->capacity);
        is_available = matrix->values[next_idx] == false;
        // printf("utils_get_next - %zu %d %d %zu\n", next_idx, is_available, !is_available, debug);
        current_idx = next_idx;
    } while(!is_available);
    // exit(1);

    return next_idx;
}
