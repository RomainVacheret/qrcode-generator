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

// TODO: add masking

#endif // INFORMATION_H
