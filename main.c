#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define VERSION 1
#define WIDTH_SIZE 21

typedef struct {
    char values [WIDTH_SIZE * WIDTH_SIZE];
    char are_empty[WIDTH_SIZE * WIDTH_SIZE];
} QRCode;

QRCode qrcode;

void qrcode_init() {
    memset(qrcode.values, ' ', WIDTH_SIZE * WIDTH_SIZE);
    memset(qrcode.are_empty, ' ', WIDTH_SIZE * WIDTH_SIZE);
}

void qrcode_display(FILE* stream) {
    char info_message[64];

    sprintf(info_message, "QR Code version: %d | width size: %d\n\n", VERSION, WIDTH_SIZE);
    fputs(info_message, stream);

    for(size_t i = 0; i < WIDTH_SIZE * WIDTH_SIZE; i++) {
        fputc(qrcode.values[i], stream);
        fputc(' ', stream);

        if(i && (i + 1) % WIDTH_SIZE == 0) {
            fputc('\n', stream);
        }
    }
}

// TODO: Check bounds beforehand
void utils_set_square_ring(size_t top_left_index, size_t width, char string[], char value) {
    for(size_t i = 0; i < width - 1; i++) {
        // top left  -> top right - 1
        string[top_left_index + i] = value; 
        // top right -> bottom right - 1
        string[top_left_index + (width - 1) + (WIDTH_SIZE * i)] = value;
        // top left + 1 -> bottom left
        string[top_left_index + (WIDTH_SIZE * (i + 1))] = value;
        // bottom right -> bottom left - 1
        string[top_left_index + (width - 1) + ((width - 1) * WIDTH_SIZE) - i] = value; 
    }
}

void pattern_set_finder_pattern(size_t initial_idx, char string[], char value) {
    utils_set_square_ring(initial_idx, 7, string, value);
    utils_set_square_ring(initial_idx + 2 + 2 * WIDTH_SIZE, 3, string, value);
    string[initial_idx + 3 + 3 * WIDTH_SIZE] = value;
}

void pattern_set_finder(size_t initial_idx) {
    pattern_set_finder_pattern(initial_idx, qrcode.values, '#');
}

void pattern_set_all_finders() {
    pattern_set_finder(0);
    pattern_set_finder(WIDTH_SIZE * 14);
    pattern_set_finder(WIDTH_SIZE - 7);

}

void pattern_reserve_finder(size_t initial_idx) {
    pattern_set_finder_pattern(initial_idx, qrcode.are_empty, ' ');
}

int main() {
    qrcode_init();
    pattern_set_all_finders();
    qrcode_display(stdout);
    return EXIT_SUCCESS;
}

