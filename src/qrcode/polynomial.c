#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "polynomial.h"
#include "log_antilog.h"


Polynomial* polynomial_alloc(size_t degree, NotationMode mode) {
    Polynomial* poli = (Polynomial*) malloc(sizeof(Polynomial));
    poli->values = (int*) malloc(sizeof(int) * degree);
    poli->degree = degree;
    poli->added_degree = 0;
    poli->mode = mode;

    return poli;
}

Polynomial* polynomial_alloc_zeros(size_t degree, NotationMode mode) {
    Polynomial* poli = (Polynomial*) malloc(sizeof(Polynomial));
    poli->values = (int*) calloc(degree, sizeof(int));
    poli->degree = degree;
    poli->added_degree = 0;
    poli->mode = mode;

    return poli;
}

Polynomial* polynomial_alloc_values(size_t degree, int* values, NotationMode mode) {
    Polynomial* poli = polynomial_alloc(degree, mode);

    while(degree--) {
        poli->values[degree] = values[degree];
    }

    return poli;
}

void polynomial_free(Polynomial* self) {
    free(self->values);
    free(self);
}

Polynomial* polynomial_get_1M_generator() {
    return polynomial_alloc_values(11, (int[11]) {
        0, 251, 67, 46, 61, 118, 70, 64, 94, 32, 45
    }, ALPHA);
}

Polynomial* polynomial_convert(Polynomial* self) {
    LogAntilogTable* table = lat_initialize_table();
    int* selected_table = self->mode == DECIMAL ? table->log : table->antilog; // : table->log;
    self->mode = (self->mode + 1) % 2;

    for(size_t i = 0; i < table->size; i++) {
        self->values[i] = selected_table[(self->values[i] + self->added_degree) % 255];
    }
    
    return self;
}

void polynomial_display(Polynomial* self) {
    printf("Poli - mode: %s, degree: %zu, added_degree: %zu\n", 
        ((char*[2]) {"DECIMAL", "ALPHA"})[self->mode],
        self->degree, self->added_degree
    );

    for(size_t i = 0; i < self->degree; i++) {
        printf("%d%c", self->values[i], (i == self->degree - 1) ? '\n' : ',');
    }
}


// int main() {
//     Polynomial* generator = polynomial_get_1M_generator();
//     generator->added_degree = 5;
//
// }
