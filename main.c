#include <stdbool.h>
#include <string.h>

#include "src/qrcode.h"

int main() {
    char* alph_string = "HELLO WORLD";
    EncodingMode encoding_mode = ALPHANUMERIC;
    ErrorCorrectionLevel correction_mode = M;
    // TODO: fix format string when mask != 2
    // Warning: do not use another mask!
    MaskPattern mask = MASK_2;
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
