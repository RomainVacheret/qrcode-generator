#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "information.h"
#include "utils.h"
#include "logger.h"

Array* get_error_correction_level(ErrorCorrectionLevel level) {
    switch(level) {
        case L: 
            return array_alloc_values(2, (bool[2]) {false, true});
        case M:
            return array_alloc_values(2, (bool[2]) {false, false});
        case Q:
            return array_alloc_values(2, (bool[2]) {true, true});
        case H:
            return array_alloc_values(2, (bool[2]) {true, false});
    }
}

Array* get_encoding_mode(EncodingMode mode) {
    switch(mode) {
        case NUMERIC:
            return array_alloc_values(4, (bool[4]) {false, false, false, true});
        case ALPHANUMERIC:
            return array_alloc_values(4, (bool[4]) {false, false, true, false});
        case BYTE:
            return array_alloc_values(4, (bool[4]) {false, true, false, false});
        case KANJI:
            return array_alloc_values(4, (bool[4]) {true, false, false, false});
        case ECI:
            return array_alloc_values(4, (bool[4]) {false, true, true, true});
    }
}

size_t get_char_count(int version, EncodingMode mode) {
    return ((int[3][4]) {
        {10, 9, 8, 8},
        {12, 11, 16, 10},
        {14, 13, 16, 12}
    })[(version > 9) + (version > 26)][mode];
}

Array* generate_format_information(Array* format_info) {
    
    // TODO: memory leak
    // WHY: segmentation fault if a variable is used 
    // Array* array_of_zeros = array_alloc_zeros(15);
    Array* full_information = array_append(
        array_alloc_zeros(15),
        // array_of_zeros,
        format_info
    );
    full_information->size = 15;

    // array_free(array_of_zeros);
    return full_information;
}




// TODO: why does it break when there is only one of the xor when only
// one should be applied
Array* xor_format_information(Array* format_information) {
    Array* mask_pattern = array_alloc_values(15, (bool[15]) {
        true, false, true, false, true, 
        false, false, false, false, false,
        true, false, false, true, false
    });

    array_perform_xor(
        format_information,
        mask_pattern
    );
    array_free(mask_pattern);

    return format_information;
}

// Note: we do not set the size to 15 because we do not yet know what size the
// information will be (which will be different for each division)
// Note: the "normal" size of polynomial is 11, we add zeros to pad because it 
// will needed later on
Array* get_generator_polynomial() {
    Array* result =  array_alloc_values(15, (bool[15]) {
        true, false, true, false, false,
        true, true, false, true, true,
        true, false, false, false, false
    });
    result->size = 11;

    return result;
}


Array* information_devide(Array* self) {
    Array* polynomial = get_generator_polynomial();
    polynomial->size = self->size;

    array_perform_xor(self, polynomial);
    array_remove_leading_zeros(self);
    array_free(polynomial);

    return self;
}

Array* information_devide_full(Array* self) {
    Array* self_copy = array_copy(self);

    while(self->size > 10) {
        information_devide(self);
    }

    // In case the result is less than 10 bits long
    while(self->size < 10) {
        self->values[self->size++] = 0;
    }

    // -- logging -- 
    char* str;
    char* self_str = array_as_string(self_copy);
    char* result_str = array_as_string(self);

    asprintf(
        &str, 
        "information_devide_full - self: %s, result: %s",
        self_str,
        result_str
    );
    logger_write(LOGGER, str, DEBUG);

	array_free(self_copy);
    free(str);
    free(self_str);
    free(result_str);

    return self;
}


Array* compute_format(Array* error_code_level, Array* mask_pattern_reference) {
    char* str;
    Array* concatenated_information =  array_concat_full(
        error_code_level,
        mask_pattern_reference
    );
    Array* correction = generate_format_information(concatenated_information);

    array_remove_leading_zeros(correction);
    information_devide_full(correction);

    Array* result = array_concat(concatenated_information, correction);

    xor_format_information(result);
    array_free(concatenated_information);
    array_free(correction);

    // -- logging --
	char* error_str = array_as_string(error_code_level);
    char* mask_str = array_as_string(mask_pattern_reference);
    char* result_str = array_as_string(result);

    asprintf(
        &str, 
        "compute_format - error: %s, mask: %s, result: %s",
        error_str,
        mask_str,
        result_str
    );
    logger_write(LOGGER, str, DEBUG);
    free(error_str);
    free(mask_str);
    free(result_str);
    free(str);

    // exit(1);
    return result;
}
