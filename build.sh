#!/bin/sh

set -xe

clang -Wall -Wextra -o main.out main.c \
    src/qrcode/qrcode.c src/qrcode/pattern.c src/qrcode/utils.c \
    src/qrcode/information.c src/qrcode/encoding.c
