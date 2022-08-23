#include "utils.h"

#ifndef ENCODING_H
#define ENCODING_H

Array* encoding_encode_int_to_binary(int value, size_t bit_count);
Array* encoding_encode_alphanumeric_string(char* string);
int* encoding_alphanumeric_values(char* string, size_t length);

#endif // ENCODING_H
