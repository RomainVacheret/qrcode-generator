#include <stdio.h>
#include <assert.h>

#include "array.h"

Array* array_alloc(size_t size) {
    Array* array = (Array*) malloc(sizeof(Array));
    array->values = (bool*) malloc(sizeof(bool) * size);
    array->size = 0;
    array->capacity = size;

    return array;
}

Array* array_alloc_zeros(size_t size) {
    Array* array = (Array*) malloc(sizeof(Array));
    array->values = (bool*) calloc(size, sizeof(bool));
    array->size = 0;
    array->capacity = size;

    return array;
}

Array* array_alloc_values(size_t size, bool* values) {
    Array* array = array_alloc(size);
    array->size = size;

    for(size_t i = 0; i < size; i++) {
        array->values[i] = values[i];
    }

    return array;
}

void array_free(Array* self) {
    free(self->values);
    free(self);
}

// TODO: refactor
Array* array_concat(Array* arr1, Array* arr2) {
    Array* new_array = array_alloc(arr1->size + arr2->size);
    new_array->size = arr1->size + arr2->size;

    for(size_t i = 0; i < arr1->size; i++) {
        new_array->values[i] = arr1->values[i];
    }

    for (size_t i = 0; i < arr2->size; i++) {
        new_array->values[i + arr1->size] = arr2->values[i];
    }

    return new_array;
}

Array* array_concat_full(Array* arr1, Array* arr2) {
    Array* new_array = array_alloc(arr1->capacity + arr2->capacity);
    new_array->size = arr1->capacity + arr2->capacity;

    for(size_t i = 0; i < arr1->capacity; i++) {
        new_array->values[i] = arr1->values[i];
    }

    for (size_t i = 0; i < arr2->capacity; i++) {
        new_array->values[i + arr1->capacity] = arr2->values[i];
    }

    return new_array;
}

Array* array_append(Array* self, Array* other) {
    assert(self->size + other->size <= self->capacity);

    for(size_t i = 0; i < other->size; i++) {
        self->values[self->size + i] = other->values[i];
    }
    self->size += other->size;

    return self;
}

Array* array_append_full(Array* self, Array* other) {
    assert(self->size + other->capacity <= self->capacity);

    for(size_t i = 0; i < other->capacity; i++) {
        self->values[self->size + i] = other->values[i];
    }
    self->size += other->capacity;

    return self;
}

Array* array_append_value(Array* self, bool value) {
    assert(self->size < self->capacity);
    self->values[self->size++] = value;

    return self;
}

void array_display(Array* self) {
    printf("Array capacity: %zu | size: %zu\n\n[", self->capacity, self->size);
    for(size_t i = 0; i < self->size; i++) {
        printf("%d%c ", self->values[i], i != self->size - 1 ? ',' : ']');
    }
    printf("\n");
}

void array_display_full(Array* self) {
    printf("Array capacity: %zu | size: %zu\n\n[", self->capacity, self->size);
    for(size_t i = 0; i < self->capacity; i++) {
        printf("%d%c ", self->values[i], i != self->capacity - 1 ? ',' : ']');
    }
    printf("\n");
}
