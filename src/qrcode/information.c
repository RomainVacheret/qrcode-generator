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
