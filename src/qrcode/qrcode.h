#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "utils.h"

#ifndef QRCODE_H
#define QRCODE_H

typedef struct {
    int version;
    size_t size;
    bool* matrix;
    bool* are_taken;
} QRCode;

QRCode* qrcode_init(size_t size, int version);
void qrcode_free(QRCode* self);

void qrcode_display(QRCode* self, FILE* stream);
void qrcode_display_availability_mask(QRCode* self, FILE* stream);

void qrcode_insert_information(QRCode* self, Array* information, size_t current_idx);
void qrcode_insert_version_format(QRCode* self, Array* information);

#endif // QRCODE_H
