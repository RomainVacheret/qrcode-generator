#include <stdio.h>
#include <assert.h>

#include "utils.h"

void utils_set_square_ring(
    Array* matrix,
    size_t top_left_index, 
    size_t width, 
    bool value) {
        for(size_t i = 0; i < width - 1; i++) {
            // top left  -> top right - 1
            matrix->values[top_left_index + i] = value; 
            // top->values right -> bottom right - 1
            matrix->values[top_left_index + (width - 1) + (matrix->capacity * i)] = value;
            // top->values left + 1 -> bottom left
            matrix->values[top_left_index + (matrix->capacity * (i + 1))] = value;
            // bot->valuestom right -> bottom left - 1
            matrix->values[top_left_index + (width - 1) + ((width - 1) * matrix->capacity) - i] = value; 
        }
}

void utils_set_vertical_line(
    Array* matrix,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool value) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->capacity] = value;
        }
}

void utils_set_vertical_line_values(
    Array* matrix,
    Array* empty,
    size_t column,
    size_t start_row, 
    size_t end_row, 
    bool* values) {
        for(size_t i = 0; i < end_row - start_row + 1; i++) {
            matrix->values[column + (start_row + i) * matrix->capacity] = values[i];
            empty->values[column + (start_row + i) * matrix->capacity] = true;
        }
}

void utils_set_horizontal_line(
    Array* matrix,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool value) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            matrix->values[row * matrix->capacity + start_column + i] = value;
        }
}

void utils_set_horizontal_line_values(
    Array* matrix,
    Array* empty,
    size_t row, 
    size_t start_column,
    size_t end_column,
    bool* values) {
        for(size_t i = 0; i < end_column - start_column + 1; i++) {
            matrix->values[row * matrix->capacity + start_column + i] = values[i];
            matrix->values[row * matrix->capacity + start_column + i] = true;
        }
}

size_t utils_get_next_idx(size_t current_idx, size_t matrix_size) {
    printf("utils_get_next_idx - %zu\n", current_idx);
    if(!current_idx) {
        fprintf(stderr, "ERROR: invalid index (0)");
        exit(1);
    } else if(current_idx % 2) {
        return current_idx < matrix_size ? 
            matrix_size * matrix_size - (matrix_size - (current_idx - 1)) : 
            current_idx - matrix_size + 1;
    } else {
            return current_idx - 1;
    }
}

size_t utils_get_next_available_idx(size_t current_idx, Array* matrix) {
    size_t next_idx;
    int is_available = matrix->values[current_idx] == false;

    do {
        next_idx = is_available? current_idx : 
            utils_get_next_idx(current_idx, matrix->capacity);
        printf("utils_get_next - %zu\n", next_idx);
        is_available = matrix->values[next_idx] == false;
        current_idx = next_idx;
    } while(!is_available);

    return next_idx;
}

Array* utils_alloc_array(size_t size) {
    Array* array = (Array*) malloc(sizeof(Array));
    array->values = (bool*) malloc(sizeof(bool) * size);
    array->size = 0;
    array->capacity = size;

    return array;
}

Array* utils_alloc_array_zeros(size_t size) {
    Array* array = (Array*) malloc(sizeof(Array));
    array->values = (bool*) calloc(size, sizeof(bool));
    array->size = 0;
    array->capacity = size;

    return array;
}

Array* utils_alloc_array_values(size_t size, bool* values) {
    Array* array = utils_alloc_array(size);
    for(size_t i = 0; i < size; i++) {
        array->values[i] = values[i];
    }

    return array;
}

void utils_free_array(Array* self) {
    free(self->values);
    free(self);
}

// TODO: refactor
Array* utils_concat_arrays_size(Array* arr1, Array* arr2) {
    Array* new_array = utils_alloc_array(arr1->size + arr2->size);
    new_array->size = arr1->size + arr2->size;

    for(size_t i = 0; i < arr1->size; i++) {
        new_array->values[i] = arr1->values[i];
    }

    for (size_t i = 0; i < arr2->size; i++) {
        new_array->values[i + arr1->size] = arr2->values[i];
    }

    return new_array;
}

Array* utils_concat_arrays(Array* arr1, Array* arr2) {
    Array* new_array = utils_alloc_array(arr1->capacity + arr2->capacity);
    new_array->size = arr1->capacity + arr2->capacity;

    for(size_t i = 0; i < arr1->capacity; i++) {
        new_array->values[i] = arr1->values[i];
    }

    for (size_t i = 0; i < arr2->capacity; i++) {
        new_array->values[i + arr1->capacity] = arr2->values[i];
    }

    return new_array;
}

Array* utils_append_arrays(Array* self, Array* other) {
    assert(self->size + other->size <= self->capacity);

    for(size_t i = 0; i < other->size; i++) {
        self->values[self->size + i] = other->values[i];
    }
    self->size += other->size;

    return self;
}

Array* utils_append_arrays_full(Array* self, Array* other) {
    assert(self->size + other->capacity <= self->capacity);

    for(size_t i = 0; i < other->capacity; i++) {
        self->values[self->size + i] = other->values[i];
    }
    self->size += other->capacity;

    return self;
}

Array* utils_append_array(Array* self, bool value) {
    assert(self->size < self->capacity);
    self->values[self->size++] = value;

    return self;
}

void utils_display_array(Array* self) {
    printf("Array capacity: %zu | size: %zu\n\n[", self->capacity, self->size);
    for(size_t i = 0; i < self->size; i++) {
        printf("%d%c ", self->values[i], i != self->size - 1 ? ',' : ']');
    }
    printf("\n");
}

void utils_display_array_full(Array* self) {
    printf("Array capacity: %zu | size: %zu\n\n[", self->capacity, self->size);
    for(size_t i = 0; i < self->capacity; i++) {
        printf("%d%c ", self->values[i], i != self->capacity - 1 ? ',' : ']');
    }
    printf("\n");
}
