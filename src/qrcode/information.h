#ifndef INFORMATION_H
#define INFORMATION_H

typedef enum {
    L, M, Q, H
}ErrorCorrectionLevel;

bool* information_get_error_correction_level(ErrorCorrectionLevel level);

// TODO: add masking

#endif // INFORMATION_H
