#include <stdbool.h>

#include "array.h"

#ifndef MASK_H
#define MASK_H

typedef enum {
    MASK_0, MASK_1, MASK_2, MASK_3,
    MASK_4, MASK_5, MASK_6, MASK_7
} MaskPattern;

typedef bool (*MaskingFunction)(size_t, size_t);

Array* mask_convert_to_binary(MaskPattern mask);
Array* mask_apply_mask(Array* matrix, MaskPattern mask);

MaskingFunction mask_from_factory(MaskPattern mask);

#endif // MASK_H
