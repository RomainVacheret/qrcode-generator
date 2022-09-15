#include <stdbool.h>
#include <string.h>

#include "src/qrcode.h"
#include "src/logger.h"

int main() {
    LOGGER = logger_alloc(DEBUG, stdout); 
    char* alph_string = "HELLO WORLD";
    EncodingMode encoding_mode = ALPHANUMERIC;
    ErrorCorrectionLevel correction_mode = M;
    // TODO: fix format string when mask != 2
    // Warning: do not use another mask!
    MaskPattern mask = MASK_2;
    /*
        FORMAT STRING
        masks:
            0: seg fault, should be: 101010000010010
            1: false, is: [1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0], should be: 101000100100101
            2: ok
            3: ok
            4: false, is: [1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0], should be 100010111111001
            5: false, is, [1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0], should be 100000011001110
            6: ok
            7: ok
    */
    int version = 1;
    QRCode* qrcode = qrcode_generate(
        alph_string,
        correction_mode,
        encoding_mode,
        mask,
        version
    );

    // TODO: replace the Python scrip with OpenGL
    // Note: export to display using the Python script
    FILE* file = fopen("result.txt", "w");
    qrcode_display(qrcode, file);
    fclose(file);

    qrcode_free(qrcode);

    return EXIT_SUCCESS;
}
