#include "mask.h" 
#include "encoding.h"

Array* mask_convert_to_binary(MaskPattern mask) {
    return encoding_encode_int_to_binary(mask, 3);
}

static bool mask_compute_mask0(size_t idx, size_t qrcode_size) {
    return (((idx % qrcode_size) + (idx / qrcode_size)) % 2 == 0);
}

static bool mask_compute_mask1(size_t idx, size_t qrcode_size) {
    return ((idx / qrcode_size) % 2 == 0);
}

static bool mask_compute_mask2(size_t idx, size_t qrcode_size) {
    return ((idx % qrcode_size) % 3 == 0);
}

static bool mask_compute_mask3(size_t idx, size_t qrcode_size) {
    return (((idx % qrcode_size) + (idx / qrcode_size)) % 3 == 0);
}

static bool mask_compute_mask4(size_t idx, size_t qrcode_size) {
    return (((idx % qrcode_size) / 3 + (idx / qrcode_size) / 2) % 2 == 0);
}

static bool mask_compute_mask5(size_t idx, size_t qrcode_size) {
    size_t j = (idx % qrcode_size);
    size_t i = (idx / qrcode_size);

    return ((i * j % 2) + (i * j) % 3) == 0;
}

static bool mask_compute_mask6(size_t idx, size_t qrcode_size) {
    size_t j = (idx % qrcode_size);
    size_t i = (idx / qrcode_size);

    return ((i * j % 2) + (i * j) % 3) % 2 == 0;
}

static bool mask_compute_mask7(size_t idx, size_t qrcode_size) {
    size_t j = (idx % qrcode_size);
    size_t i = (idx / qrcode_size);

    return ((i + j % 2) + (i * j) % 3) % 2 == 0;
}

// TODO: change masking function according to the parameter 
Array* mask_apply_mask(Array* matrix, MaskPattern mask) {
    MaskingFunction func = mask_from_factory(mask);
    for(size_t i = 0; i < matrix->size * matrix->size; i++) {
        matrix->values[i] ^= func(i, matrix->size);
    }

    return matrix;
}

MaskingFunction mask_from_factory(MaskPattern mask) {
    switch(mask) {
        case MASK_0:
            return mask_compute_mask0;
        case MASK_1:
            return mask_compute_mask1;
        case MASK_2:
            return mask_compute_mask2;
        case MASK_3:
            return mask_compute_mask3;
        case MASK_4:
            return mask_compute_mask4;
        case MASK_5:
            return mask_compute_mask5;
        case MASK_6:
            return mask_compute_mask6;
        case MASK_7:
            return mask_compute_mask7;
    }
}