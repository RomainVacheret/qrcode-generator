#include <stdlib.h>

#include "utils.h"

#ifndef POLYNOMIAL_H 
#define POLYNOMIAL_H

typedef enum {
    DECIMAL, ALPHA
} NotationMode;

typedef struct {
    int* values;
    // Note: is not the real degree but more the length of the polynomial
    size_t degree;
    size_t added_degree;
    NotationMode mode;
} Polynomial;

Polynomial* polynomial_alloc(size_t degree, NotationMode mode);
Polynomial* polynomial_alloc_zeros(size_t degree, NotationMode mode);
Polynomial* polynomial_alloc_values(size_t degree, int* values, NotationMode mode);
void polynomial_free(Polynomial* self);
Polynomial* polynomial_get_1M_generator();
Polynomial* polynomial_convert(Polynomial* self);
Polynomial* polynomial_pop(Polynomial* self);
Polynomial* polynomial_convert_new(Polynomial* self);
Polynomial* polynomal_mul_alpha(Polynomial* self, int value);
void polynomial_display(Polynomial* self);
Polynomial* polynomial_create_from_info(Array* info);
Polynomial* polynomial_devide(Polynomial* poly);

#endif // POLYNOMIAL_H
