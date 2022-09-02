#include <stdbool.h>
#include <stdio.h>

#include "qrcode.h"
#include "utils.h"

QRCode* qrcode_init(size_t size, int version) {
    QRCode* qrcode = (QRCode*) malloc(sizeof(QRCode));

    qrcode->matrix = (bool*) calloc(size * size, sizeof(bool));
    qrcode->are_taken = (bool*) calloc(size* size, sizeof(bool) * size);
    qrcode->size = size;
    qrcode->version = version;

    return qrcode;
}

void qrcode_free(QRCode* self) {
    free(self->matrix);
    free(self->are_taken);
    free(self);
}

static void qrcode_display_matrix(FILE* stream, size_t size, int version, bool* matrix, bool is_qrcode) {
    char info_message[64];

    sprintf(
        info_message, 
        "QR Code version: %d | width size: %zu%s\n\n",
        version,
        size,
        is_qrcode ? "" : " | Availability mask");
    fputs(info_message, stream);

    for(size_t i = 0; i < size * size; i++) {
        fputc(" #"[matrix[i]], stream);
        fputc(' ', stream);

        if(i && (i + 1) % size == 0) {
            fputc('\n', stream);
        }
    }
}

void qrcode_display(QRCode* self, FILE* stream) {
    qrcode_display_matrix(stream, self->size, self->version, self->matrix, true);
}

void qrcode_display_availability_mask(QRCode* self, FILE* stream) {
    qrcode_display_matrix(stream, self->size, self->version, self->are_taken, false);
}

void qrcode_insert_information(QRCode* self, Array* information, size_t current_idx) {
    Array matrix = {self->are_taken, self->size, self->size};

    // TODO improve performance: at the moment need to go through all 
    // cells each time
    for(size_t i = 0; i < information->size; i++) {
        current_idx = utils_get_next_available_idx(current_idx, &matrix);
        printf("qrcode_insert_information - %zu, %zu, %d, %d %d\n", 
            i, 
            current_idx, 
            self->matrix[current_idx],
            self->are_taken[current_idx],
            information->values[i]
        );
        self->matrix[current_idx] = information->values[i];
        self->are_taken[current_idx] = true;

        // qrcode_display(self, stdout);
        printf("%zu\n", i);
        qrcode_display_availability_mask(self, stdout);
    }
}

void qrcode_insert_version_format(QRCode* self, Array* information) {
    for(size_t i = 0; i < 7; i++) {
        size_t h_idx = 8 * self->size + self->size - 7 + i;
        size_t v_idx = 8 + (self->size - 7 + i) * self->size;

        self->matrix[h_idx] = information->values[8 + i];
        // self->matrix[h_idx] = information->values[6 - i];
        self->matrix[v_idx] = information->values[6 - i];
    }
    
    for(size_t i = 0; i < 6; i++) {
        size_t h_idx = 8 * self->size + i;
        size_t v_idx = 8 + i * self->size;

        self->matrix[h_idx] = information->values[i];
         // self->matrix[h_idx] = information->values[information->size - 1 - i];
        self->matrix[v_idx] = information->values[information->size - 1 - i];
    }

    self->matrix[8 * self->size + 7] = information->values[6];
    // self->matrix[8 * self->size + 7] = information->values[8];
    self->matrix[8 * self->size + 8] = information->values[7];
    // TODO: This one is false? It covers the first timing
    self->matrix[7 * self->size + 8] = information->values[8];
    // self->matrix[7 * self->size + 8] = information->values[6];
    self->matrix[8 * self->size + self->size - 7 - 1] = information->values[7];
}
