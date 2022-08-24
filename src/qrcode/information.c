#include <stdlib.h>
#include <stdbool.h>

#include "information.h"
#include "utils.h"

Array* information_get_error_correction_level(ErrorCorrectionLevel level) {
    Array* information = utils_alloc_array(2);
    information->size = 2;

    switch(level) {
        case L: 
            information->values[0] = false;
            information->values[1] = true;
            break;
        case M:
            information->values[0] = false;
            information->values[1] = false;
            break;
        case Q:
            information->values[0] = true;
            information->values[1] = true;
                break;
        case H:
            information->values[0] = true;
            information->values[1] = false;
    }

    return information;
}

Array* information_get_encoding_mode(EncodingMode mode) {
    Array* information = utils_alloc_array(4);
    information->size = 4;

    switch(mode) {
        case NUMERIC:
            information->values[0] = 0;
            information->values[1] = 0;
            information->values[2] = 0;
            information->values[3] = 1;
            break;
        case ALPHANUMERIC:
            information->values[0] = 0;
            information->values[1] = 0;
            information->values[2] = 1;
            information->values[3] = 0;
            break;
        case BYTE:
            information->values[0] = 0;
            information->values[1] = 1;
            information->values[2] = 0;
            information->values[3] = 0;
            break;
        case KANJI:
            information->values[0] = 1;
            information->values[1] = 0;
            information->values[2] = 0;
            information->values[3] = 0;
            break;
        case ECI:
            information->values[0] = 0;
            information->values[1] = 1;
            information->values[2] = 1;
            information->values[3] = 1;
    }
    
    return information;
}

size_t information_get_char_count(int version, EncodingMode mode) {
    return ((int[3][4]) {
        {10, 9, 8, 8},
        {12, 11, 16, 10},
        {14, 13, 16, 12}
    })[(version > 9) + (version > 26)][mode];
}

bool* information_generate_format_information(bool* error_code_level, bool* mask_pattern_reference);

bool* information_xor_format_information(bool* format_information) {
    bool mask_pattern[] = {
        true, false, true, false, true, 
        false, false, false, false, false,
        true, false, false, true, false
    };

    for(size_t i = 0; i < 15; i++) {
        format_information[i] ^= mask_pattern[i];
    }

    return format_information;
}
