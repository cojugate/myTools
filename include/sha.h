#ifndef SHA
#define SHA
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

void sha3_512(FILE* fp, uint8_t digest[]);
#endif
