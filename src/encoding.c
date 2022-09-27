#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "utils.h"
#include "encoding.h"
#include "information.h"

static const char* ALPHANUMERIC_CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

// TODO: warning when out of bound
Array* encode_int_to_binary(int value, size_t bit_count) {
    Array* array = array_alloc_zeros(bit_count);
    size_t idx = array->capacity - array->size - 1;
    array->size = bit_count;

    while(value) {
        array->values[idx] = value % 2;
        value /= 2;
        idx--;
    }

    return array;
}

int decode_binary_to_int(Array* arr) {
    int result = 0;
    for(size_t i = 0; i < arr->size; i++) {
        result += pow(2, i) * arr->values[arr->size - 1 - i];
    }

    return result;
}

int get_alphanumeric_value(char character) {
    if(!strchr(ALPHANUMERIC_CHARS, character)) {
        fprintf(stderr, "ERROR `%c` is not an alphanumeric character\n", character);
        exit(1);
    }

    return (int) (strchr(ALPHANUMERIC_CHARS, character) - ALPHANUMERIC_CHARS);
}

static int encode_alphanumeric_pair(char char1, char char2) {
    // printf("alpha_pair `%c` `%c` %d\n", char1, char2, char2 == '\0');
    int first_char_encoding = get_alphanumeric_value(char1);

    return char2 == '\0' ? first_char_encoding 
        : first_char_encoding * 45 + get_alphanumeric_value(char2);
}

int* encode_alphanumeric_values(char* string, size_t length) {
    size_t pairs_length = length / 2 + length % 2;
    int* result = (int*) malloc(sizeof(int) * pairs_length);
    // printf("-- `%s` %zu %zu \n", string, length, strlen(string));

    for(size_t i = 0; i < length; i += 2) {
        result[i / 2] = encode_alphanumeric_pair(
            string[i],
            i < length -1 ? string[i + 1] : '\0'
        );
    }

    return result;
}

Array* encode_alphanumeric_string(char* string) {
    const size_t ALPHANUMERIC_BIT_LENGTH = 11;
    const size_t ALPHANUMERIC_ODD_BIT_LENGTH = 6;
    size_t length = strlen(string);
    size_t pairs_length = length / 2 + length % 2;
    size_t binary_length = pairs_length * ALPHANUMERIC_BIT_LENGTH;
    Array* result = array_alloc(binary_length);
    printf("LENGTH %zu\n", length);
    int* pairs_values = encode_alphanumeric_values(string, length);

    for(size_t i = 0; i < length / 2; i++) {
        Array* binary_encoding = encode_int_to_binary(
            pairs_values[i],
            ALPHANUMERIC_BIT_LENGTH
        );
        array_append_full(result, binary_encoding);
        array_free(binary_encoding);
    }
    
    if(length % 2) {
        Array* binary_encoding = encode_int_to_binary(
            pairs_values[pairs_length - 1],
            ALPHANUMERIC_ODD_BIT_LENGTH
        );
        array_display(binary_encoding);
        array_append_full(result, binary_encoding);
        array_free(binary_encoding);
    }

    return result;
}

static Array* add_terminator(Array* self) {
    int maximum = 4;

    while(maximum-- && self->size < self->capacity) {
        array_append_value(self, false);
    }

    return self;
}

static Array* pad_to_eight_bits(Array* self) {
    while(self->size % 8) {
        array_append_value(self, false);
    }

    return self;
}

static Array* add_final_padding(Array* self) {
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
        array_append_full(self, &x);
    }

    return self;
}

// Note: the array must already be the size of the final message
Array* pad_codewords(Array* self) {
    add_terminator(self);
    pad_to_eight_bits(self);
    return add_final_padding(self);
}

// TODO: complete later
size_t get_number_codewords(int version, EncodingMode mode) {
    assert(version == 1 && mode == ALPHANUMERIC);
    // TODO: set back to 16 after tests
    // return 16;
    return 16;
}
