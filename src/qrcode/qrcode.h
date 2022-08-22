#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#ifndef QRCODE_H
#define QRCODE_H

typedef struct {
    int version;
    size_t size;
    bool* matrix;
    bool* are_empty;
} QRCode;


QRCode* qrcode_init(size_t size, int version);
void qrcode_free(QRCode* self);

void qrcode_display(QRCode* self, FILE* stream);
void qrcode_display_availability_mask(QRCode* self, FILE* stream);

#endif // QRCODE_H
