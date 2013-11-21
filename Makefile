all: encoder iassim

encoder:
    gcc -o encoder ./src/ias-hex-encoder/ias-hex-encoder.c

iassim:
    gcc -o iassim ./src/ias-simulator/*

