#include "utils.h"
#include "information.h"

#ifndef ENCODING_H
#define ENCODING_H

Array* encoding_encode_int_to_binary(int value, size_t bit_count);
Array* encoding_encode_alphanumeric_string(char* string);
int* encoding_alphanumeric_values(char* string, size_t length);

Array* encoding_pad_codewords(Array* self);
size_t encoding_get_number_codewords(int version, EncodingMode mode);

#endif // ENCODING_H
