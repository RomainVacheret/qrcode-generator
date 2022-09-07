#include "mask.h" 
#include "encoding.h"



Array* mask_convert_to_binary(MaskPattern mask) {
    return encoding_encode_int_to_binary(mask, 3);

}

static bool mask_compute_mask2(size_t idx, size_t qrcode_size) {
    return ((idx % qrcode_size) % 3 == 0);
}

// TODO: change masking function according to the parameter 
Array* mask_apply_mask(Array* matrix, MaskPattern mask) {
    for(size_t i = 0; i < matrix->size * matrix->size; i++) {
        matrix->values[i] ^= mask_compute_mask2(i, matrix->size);
    }

    return matrix;
}
