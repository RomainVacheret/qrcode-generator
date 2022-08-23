#include "src/qrcode/qrcode.h"
#include "src/qrcode/pattern.h"
#include "src/qrcode/utils.h"

#define WIDTH_SIZE 21
#define VERSION 1

int main() {
    QRCode* qrcode = qrcode_init(WIDTH_SIZE, VERSION);
    Array value_matrix = {qrcode->matrix, WIDTH_SIZE};
    Array mask_matrix = {qrcode->are_empty, WIDTH_SIZE};
    // size_t before = 439;
    // size_t result = utils_get_next_idx(before, WIDTH_SIZE);
    bool booleans[] = {true, true,};
    Array information = {booleans, 2};

    // qrcode->matrix[before] = '4';
    // qrcode->matrix[result] = '5';

    pattern_set_all_finders(&value_matrix);
    pattern_reserve_all_patterns(&mask_matrix);

    qrcode_insert_information(qrcode, &information, WIDTH_SIZE * WIDTH_SIZE - 1);

    qrcode_display(qrcode, stdout);
    qrcode_display_availability_mask(qrcode, stdout);

    return EXIT_SUCCESS;
}

