#include <stdio.h>
#include <assert.h>

#include "array.h"
#include "logger.h"

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
    array->size = 0;

    for(size_t i = 0; i < size; i++) {
        array->values[i] = values[i];
        array->size++;
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

    // -- logging --
    char* str;
    char* arr1_str = array_as_string(arr1);
    char* arr2_str = array_as_string(arr2);
    char* result_str = array_as_string(new_array);

    asprintf(
        &str, 
        "array_concat_full - arr1: %s, arr2: %s, result: %s",
        arr1_str,
        arr2_str,
        result_str
    );
    logger_write(LOGGER, str, DEBUG);
    free(str);
    free(arr1_str);
    free(arr2_str);
    free(result_str);

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

// Warning: must be freed after usage!
char* array_as_string(Array* self) {
    char* str = "[";
    
    // TODO: memory leak with the successive calls to asprintf without freeing str?
    for(size_t i = 0; i < self->size; i++) {
        asprintf(
            &str, 
            "%s%d%c ", 
            str, 
            self->values[i],
            i != self->size - 1 ? ',' : ']'
        );
    }

    return str;
}

Array* array_copy(Array* self) {
    return array_alloc_values(self->capacity, self->values);
}

bool array_is_full_zeros(Array* self) {
    for(size_t i = 0; i < self->size; i++) {
        if(self->values[i]) {
            return false;
        }
    }

    return true;
}


Array* array_perform_xor(Array* self, Array* other) {
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
        "array_perform_xor - self: %s, other: %s, result: %s",
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

Array* array_remove_leading_zeros(Array* self) {
    size_t idx = 0, zero_count = 0;
    Array* self_copy = array_copy(self);

	// In case of M / 4 which results in 00 / 000
    if(array_is_full_zeros(self)) {
        char* str;
        asprintf(
            &str, 
            "array_remove_leading_zeros - size: %zu, capacity %zu - is full or zeros",
            self->size,
            self->capacity
        );
        logger_write(LOGGER, str, WARNING);
        free(str);

        return self;
    }

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
        "array_remove_leading_zeros - self: %s, result: %s",
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