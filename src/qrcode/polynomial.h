#include <stdlib.h>

#ifndef POLYNOMIAL_H 
#define POLYNOMIAL_H

typedef enum {
    DECIMAL, ALPHA
} NotationMode;

typedef struct {
    int* values;
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
void polynomial_display(Polynomial* self);

#endif // POLYNOMIAL_H
