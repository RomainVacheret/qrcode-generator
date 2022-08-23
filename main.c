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
    // int* int_vals = encoding_alphanumeric_values(alph_string, 2);
    // printf("%d", int_vals[0]);
    Array* alph_encoding = encoding_encode_alphanumeric_string(alph_string);
    utils_display_array(alph_encoding);

    return EXIT_SUCCESS;
}

