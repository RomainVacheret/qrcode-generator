#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "utils.h"
#include "encoding.h"
#include "information.h"

static const char* ALPHANUMERIC_CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

// TODO: warning when out of bound
Array* encoding_encode_int_to_binary(int value, size_t bit_count) {
    Array* array = utils_alloc_array_zeros(bit_count);
    size_t idx = array->capacity - array->size - 1;
    array->size = bit_count;

    while(value) {
        array->values[idx] = value % 2;
        value /= 2;
        idx--;
    }

    return array;
}

int encoding_get_alphanumeric_value(char character) {
    if(!strchr(ALPHANUMERIC_CHARS, character)) {
        fprintf(stderr, "ERROR `%c` is not an alphanumeric character\n", character);
        exit(1);
    }

    return (int) (strchr(ALPHANUMERIC_CHARS, character) - ALPHANUMERIC_CHARS);
}

static int encoding_encode_alphanumeric_pair(char char1, char char2) {
    printf("alpha_pair `%c` `%c` %d\n", char1, char2, char2 == '\0');
    int first_char_encoding = encoding_get_alphanumeric_value(char1);

    return char2 == '\0' ? first_char_encoding 
        : first_char_encoding * 45 + encoding_get_alphanumeric_value(char2);
}

int* encoding_alphanumeric_values(char* string, size_t length) {
    size_t pairs_length = length / 2 + length % 2;
    int* result = (int*) malloc(sizeof(int) * pairs_length);
    printf("-- `%s` %zu %zu \n", string, length, strlen(string));

    for(size_t i = 0; i < length; i += 2) {
        result[i / 2] = encoding_encode_alphanumeric_pair(
            string[i],
            i < length -1 ? string[i + 1] : '\0'
        );
    }

    return result;
}

Array* encoding_encode_alphanumeric_string(char* string) {
    const size_t ALPHANUMERIC_BIT_LENGTH = 11;
    const size_t ALPHANUMERIC_ODD_BIT_LENGTH = 6;
    size_t length = strlen(string);
    size_t pairs_length = length / 2 + length % 2;
    size_t binary_length = pairs_length * ALPHANUMERIC_BIT_LENGTH;
    Array* result = utils_alloc_array(binary_length);
    printf("LENGTH %zu\n", length);
    int* pairs_values = encoding_alphanumeric_values(string, length);

    for(size_t i = 0; i < length / 2; i++) {
        Array* binary_encoding = encoding_encode_int_to_binary(
            pairs_values[i],
            ALPHANUMERIC_BIT_LENGTH
        );
        utils_append_arrays_full(result, binary_encoding);
        utils_free_array(binary_encoding);
    }
    
    if(length % 2) {
        Array* binary_encoding = encoding_encode_int_to_binary(
            pairs_values[pairs_length - 1],
            ALPHANUMERIC_ODD_BIT_LENGTH
        );
        utils_display_array(binary_encoding);
        utils_append_arrays_full(result, binary_encoding);
        utils_free_array(binary_encoding);
    }

    return result;
}

static Array* encoding_add_terminator(Array* self) {
    int maximum = 4;

    while(maximum-- && self->size < self->capacity) {
        utils_append_array(self, false);
    }

    return self;
}

static Array* encoding_pad_to_eight_bits(Array* self) {
    while(self->size % 8) {
        utils_append_array(self, false);
    }

    return self;
}

static Array* encoding_add_final_padding(Array* self) {
    Array pad_bytes[2] = {{
        (bool[8]) {true, true, true, false, true, true, false, false}, // 236
        8,
        8
    }, {
        (bool[8]) {false, false, false, true, false, false, false, true}, // 17
        8,
        8
    }};
    int step_count = 0;

    while(self->size < self->capacity) {
        int idx = step_count++ % 2;
        Array x = pad_bytes[idx];
        utils_append_arrays_full(self, &x);
    }

    return self;
}

// Note: the array must already be the size of the final message
Array* encoding_pad_codewords(Array* self) {
    encoding_add_terminator(self);
    encoding_pad_to_eight_bits(self);
    return encoding_add_final_padding(self);
}

// TODO: complete later
size_t encoding_get_number_codewords(int version, EncodingMode mode) {
    assert(version == 1 && mode == ALPHANUMERIC);
    // TODO: set back to 16 after tests
    // return 16;
    return 16;
}
