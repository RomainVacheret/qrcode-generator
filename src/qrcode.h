#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

#include "utils.h"
#include "encoding.h"
#include "mask.h"

#ifndef QRCODE_H
#define QRCODE_H

typedef struct {
    int version;
    size_t size;
    bool* matrix;
    bool* are_taken;
} QRCode;

QRCode* qrcode_alloc(size_t size, int version);
void qrcode_free(QRCode* self);

void qrcode_display(QRCode* self, FILE* stream);
void qrcode_display_availability_mask(QRCode* self, FILE* stream);

void qrcode_insert_information(QRCode* self, Array* information, size_t current_idx);
void qrcode_insert_version_format(QRCode* self, Array* information);

QRCode* qrcode_generate(
    char* string, 
    ErrorCorrectionLevel correction_mode,
    EncodingMode encoding_mode, 
    MaskPattern mask,
    int version
);

#endif // QRCODE_H
