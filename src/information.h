#include <stdbool.h>

#include "utils.h"

#ifndef INFORMATION_H
#define INFORMATION_H

typedef enum {
    L, M, Q, H
}ErrorCorrectionLevel;

typedef enum {
    NUMERIC, ALPHANUMERIC, BYTE, KANJI, ECI 
}EncodingMode;

Array* get_error_correction_level(ErrorCorrectionLevel level);
Array* get_encoding_mode(EncodingMode mode);

Array* generate_format_information(Array* format_info);
Array* xor_format_information(Array* format_information);
Array* get_generator_polynomial();

Array* array_remove_leading_zeros(Array* self);
Array* information_devide(Array* self);
Array* information_devide_full(Array* self);
Array* compute_format(Array* error_code_level, Array* mask_pattern_reference);


// TODO: add masking

#endif // INFORMATION_H
