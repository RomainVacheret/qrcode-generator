#include <stdbool.h>
#include <stdio.h>

#include "qrcode.h"
#include "utils.h"

QRCode* qrcode_init(size_t size, int version) {
    QRCode* qrcode = (QRCode*) malloc(sizeof(QRCode));

    qrcode->matrix = (bool*) calloc(size * size, sizeof(bool));
    qrcode->are_empty = (bool*) calloc(size* size, sizeof(bool) * size);
    qrcode->size = size;
    qrcode->version = version;

    return qrcode;
}

void qrcode_free(QRCode* self) {
    free(self->matrix);
    free(self->are_empty);
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
    qrcode_display_matrix(stream, self->size, self->version, self->are_empty, false);
}

void qrcode_insert_information(QRCode* self, Array* information, size_t current_idx) {
    Array matrix = {self->matrix, self->size, self->size};

    // TODO improve performance: at the moment need to go through all 
    // cells each time
    for(size_t i = 0; i < information->size; i++) {
        current_idx = utils_get_next_available_idx(current_idx, &matrix);
        self->matrix[current_idx] = information->values[i];
        self->are_empty[current_idx] = true;
        printf("qrcode_insert_information - %zu, %zu, %d, %d\n", i, current_idx, self->matrix[current_idx], self->are_empty[current_idx]);
    }
}
