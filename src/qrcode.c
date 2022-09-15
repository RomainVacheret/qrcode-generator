#include <assert.h>

#include "qrcode.h"
#include "pattern.h"
#include "polynomial.h"

#include "logger.h"

QRCode* qrcode_alloc(size_t size, int version) {
    QRCode* qrcode = (QRCode*) malloc(sizeof(QRCode));

    qrcode->matrix = (bool*) calloc(size * size, sizeof(bool));
    qrcode->are_taken = (bool*) calloc(size* size, sizeof(bool) * size);
    qrcode->size = size;
    qrcode->version = version;

    return qrcode;
}

void qrcode_free(QRCode* self) {
    free(self->matrix);
    free(self->are_taken);
    free(self);
}

static void qrcode_display_matrix(FILE* stream, size_t size, int version, bool* matrix, bool is_qrcode) {
    char info_message[64];

    sprintf(
        info_message, 
        "QR Code version: %d | width size: %zu%s\n\n",
        version,
        size,
        is_qrcode ? "" : " | Availability mask");
    fputs(info_message, stream);

    for(size_t i = 0; i < size * size; i++) {
        fputc(" #"[matrix[i]], stream);
        fputc(' ', stream);

        if(i && (i + 1) % size == 0) {
            fputc('\n', stream);
        }
    }
}

void qrcode_display(QRCode* self, FILE* stream) {
    qrcode_display_matrix(stream, self->size, self->version, self->matrix, true);
}

void qrcode_display_availability_mask(QRCode* self, FILE* stream) {
    qrcode_display_matrix(stream, self->size, self->version, self->are_taken, false);
}

void qrcode_insert_information(QRCode* self, Array* information, size_t current_idx) {
    Array matrix = {self->are_taken, self->size, self->size};
    char* str;

    // TODO improve performance: at the moment need to go through all 
    // cells each time
    for(size_t i = 0; i < information->size; i++) {
        current_idx = utils_get_next_available_idx(current_idx, &matrix);
        asprintf(&str, "qrcode_insert_information - %zu, %zu, %d, %d %d", 
            i, 
            current_idx, 
            self->matrix[current_idx],
            self->are_taken[current_idx],
            information->values[i]
        );
        self->matrix[current_idx] = information->values[i];
        self->are_taken[current_idx] = true;

        logger_write(LOGGER, str, DEBUG);
        free(str);
    }
}

void qrcode_insert_version_format(QRCode* self, Array* information) {
    for(size_t i = 0; i < 7; i++) {
        size_t h_idx = 8 * self->size + self->size - 7 + i;
        size_t v_idx = 8 + (self->size - 7 + i) * self->size;

        self->matrix[h_idx] = information->values[8 + i];
        // self->matrix[h_idx] = information->values[6 - i];
        self->matrix[v_idx] = information->values[6 - i];
    }
    
    for(size_t i = 0; i < 6; i++) {
        size_t h_idx = 8 * self->size + i;
        size_t v_idx = 8 + i * self->size;

        self->matrix[h_idx] = information->values[i];
         // self->matrix[h_idx] = information->values[information->size - 1 - i];
        self->matrix[v_idx] = information->values[information->size - 1 - i];
    }

    self->matrix[8 * self->size + 7] = information->values[6];
    // self->matrix[8 * self->size + 7] = information->values[8];
    self->matrix[8 * self->size + 8] = information->values[7];
    // TODO: This one is false? It covers the first timing
    self->matrix[7 * self->size + 8] = information->values[8];
    // self->matrix[7 * self->size + 8] = information->values[6];
    self->matrix[8 * self->size + self->size - 7 - 1] = information->values[7];
}

// TODO: add other versions
static size_t get_size_from_version(int version) {
    assert(version == 1);
    return 21;
}

static Array* process_information(
    QRCode* qrcode, 
    char* string, 
    EncodingMode encoding_mode) {
        size_t codewords_count = encoding_get_number_codewords(qrcode->version, encoding_mode);
        Array* binary_encoding_mode = information_get_encoding_mode(encoding_mode);
        // TODO: change length depending on the encoding mode
        Array* binary_count_indicator = encoding_encode_int_to_binary(strlen(string), 9);
        Array* binary_information = array_alloc(codewords_count * 8);
        Array* alph_encoding = encoding_encode_alphanumeric_string(string);

        array_append(binary_information, binary_encoding_mode);
        array_append(binary_information, binary_count_indicator);
        array_append(binary_information, alph_encoding);
        encoding_pad_codewords(binary_information);

        array_free(binary_encoding_mode);
        array_free(binary_count_indicator);
        array_free(alph_encoding);

        return binary_information;
}

QRCode* qrcode_generate(
    char* string, 
    ErrorCorrectionLevel correction_mode,
    EncodingMode encoding_mode, 
    MaskPattern mask,
    int version) {

    QRCode* qrcode = qrcode_alloc(get_size_from_version(version), version);
    Array value_matrix = {qrcode->matrix, qrcode->size, qrcode->size};
    Array mask_matrix = {qrcode->are_taken, qrcode->size, qrcode->size};
        Array* mask_pattern = mask_convert_to_binary(mask);
    Array* format_version = information_compute_format(
        information_get_error_correction_level(correction_mode),
        mask_pattern
    );
    Array* binary_information = process_information(
        qrcode, 
        string, 
        encoding_mode
    );

    pattern_reserve_all_patterns(&mask_matrix);


    // Note: error correction step 
    Polynomial* information_poly = polynomial_create_from_info(binary_information);
    Array* final_array = array_alloc(binary_information->size + 8 * information_poly->degree);

    polynomial_devide(information_poly);
    array_append(final_array, binary_information);

    // Note: add the binary encoded error corrections
    for(size_t i = 0; i < information_poly->degree; i++) {
        Array* int_to_bin = encoding_encode_int_to_binary(information_poly->values[i], 8);
        array_append(final_array, int_to_bin);
        array_free(int_to_bin);
    }

    qrcode_insert_information(qrcode, final_array, qrcode->size * qrcode->size - 1);

    // Note: masking step
    mask_apply_mask(&value_matrix, mask);

    // Note: these must not be masked
    qrcode_insert_version_format(qrcode, format_version);
    pattern_set_all_finders(&value_matrix);
    pattern_set_timings_n_dark(&value_matrix);

    array_free(mask_pattern);
    array_free(binary_information);
    array_free(format_version);
    array_free(final_array);

    polynomial_free(information_poly);

    return qrcode;
}
