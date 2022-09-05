#include <stdbool.h>
#include <string.h>

#include "src/information.h"
#include "src/qrcode.h"
#include "src/pattern.h"
#include "src/array.h"
#include "src/utils.h"
#include "src/encoding.h"
#include "src/log_antilog.h"
#include "src/polynomial.h"

#define WIDTH_SIZE 21
#define VERSION 1

int main() {
    char* alph_string = "HELLO WORLD";
    EncodingMode mode = ALPHANUMERIC;
    size_t codewords_count = encoding_get_number_codewords(VERSION, mode);
    QRCode* qrcode = qrcode_init(WIDTH_SIZE, VERSION);
    Array value_matrix = {qrcode->matrix, WIDTH_SIZE, WIDTH_SIZE};
    Array mask_matrix = {qrcode->are_taken, WIDTH_SIZE, WIDTH_SIZE};
    Array* format_version = information_compute_format(
        information_get_error_correction_level(M),
        array_alloc_values(3, (bool[3]) {false, true, false})
    );

    Array* binary_encoding_mode = information_get_encoding_mode(mode);
    Array* binary_count_indicator = encoding_encode_int_to_binary(strlen(alph_string), 9);
    Array* binary_information = array_alloc(codewords_count * 8);
    Array* alph_encoding = encoding_encode_alphanumeric_string(alph_string);

    pattern_reserve_all_patterns(&mask_matrix);

    array_append(binary_information, binary_encoding_mode);
    array_append(binary_information, binary_count_indicator);
    array_append(binary_information, alph_encoding);
    encoding_pad_codewords(binary_information);

    // Note: error correction step 
    Polynomial* information_poly = polynomial_create_from_info(binary_information);
    Array* final_array = array_alloc(binary_information->size + 8 * information_poly->degree);

    polynomial_devide2(information_poly);
    array_append(final_array, binary_information);

    // Note: add the binary encoded error corrections
    for(size_t i = 0; i < information_poly->degree; i++) {
        Array* int_to_bin = encoding_encode_int_to_binary(information_poly->values[i], 8);
        array_append(final_array, int_to_bin);
        array_free(int_to_bin);
    }

    qrcode_insert_information(qrcode, final_array, WIDTH_SIZE * WIDTH_SIZE - 1);

    // Note: masking step
    for(size_t i = 0; i < qrcode->size * qrcode->size; i++) {
        qrcode->matrix[i] ^= ((i % 21) % 3 == 0);
    }

    // Note: these must not be masked
    qrcode_insert_version_format(qrcode, format_version);
    pattern_set_all_finders(&value_matrix);
    pattern_set_timings_n_dark(&value_matrix);

    qrcode_display(qrcode, stdout);
    qrcode_display_availability_mask(qrcode, stdout);

    // TODO: replace the Python scrip with OpenGL
    // Note: export to display using the Python script
    FILE* file = fopen("result.txt", "w");
    qrcode_display(qrcode, file);
    fclose(file);

    // TODO: close the log-antilog file
    // TODO: look for all the memory leaks (there are a lot)
    return EXIT_SUCCESS;
}
