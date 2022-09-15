#include <stdlib.h>
#include <stdbool.h>

#include "information.h"
#include "utils.h"
#include "logger.h"

// TODO: refactor with the new utils_alloc_array_values function
Array* information_get_error_correction_level(ErrorCorrectionLevel level) {
    Array* information = array_alloc(2);
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
    Array* information = array_alloc(4);
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

Array* information_generate_format_information(Array* format_info) {
    
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

Array* information_perform_xor(Array* self, Array* other) {
	Array* self_copy = array_copy(self);

    for(size_t i = 0; i < 15; i++) {
        self->values[i] ^= other->values[i];
    }

    // -- logging -- 
    char* str = "";
    char* self_str = array_as_string(self_copy);
    char* other_str = array_as_string(other);
    char* result_str = array_as_string(self);

    asprintf(
        &str, 
        "information_perform_xor - self: %s, other: %s, result: %s",
        self_str,
        other_str,
        result_str
    );
    logger_write(LOGGER, str, DEBUG);

    array_free(self_copy);
    free(str);
    free(self_str);
    free(other_str);
    free(result_str);

    return self;
}


// TODO: why does it break when there is only one of the xor when only
// one should be applied
Array* information_xor_format_information(Array* format_information) {
    Array* mask_pattern = array_alloc_values(15, (bool[15]) {
        true, false, true, false, true, 
        false, false, false, false, false,
        true, false, false, true, false
    });

    information_perform_xor(
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
Array* information_get_generator_polynomial() {
    Array* result =  array_alloc_values(15, (bool[15]) {
        true, false, true, false, false,
        true, true, false, true, true,
        true, false, false, false, false
    });
    result->size = 11;

    return result;
}

Array* information_remove_leading_zeros(Array* self) {
    size_t idx = 0, zero_count = 0;
    Array* self_copy = array_copy(self);

    while(!self->values[idx]){
        idx++;
        zero_count++;
    }

    for(size_t i = zero_count; i < self->size; i++) {
        self->values[i - zero_count] = self->values[i]; 
    }
    self->size -= zero_count;

    // -- logging --
    char* str;
    char* self_str = array_as_string(self_copy);
    char* result_str = array_as_string(self);

    asprintf(
        &str,
        "information_remove_leading_zeros - self: %s, result: %s",
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

Array* information_devide(Array* self) {
    Array* polynomial = information_get_generator_polynomial();
    polynomial->size = self->size;

    information_perform_xor(self, polynomial);
    information_remove_leading_zeros(self);
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


Array* information_compute_format(Array* error_code_level, Array* mask_pattern_reference) {
    char* str;
    Array* concatenated_information =  array_concat_full(
        error_code_level,
        mask_pattern_reference
    );
    Array* correction = information_generate_format_information(concatenated_information);

    information_remove_leading_zeros(correction);
    information_devide_full(correction);

    Array* result = array_concat(concatenated_information, correction);

    information_xor_format_information(result);
    array_free(concatenated_information);
    array_free(correction);

    // -- logging --
	char* error_str = array_as_string(error_code_level);
    char* mask_str = array_as_string(mask_pattern_reference);
    char* result_str = array_as_string(result);

    asprintf(
        &str, 
        "information_compute_format - error: %s, mask: %s, result: %s",
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
