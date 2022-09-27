#include "utils.h"
#include "information.h"

#ifndef ENCODING_H
#define ENCODING_H

Array* encode_int_to_binary(int value, size_t bit_count);
int decode_binary_to_int(Array* arr);
Array* encode_alphanumeric_string(char* string);
int* encode_alphanumeric_values(char* string, size_t length);

Array* pad_codewords(Array* self);
size_t get_number_codewords(int version, EncodingMode mode);

#endif // ENCODING_H
