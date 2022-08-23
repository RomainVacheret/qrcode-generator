#include <stdlib.h>
#include <stdbool.h>

#include "information.h"

bool* information_get_error_correction_level(ErrorCorrectionLevel level) {
    bool* information = (bool*) malloc(sizeof(bool) * 2);

    switch(level) {
        case L: 
            information[0] = false;
            information[1] = true;
            break;
        case M:
            information[0] = false;
            information[1] = false;
            break;
        case Q:
            information[0] = true;
            information[1] = true;
                break;
        case H:
            information[0] = true;
            information[1] = false;
    }

    return information;
}

bool* information_get_encoding_mode(EncodingMode mode) {
    bool* information = (bool*) malloc(sizeof(bool) * 4);

    switch(mode) {
        case NUMERIC:
            information[0] = 0;
            information[1] = 0;
            information[2] = 0;
            information[3] = 1;
            break;
        case ALPHANUMERIC:
            information[0] = 0;
            information[1] = 0;
            information[2] = 1;
            information[3] = 0;
            break;
        case BYTE:
            information[0] = 0;
            information[1] = 1;
            information[2] = 0;
            information[3] = 0;
            break;
        case KANJI:
            information[0] = 1;
            information[1] = 0;
            information[2] = 0;
            information[3] = 0;
            break;
        case ECI:
            information[0] = 0;
            information[1] = 1;
            information[2] = 1;
            information[3] = 1;
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
