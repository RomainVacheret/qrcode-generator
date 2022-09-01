#include <string.h>

#include "src/qrcode/information.h"
#include "src/qrcode/qrcode.h"
#include "src/qrcode/pattern.h"
#include "src/qrcode/utils.h"
#include "src/qrcode/encoding.h"
#include "src/qrcode/log_antilog.h"
#include "src/qrcode/polynomial.h"

#define WIDTH_SIZE 21
#define VERSION 1

int c = 0;
size_t get_next_idx(size_t current_idx, size_t matrix_size) {
    // printf("utils_get_next_idx - %zu\n", current_idx);
    size_t x;
    c++;

    if(!current_idx) {
        fprintf(stderr, "ERROR: invalid index (0)");
        exit(1);
    } else if((current_idx % 21) % 2 != (current_idx % 21) < 9){
        x = current_idx < matrix_size ? 
            matrix_size * matrix_size - (matrix_size - (current_idx - 1)) : 
            current_idx - matrix_size + 1;
    } else {
            x = current_idx - 1;
    }


    printf("%zu ", x);
    return x;
}

int main2() {
    size_t x = 21 * 21 - 1;
    // size_t x = 190;
    while(c < 61) {
         x = get_next_idx(x, 21);

        if(!x) {
            break;
        }
    }

    return 0;
}

int main() {
    QRCode* qrcode = qrcode_init(WIDTH_SIZE, VERSION);
    Array value_matrix = {qrcode->matrix, WIDTH_SIZE, WIDTH_SIZE};
    Array mask_matrix = {qrcode->are_taken, WIDTH_SIZE, WIDTH_SIZE};
    // size_t before = 439;
    // size_t result = utils_get_next_idx(before, WIDTH_SIZE);
    bool booleans[] = {true, true,};
    Array information = {booleans, 2, 2};
    Array* format_version = information_compute_format(
        information_get_error_correction_level(L),
        utils_alloc_array_values(3, (bool[3]) {false, true, false})
    );

    // qrcode->matrix[before] = '4';
    // qrcode->matrix[result] = '5';

    pattern_reserve_all_patterns(&mask_matrix);

    // qrcode_insert_information(qrcode, &information, WIDTH_SIZE * WIDTH_SIZE - 1);
    qrcode_insert_version_format(qrcode, format_version);
    


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
    printf("-----\n");

    // Array* info = utils_concat_arrays(
    //
    //     information_get_error_correction_level(L),
    //     utils_alloc_array_values(3, (bool[3]) {true, false, false})
    // );
    // Array* x = information_generate_format_information(info);
    // utils_display_array(x);
    //
    // information_remove_leading_zeros(x);
    // utils_display_array(x);
    //
    // information_devide_full(x);
    // information_xor_format_information(x);


    utils_display_array(format_version);
    //
    Polynomial* generator = polynomial_get_1M_generator();
    polynomal_mul_alpha(generator, 5);
    // generator->added_degree = 5;

    polynomial_display(generator);
    polynomial_convert(generator);
    polynomial_display(generator);
    polynomial_convert(generator);
    polynomial_display(generator);
    polynomial_pop(generator);
    polynomial_display(generator);

    Array* arr = utils_alloc_array(8);
    arr->size = 8;

    // for(size_t i = 0; i < 8; i++) {
    //     arr->values[i] = resul[i];
    // }

    memcpy(arr->values, result->values + 16, 8);
    utils_display_array(arr);

    printf("DECODE %d %p %p\n", encoding_decode_binary_to_int(arr), arr->values, result);

    Polynomial* poly = polynomial_create_from_info(result);
    polynomial_display(poly);
    // Polynomial* alpha = polynomial_convert_new(poly); 
    // polynomial_display(alpha);

    Polynomial* gen = polynomial_get_1M_generator();

    // TODO: issue with the degree, we need 2 values
    // - one with the minimum degree
    // - one with the 
    polynomial_devide2(poly);

    // I know, I should free all the allocated memory
    qrcode_display(qrcode, stdout);

    // --------------------

    // Array correction = poly->values;
    Array* final_array = utils_alloc_array(result->size + 8 * poly->degree);
    utils_append_arrays(final_array, result);

    for(size_t i = 0; i < poly->degree; i++) {
        Array* int_to_bin = encoding_encode_int_to_binary(poly->values[i], 8);
        utils_append_arrays(final_array, int_to_bin);
        utils_free_array(int_to_bin);
    }

    printf("FINAL FINAL FINAL\n");
    utils_display_array(final_array);

    qrcode_display_availability_mask(qrcode, stdout);
    printf("%zu\n", final_array->size);

    // return 0;

    qrcode_insert_information(qrcode, final_array, WIDTH_SIZE * WIDTH_SIZE - 1);


    // masking

    for(size_t i = 0; i < qrcode->size * qrcode->size; i++) {
        qrcode->matrix[i] ^= ((i % 21) % 3 == 0);
    }

    pattern_set_all_finders(&value_matrix);
    pattern_set_timings_n_dark(&value_matrix);

    qrcode_display(qrcode, stdout);
    qrcode_display_availability_mask(qrcode, stdout);
    FILE* file = fopen("result.txt", "w");

    qrcode_display(qrcode, file);
    fclose(file);

    //TODO: close the log-antilog file
    return EXIT_SUCCESS;
}

