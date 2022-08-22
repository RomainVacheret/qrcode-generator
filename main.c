#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#define VERSION 1
#define WIDTH_SIZE 21

typedef struct {
    bool values [WIDTH_SIZE * WIDTH_SIZE];
    bool are_empty[WIDTH_SIZE * WIDTH_SIZE];
} QRCode;

QRCode qrcode;

void qrcode_init() {
    memset(qrcode.values, false, WIDTH_SIZE * WIDTH_SIZE);
    memset(qrcode.are_empty, false, WIDTH_SIZE * WIDTH_SIZE);
}

void utils_display_matrix(FILE* stream, bool matrix[], bool is_qrcode) {
    char info_message[64];

    sprintf(
        info_message, 
        "QR Code version: %d | width size: %d%s\n\n",
        VERSION,
        WIDTH_SIZE,
        is_qrcode ? "" : " | Availability mask");
    fputs(info_message, stream);

    for(size_t i = 0; i < WIDTH_SIZE * WIDTH_SIZE; i++) {
        fputc(" #"[matrix[i]], stream);
        fputc(' ', stream);

        if(i && (i + 1) % WIDTH_SIZE == 0) {
            fputc('\n', stream);
        }
    }
}

void utils_display_availability_mask(FILE* stream) {
    utils_display_matrix(stream, qrcode.are_empty, false);
}

void qrcode_display(FILE* stream) {
    utils_display_matrix(stream, qrcode.values, true);
}

// TODO: Check bounds beforehand
void utils_set_square_ring(size_t top_left_index, size_t width, bool matrix[], bool value) {
    for(size_t i = 0; i < width - 1; i++) {
        // top left  -> top right - 1
        matrix[top_left_index + i] = value; 
        // top right -> bottom right - 1
        matrix[top_left_index + (width - 1) + (WIDTH_SIZE * i)] = value;
        // top left + 1 -> bottom left
        matrix[top_left_index + (WIDTH_SIZE * (i + 1))] = value;
        // bottom right -> bottom left - 1
        matrix[top_left_index + (width - 1) + ((width - 1) * WIDTH_SIZE) - i] = value; 
    }
}

void utils_set_vertical_line(size_t column, size_t start_row, size_t end_row, bool matrix[], bool value) {
    for(size_t i = 0; i < end_row - start_row + 1; i++) {
        matrix[column + (start_row + i) * WIDTH_SIZE] = value;
    }
}

void utils_set_horizontal_line(size_t row, size_t start_column, size_t end_column, bool matrix[], bool value) {
    for(size_t i = 0; i < end_column - start_column + 1; i++) {
        matrix[row * WIDTH_SIZE + start_column + i] = value;
    }
}

void pattern_set_finder_pattern(size_t initial_idx, bool matrix[], bool value) {
    utils_set_square_ring(initial_idx, 7, matrix, value);
    utils_set_square_ring(initial_idx + 2 + 2 * WIDTH_SIZE, 3, matrix, value);
    matrix[initial_idx + 3 + 3 * WIDTH_SIZE] = value;
}

void pattern_set_finder(size_t initial_idx) {
    pattern_set_finder_pattern(initial_idx, qrcode.values, true);
}

void pattern_set_all_finders() {
    pattern_set_finder(0);
    pattern_set_finder(WIDTH_SIZE * 14);
    pattern_set_finder(WIDTH_SIZE - 7);
}

void pattern_reserve_finder(size_t initial_idx) {
    pattern_set_finder_pattern(initial_idx, qrcode.are_empty, true);
    utils_set_square_ring(initial_idx + 1 + WIDTH_SIZE, 5, qrcode.are_empty, true);
}

void pattern_reserve_all_finders() {
    pattern_reserve_finder(0);
    pattern_reserve_finder(WIDTH_SIZE * 14);
    pattern_reserve_finder(WIDTH_SIZE - 7);
}

void pattern_reserve_all_patterns() {
    pattern_reserve_all_finders();
    
    // separators for finder patterns
    utils_set_horizontal_line(7, 0, 7, qrcode.are_empty, true); 
    utils_set_horizontal_line(7, 13, 20, qrcode.are_empty, true); 
    utils_set_horizontal_line(13, 0, 7, qrcode.are_empty, true); 
    utils_set_vertical_line(7, 0, 6, qrcode.are_empty, true); 
    utils_set_vertical_line(7, 13, 20, qrcode.are_empty, true); 
    utils_set_vertical_line(13, 0, 6, qrcode.are_empty, true); 
}

size_t utils_get_next_idx(size_t current_idx) {
    if(!current_idx) {
        fprintf(stderr, "ERROR: invalid index (0)");
        exit(1);
    } else if(current_idx % 2) {
        return current_idx < WIDTH_SIZE ? 
            WIDTH_SIZE * WIDTH_SIZE - (WIDTH_SIZE - (current_idx - 1)) : 
            current_idx - WIDTH_SIZE + 1;
    } else {
            return current_idx - 1;
    }
}

size_t utils_get_next_available_idx(size_t current_idx) {
    size_t next_idx;
    int is_available;

    do {
        next_idx = utils_get_next_idx(current_idx);
        is_available = qrcode.are_empty[next_idx] == false;
    } while(!is_available);

    return next_idx;
}

int main() {
    qrcode_init();
    pattern_set_all_finders();
    size_t before = 439;
    size_t result = utils_get_next_idx(before);
    qrcode.values[before] = '4';
    qrcode.values[result] = '5';
    // printf("%zu", );
    qrcode_display(stdout);
    pattern_reserve_all_patterns();
    utils_display_availability_mask(stdout);
    return EXIT_SUCCESS;
}

