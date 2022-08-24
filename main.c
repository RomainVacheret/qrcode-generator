#include <string.h>

#include "src/qrcode/information.h"
#include "src/qrcode/qrcode.h"
#include "src/qrcode/pattern.h"
#include "src/qrcode/utils.h"
#include "src/qrcode/encoding.h"

#define WIDTH_SIZE 21
#define VERSION 1

int main() {
    QRCode* qrcode = qrcode_init(WIDTH_SIZE, VERSION);
    Array value_matrix = {qrcode->matrix, WIDTH_SIZE, WIDTH_SIZE};
    Array mask_matrix = {qrcode->are_empty, WIDTH_SIZE, WIDTH_SIZE};
    // size_t before = 439;
    // size_t result = utils_get_next_idx(before, WIDTH_SIZE);
    bool booleans[] = {true, true,};
    Array information = {booleans, 2, 2};

    // qrcode->matrix[before] = '4';
    // qrcode->matrix[result] = '5';

    pattern_set_all_finders(&value_matrix);
    pattern_reserve_all_patterns(&mask_matrix);

    qrcode_insert_information(qrcode, &information, WIDTH_SIZE * WIDTH_SIZE - 1);

    qrcode_display(qrcode, stdout);
    qrcode_display_availability_mask(qrcode, stdout);

    Array* a = encoding_encode_int_to_binary(255, 8);
    utils_display_array(a);

    char* alph_string = "HELLO WORLD";
    EncodingMode mode = ALPHANUMERIC;
    size_t message_length = strlen(alph_string);
    size_t codewords_count = encoding_get_number_codewords(VERSION, mode);
    Array* binary_encoding_mode = information_get_encoding_mode(mode);
    Array* binary_count_indicator = encoding_encode_int_to_binary(message_length, 9);
    Array* result = utils_alloc_array(codewords_count * 8);
    Array* alph_encoding = encoding_encode_alphanumeric_string(alph_string);

    utils_append_arrays(result, binary_encoding_mode);
    utils_append_arrays(result, binary_count_indicator);
    utils_append_arrays(result, alph_encoding);
    encoding_pad_codewords(result);

    utils_display_array(result);

    // I know, I should free all the allocated memory
    return EXIT_SUCCESS;
}

