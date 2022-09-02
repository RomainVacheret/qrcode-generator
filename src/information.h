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

Array* information_get_error_correction_level(ErrorCorrectionLevel level);
Array* information_get_encoding_mode(EncodingMode mode);

Array* information_generate_format_information(Array* format_info);
Array* information_xor_format_information(Array* format_information);
Array* information_get_generator_polynomial();

Array* information_remove_leading_zeros(Array* self);
Array* information_devide(Array* self);
Array* information_devide_full(Array* self);
Array* information_compute_format(Array* error_code_level, Array* mask_pattern_reference);


// TODO: add masking

#endif // INFORMATION_H
